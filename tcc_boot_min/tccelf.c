/*
 *  ELF file handling for TCC
 *
 *  Copyright (c) 2001-2004 Fabrice Bellard
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/********************************************************/
/* global variables */

/* predefined sections */
extern Section *text_section;
extern Section *data_section;
extern Section *bss_section;
extern Section *common_section;

extern Section *cur_text_section; /* current section where function code is generated */
/* symbol sections */
extern Section *symtab_section;

/* special flag to indicate that the section should not be linked to the other ones */
extern int SHF_PRIVATE; /* 0x80000000 */

/* 2 */
static void free_section(Section *s) {
    tcc_free(gs_data(s));
}

/* 3 */
ST_FUNC void tccelf_delete(TCCState *s1) {
    int i;

    /* free all sections */
    i = 1;
    while(lt(i, gts_nb_sections(s1))) {
        free_section(ri32(add(gts_sections(s1), mul(i, 4))));
        i = add(i, 1);
    }
    dynarray_reset(ats_sections(s1), ats_nb_sections(s1));

    i = 0;
    while(lt(i, gts_nb_priv_sections(s1))) {
        free_section(ri32(add(s1->priv_sections, mul(i, 4))));
        i = add(i, 1);
    }
    dynarray_reset(&s1->priv_sections, &s1->nb_priv_sections);

    symtab_section = 0; /* for tccrun.c:rt_printline() */
}

/* 4 */
/* save section data state */
ST_FUNC void tccelf_begin_file(TCCState *s1) {
    Section *s;
    int i;
    i = 1;
    while(lt(i, s1->nb_sections)) {
        s = s1->sections[i];
        s->sh_offset = s->data_offset;
        i = add(i, 1);
    }
    /* disable symbol hashing during compilation */
    s = s1->symtab;
    s->reloc = s->hash;
    s->hash = 0;
}

/* 5 */
/* At the end of compilation, convert any UNDEF syms to global, and merge
   with previously existing symbols */
ST_FUNC void tccelf_end_file(TCCState *s1) {
    Section *s;
    int first_sym;
    int nb_syms;
    int *tr;
    int i;
    Section *sr;
    int n;
    ElfW_Rel *rel;
    ElfW_Rel *rel_end;

    s = s1->symtab;

    first_sym = div_(s->sh_offset, sizeof (ElfSym));
    nb_syms = sub(div_(s->data_offset, sizeof (ElfSym)), first_sym);
    s->data_offset = s->sh_offset;
    s->link->data_offset = s->link->sh_offset;
    s->hash = s->reloc, s->reloc = NULL;
    tr = tcc_mallocz(nb_syms * sizeof *tr);

    i = 0;
    while(lt(i, nb_syms)) {
        ElfSym *sym = (ElfSym*)s->data + first_sym + i;
        if (and(eq(sym->st_shndx, SHN_UNDEF),
            eq(ELFW(ST_BIND)(sym->st_info), STB_LOCAL))) {
            sym->st_info = ELFW(ST_INFO)(STB_GLOBAL, ELFW(ST_TYPE)(sym->st_info));
        }
        tr[i] = set_elf_sym(s, sym->st_value, sym->st_size, sym->st_info,
            sym->st_other, sym->st_shndx, s->link->data + sym->st_name);

        i = add(i, 1);
    }
    /* now update relocations */
    i = 1;
    while(lt(i, s1->nb_sections)) {
        sr = s1->sections[i];
        if (and(eq(sr->sh_type, SHT_RELX), eq(sr->link, s))) {
            rel = (ElfW_Rel*)(add(sr->data, sr->sh_offset));
            rel_end = (ElfW_Rel*)(add(sr->data, sr->data_offset));
            while(lt(rel, rel_end)) {
                n = sub(ELFW(R_SYM)(rel->r_info), first_sym);
                rel->r_info = ELFW(R_INFO)(tr[n], ELFW(R_TYPE)(rel->r_info));
                rel = rel + 1;
            }
        }
        i = add(i, 1);
    }
    tcc_free(tr);
}

/* 6 */
Section *new_section(TCCState *s1, char *name, int sh_type, int sh_flags) {
    Section *sec;

    sec = tcc_mallocz(add(sizeof(Section), strlen(name)));
    strcpy(sec->name, name);
    sec->sh_type = sh_type;
    sec->sh_flags = sh_flags;

    if(or(or(eq(sh_type, SHT_HASH), eq(sh_type, SHT_REL)),
          or(eq(sh_type, SHT_RELA), eq(sh_type, SHT_SYMTAB)))) {
        sec->sh_addralign = 4;
    } else if (eq(sh_type, SHT_STRTAB)) {
        sec->sh_addralign = 1;
    } else {
        sec->sh_addralign =  PTR_SIZE; /* gcc/pcc default alignment */
    }

    if (and(sh_flags, SHF_PRIVATE)) {
        dynarray_add(&s1->priv_sections, &s1->nb_priv_sections, sec);
    } else {
        sec->sh_num = s1->nb_sections;
        dynarray_add(&s1->sections, &s1->nb_sections, sec);
    }

    return sec;
}

/* 7 */
ST_FUNC Section *new_symtab(TCCState *s1,
                           const char *symtab_name, int sh_type, int sh_flags,
                           const char *strtab_name,
                           const char *hash_name, int hash_sh_flags)
{
    Section *symtab;
    Section *strtab;
    Section *hash;
    int *ptr;
    int nb_buckets;

    symtab = new_section(s1, symtab_name, sh_type, sh_flags);
    symtab->sh_entsize = sizeof(ElfW(Sym));
    strtab = new_section(s1, strtab_name, SHT_STRTAB, sh_flags);
    put_elf_str(strtab, mks(""));
    symtab->link = strtab;
    put_elf_sym(symtab, 0, 0, 0, 0, 0, NULL);

    nb_buckets = 1;

    hash = new_section(s1, hash_name, SHT_HASH, hash_sh_flags);
    hash->sh_entsize = sizeof(int);
    symtab->hash = hash;
    hash->link = symtab;

    ptr = section_ptr_add(hash, mul(add(add(2, nb_buckets), 1), sizeof(int)));
    wi32(ptr, nb_buckets);
    wi32(add(ptr, 4), 1);
    memset(add(ptr, 2), 0, mul(add(nb_buckets, 1), sizeof(int)));
    return symtab;
}

/* 8 */
/* realloc section and set its content to zero */
ST_FUNC void section_realloc(Section *sec, unsigned long new_size) {
    int size;
    int data;

    size = sec->data_allocated;
    if (eq(size, 0)) {
        size = 1;
    }
    while (lt(size, new_size)) {
        size = mul(size, 2);
    }
    data = tcc_realloc(sec->data, size);
    memset(add(data, sec->data_allocated), 0, sub(size, sec->data_allocated));
    sec->data = data;
    sec->data_allocated = size;
}

/* 9 */
/* reserve at least 'size' bytes aligned per 'align' in section
   'sec' from current offset, and return the aligned offset */
ST_FUNC size_t section_add(Section *sec, addr_t size, int align) {
    int offset;
    int offset1;

    offset = and(sub(add(sec->data_offset, align), 1), sub(0, align));
    offset1 = add(offset, size);
    if (and(neq(sec->sh_type, SHT_NOBITS), gt(offset1, sec->data_allocated))) {
        section_realloc(sec, offset1);
    }
    sec->data_offset = offset1;
    if (gt(align, sec->sh_addralign)) {
        sec->sh_addralign = align;
    }
    return offset;
}

/* 10 */
/* reserve at least 'size' bytes in section 'sec' from
   sec->data_offset. */
ST_FUNC void *section_ptr_add(Section *sec, addr_t size) {
    size_t offset = section_add(sec, size, 1);
    return add(sec->data, offset);
}

/* 11 */
/* reserve at least 'size' bytes from section start */
ST_FUNC void section_reserve(Section *sec, int size) {
    if (gt(size, sec->data_allocated)) {
        section_realloc(sec, size);
    }
    if (gt(size, sec->data_offset)) {
        sec->data_offset = size;
    }
}

/* ------------------------------------------------------------------------- */
/* 12 */
ST_FUNC int put_elf_str(Section *s, const char *sym) {
    int offset;
    int len;
    int ptr;

    len = add(strlen(sym), 1);
    offset = s->data_offset;
    ptr = section_ptr_add(s, len);
    memmove(ptr, sym, len);
    return offset;
}

/* 13 */
/* elf symbol hashing function */
static unsigned long elf_hash(const unsigned char *name) {
    int h;
    int g;

    h = 0;

    while (neq(0, ri8(name))) {
        h = add(shl(h, 4), ri8(name));
        name = add(name, 1);
        g = and(h, 0xf0000000);
        if (g) {
            h = xor(h, and(shr(g, 24), 255)); /* ljw see original extra bit
                                                 twiddling for sign */
        }
        h = and(h, not(g));
    }
    return h;
}

/* 14 */
/* rebuild hash table of section s */
/* NOTE: we do factorize the hash table code to go faster */
static void rebuild_hash(Section *s, unsigned int nb_buckets) {
    ElfW(Sym) *sym;
    int *ptr;
    int *hash;
    int nb_syms;
    int sym_index;
    int h;
    unsigned char *strtab;

    strtab = s->link->data;
    nb_syms = div_(s->data_offset, sizeof(ElfW(Sym)));

    if (eq(nb_buckets, 0)) {
        nb_buckets = ((int*)s->hash->data)[0];
    }

    s->hash->data_offset = 0;
    ptr = section_ptr_add(s->hash, mul(add(add(2, nb_buckets), nb_syms),
                                       sizeof(int)));
    wi32(ptr, nb_buckets);
    wi32(add(ptr, 4), nb_syms);
    ptr = add(ptr, 8);
    hash = ptr;
    memset(hash, 0, mul(add(nb_buckets, 1), sizeof(int)));
    ptr = add(ptr, mul(add(nb_buckets, 1), 4));

    sym = (ElfW(Sym) *)s->data + 1;
    sym_index = 1;
    while(lt(sym_index, nb_syms)) {
        if (ELFW(ST_BIND)(sym->st_info) != STB_LOCAL) {
            h = mod(elf_hash(add(strtab, sym->st_name)), nb_buckets);
            wi32(ptr, ri32(add(hash, mul(h, 4))));
            wi32(add(hash, mul(h, 4)), sym_index);
        } else {
            wi32(ptr, 0);
        }
        ptr = add(ptr, 4);
        sym++;
        sym_index = add(sym_index, 1);
    }
}

/* 15 */
/* return the symbol number */
ST_FUNC int put_elf_sym(Section *s, addr_t value, unsigned long size,
    int info, int other, int shndx, const char *name)
{
    int name_offset;
    int sym_index;
    int nbuckets;
    int h;
    ElfW(Sym) *sym;
    Section *hs;
    int *ptr;
    int *base;

    sym = section_ptr_add(s, sizeof(ElfW(Sym)));
    if (name) {
        if(ri8(name)) {
            name_offset = put_elf_str(s->link, name);
        }
    } else {
        name_offset = 0;
    }
    /* XXX: endianness */
    sym->st_name = name_offset;
    sym->st_value = value;
    sym->st_size = size;
    sym->st_info = info;
    sym->st_other = other;
    sym->st_shndx = shndx;
    sym_index = sym - (ElfW(Sym) *)s->data;
    hs = s->hash;
    if (hs) {
        ptr = section_ptr_add(hs, sizeof(int));
        base = (int *)hs->data;
        /* only add global or weak symbols. */
        if (neq(ELFW(ST_BIND)(info), STB_LOCAL)) {
            /* add another hashing entry */
            nbuckets = ri32(base);
            h = mod(elf_hash(add((unsigned char *)s->link->data, name_offset)),
                    nbuckets);
            wi32(ptr, ri32(base,mul(add(2, h), 4)));
            wi32(add(base,mul(add(2, h), 4)), sym_index);
            wi32(add(base, 4), add(ri32(add(base, 4)), 1));
            /* we resize the hash table */
            hs->nb_hashed_syms++;
            if (gt(hs->nb_hashed_syms, mul(2, nbuckets))) {
                rebuild_hash(s, mul(2, nbuckets));
            }
        } else {
            wi32(ptr, 0);
            wi32(add(base, 4),add(ri32(add(base, 4)), 1));
        }
    }
    return sym_index;
}

/* 16 */
ST_FUNC int find_elf_sym(Section *s, const char *name) {
    ElfW(Sym) *sym;
    Section *hs;
    int nbuckets;
    int sym_index;
    int h;
    const char *name1;

    hs = s->hash;
    if (eq(hs, 0)) {
        return 0;
    }
    nbuckets = ((int *)hs->data)[0];
    h = mod(elf_hash((unsigned char *) name), nbuckets);
    sym_index = ((int *)hs->data)[2 + h];
    while (neq(sym_index, 0)) {
        sym = &((ElfW(Sym) *)s->data)[sym_index];
        name1 = add((char *) s->link->data, sym->st_name);
        if (eq(strcmp(name, name1), 0)) {
            return sym_index;
        }
        sym_index = ((int *)hs->data)[add(add(2, nbuckets), sym_index)];
    }
    return 0;
}

/* 17 */
/* add an elf symbol : check if it is already defined and patch
   it. Return symbol index. NOTE that sh_num can be SHN_UNDEF. */
ST_FUNC int set_elf_sym(Section *s, addr_t value, unsigned long size,
                       int info, int other, int shndx, const char *name)
{
    ElfW(Sym) *esym;
    int sym_bind;
    int sym_index;
    int sym_type;
    int esym_bind;
    unsigned char sym_vis;
    unsigned char esym_vis;
    unsigned char new_vis;

    sym_bind = ELFW(ST_BIND)(info);
    sym_type = ELFW(ST_TYPE)(info);
    sym_vis = ELFW(ST_VISIBILITY)(other);

    if (neq(sym_bind, STB_LOCAL)) {
        /* we search global or weak symbols */
        sym_index = find_elf_sym(s, name);
    }
    sym_index = put_elf_sym(s, value, size,
                                ELFW(ST_INFO)(sym_bind, sym_type), other,
                                shndx, name);
    return sym_index;
}

/* 18 */
/* put relocation */
ST_FUNC void put_elf_reloca(Section *symtab, Section *s, unsigned long offset,
                            int type, int symbol, addr_t addend) {
    char *buf;
    int buf_size;
    Section *sr;
    ElfW_Rel *rel;

    buf_size=256;
    buf = tcc_mallocz(buf_size);

    sr = s->reloc;
    if (eq(sr, 0)) {
        /* if no relocation section, create it */
        snprintf(buf, buf_size, REL_SECTION_FMT, s->name);
        /* if the symtab is allocated, then we consider the relocation
           are also */
        sr = new_section(tcc_state, buf, SHT_RELX, symtab->sh_flags);
        sr->sh_entsize = sizeof(ElfW_Rel);
        sr->link = symtab;
        sr->sh_info = s->sh_num;
        s->reloc = sr;
    }
    rel = section_ptr_add(sr, sizeof(ElfW_Rel));
    rel->r_offset = offset;
    rel->r_info = ELFW(R_INFO)(symbol, type);

    tcc_free(buf);
}

/* 19 */
/* In an ELF file symbol table, the local symbols must appear below
   the global and weak ones. Since TCC cannot sort it while generating
   the code, we must do it after. All the relocation tables are also
   modified to take into account the symbol table sorting */
static void sort_syms(TCCState *s1, Section *s) {
    int *old_to_new_syms;
    ElfW(Sym) *new_syms;
    int nb_syms;
    int i;
    ElfW(Sym) *p;
    ElfW(Sym) *q;
    ElfW_Rel *rel;
    Section *sr;
    int type;
    int sym_index;

    nb_syms = div_(s->data_offset, sizeof(ElfW(Sym)));
    new_syms = tcc_malloc(mul(nb_syms, sizeof(ElfW(Sym))));
    old_to_new_syms = tcc_malloc(mul(nb_syms, sizeof(int)));

    /* first pass for local symbols */
    p = (ElfW(Sym) *)s->data;
    q = new_syms;
    i = 0;
    while(lt(i, nb_syms)) {
        if (ELFW(ST_BIND)(p->st_info) == STB_LOCAL) {
            old_to_new_syms[i] = q - new_syms;
            *q++ = *p;
        }
        p++;
        i = add(i, 1);
    }
    /* save the number of local symbols in section header */
    if( s->sh_size )    /* this 'if' makes IDA happy */
        s->sh_info = q - new_syms;

    /* then second pass for non local symbols */
    p = (ElfW(Sym) *)s->data;
    i = 0;
    while(lt(i, nb_syms)) {
        if (neq(ELFW(ST_BIND)(p->st_info), STB_LOCAL)) {
            old_to_new_syms[i] = q - new_syms;
            *q++ = *p;
        }
        p++;
        i = add(i, 1);
    }

    /* we copy the new symbols to the old */
    memcpy(s->data, new_syms, nb_syms * sizeof(ElfW(Sym)));
    tcc_free(new_syms);

    /* now we modify all the relocations */
    i = 1;
    while(lt(i, s1->nb_sections)) {
        sr = s1->sections[i];
        if (and(eq(sr->sh_type, SHT_RELX), eq(sr->link, s))) {
            rel = (ElfW_Rel *) sr->data;
            while(lt(rel, (ElfW_Rel *) (add(sr->data, sr->data_offset)))) {
                sym_index = ELFW(R_SYM)(rel->r_info);
                type = ELFW(R_TYPE)(rel->r_info);
                sym_index = old_to_new_syms[sym_index];
                rel->r_info = ELFW(R_INFO)(sym_index, type);
                rel = rel + 1;
            }
        }
        i = add(i, 1);
    }

    tcc_free(old_to_new_syms);
}

/* 20 */
/* Allocate strings for section names and decide if an unallocated section
   should be output.
   NOTE: the strsec section comes last, so its size is also correct ! */
static int alloc_sec_names(TCCState *s1, Section *strsec) {
    int i;
    Section *s;
    int textrel;

    textrel = 0;
    /* Allocate strings for section names */
    i = 1;
    while(lt(i, s1->nb_sections)) {
        s = s1->sections[i];
        s->sh_size = s->data_offset;
	if (or(s->sh_size, and(s->sh_flags, SHF_ALLOC))) {
            s->sh_name = put_elf_str(strsec, s->name);
        }
        i = add(i, 1);
    }
    strsec->sh_size = strsec->data_offset;
    return textrel;
}

/* 21 */
/* Assign sections to segments and decide how are sections laid out when loaded
   in memory. This function also fills corresponding program headers. */
static int layout_sections(TCCState *s1, ElfW(Phdr) *phdr, int phnum,
                           Section *interp, Section* strsec,
                           struct dyn_inf *dyninf, int *sec_order) {
    int i;
    int sh_order_index;
    int file_offset;
    ElfW(Phdr) *ph;
    Section *s;

    sh_order_index = 1;
    file_offset = 0;
    file_offset = add(sizeof(ElfW(Ehdr)), mul(phnum, sizeof(ElfW(Phdr))));

    /* all other sections come after */
    i = 1;
    while(lt(i, s1->nb_sections)) {
        s = s1->sections[i];
        wi32(add(sec_order, mul(sh_order_index, 4)), i);
        sh_order_index = add(sh_order_index, 1);

        file_offset = and((add(file_offset, sub(s->sh_addralign, 1))),
                          not(sub(s->sh_addralign, 1)));
        s->sh_offset = file_offset;
        if (neq(s->sh_type, SHT_NOBITS)) {
            file_offset = add(file_offset, s->sh_size);
        }
        i = add(i, 1);
    }

    return file_offset;
}

/* 22 */
/* Create an ELF file on disk.
   This function handle ELF specific layout requirements */
static void tcc_output_elf(TCCState *s1, FILE *f, int phnum, ElfW(Phdr) *phdr,
                           int file_offset, int *sec_order) {
    int i;
    int shnum;
    int offset;
    int size;
    Section *s;
    ElfW(Ehdr) ehdr;
    ElfW(Shdr) shdr, *sh;

    shnum = s1->nb_sections;

    memset(&ehdr, 0, sizeof(ehdr));

    /* align to 4 */
    file_offset = and(add(file_offset, 3), sub(0, 4));

    /* fill header */
    ehdr.e_ident[0] = ELFMAG0;
    ehdr.e_ident[1] = mkc('E');
    ehdr.e_ident[2] = mkc('L');
    ehdr.e_ident[3] = mkc('F');
    ehdr.e_ident[4] = ELFCLASSW;
    ehdr.e_ident[5] = ELFDATA2LSB;
    ehdr.e_ident[6] = EV_CURRENT;

    ehdr.e_type = ET_REL;
    ehdr.e_machine = EM_TCC_TARGET;
    ehdr.e_version = EV_CURRENT;
    ehdr.e_shoff = file_offset;
    ehdr.e_ehsize = sizeof(ElfW(Ehdr));
    ehdr.e_shentsize = sizeof(ElfW(Shdr));
    ehdr.e_shnum = shnum;
    ehdr.e_shstrndx = sub(shnum, 1);

    fwrite(&ehdr, 1, sizeof(ElfW(Ehdr)), f);
    fwrite(phdr, 1, mul(phnum, sizeof(ElfW(Phdr))), f);
    offset = add(sizeof(ElfW(Ehdr)), mul(phnum, sizeof(ElfW(Phdr))));

    sort_syms(s1, symtab_section);
    i = 1;
    while(lt(i, s1->nb_sections)) {
        s = s1->sections[sec_order[i]];
        if (neq(s->sh_type, SHT_NOBITS)) {
            while (lt(offset, s->sh_offset)) {
                fputc(0, f);
                offset = add(offset, 1);
            }
            size = s->sh_size;
            if (size) {
                fwrite(s->data, 1, size, f);
            }
            offset = add(offset, size);
        }
        i = add(i, 1);
    }

    /* output section headers */
    while (lt(offset, ehdr.e_shoff)) {
        fputc(0, f);
        offset++;
    }

    i = 0;
    while(lt(i, s1->nb_sections)) {
        sh = &shdr;
        memset(sh, 0, sizeof(ElfW(Shdr)));
        s = s1->sections[i];
        if (s) {
            sh->sh_name = s->sh_name;
            sh->sh_type = s->sh_type;
            sh->sh_flags = s->sh_flags;
            sh->sh_entsize = s->sh_entsize;
            sh->sh_info = s->sh_info;
            if (s->link)
                sh->sh_link = s->link->sh_num;
            sh->sh_addralign = s->sh_addralign;
            sh->sh_addr = s->sh_addr;
            sh->sh_offset = s->sh_offset;
            sh->sh_size = s->sh_size;
        }
        fwrite(sh, 1, sizeof(ElfW(Shdr)), f);
        i = add(i, 1);
    }
}

/* 23 */
/* Write an elf file */
static int tcc_write_elf_file(TCCState *s1, const char *filename, int phnum,
                              ElfW(Phdr) *phdr, int file_offset, int *sec_order)
{
    int fd, mode;
    FILE *f;

    unlink(filename);

    f = fopen(filename, mks("wb"));
    tcc_output_elf(s1, f, phnum, phdr, file_offset, sec_order);
    fclose(f);

    return 0;
}

/* 24 */
/* Output an elf, coff or binary file */
/* XXX: suppress unneeded sections */
static int elf_output_file(TCCState *s1, const char *filename) {
    int i;
    int ret;
    int phnum;
    int shnum;
    int file_offset;
    int *sec_order;
    ElfW(Phdr) *phdr;
    ElfW(Sym) *sym;
    Section *strsec;
    Section *interp;
    int textrel;

    ret = sub(0, 1);
    phdr = NULL;
    sec_order = NULL;
    interp = 0; /* avoid warning */
    textrel = 0;

    /* we add a section for symbols */
    strsec = new_section(s1, mks(".shstrtab"), SHT_STRTAB, 0);
    put_elf_str(strsec, mks(""));

    /* Allocate strings for section names */
    textrel = alloc_sec_names(s1, strsec);

    /* compute number of program headers */
    phnum = 0;

    /* allocate program segment headers */
    phdr = tcc_mallocz(mul(phnum, sizeof(ElfW(Phdr))));

    /* compute number of sections */
    shnum = s1->nb_sections;

    /* this array is used to reorder sections in the output file */
    sec_order = tcc_malloc(mul(sizeof(int), shnum));
    sec_order[0] = 0;

    /* compute section to program header mapping */
    file_offset = layout_sections(s1, phdr, phnum, interp, strsec, 0,
                                  sec_order);

    /* Create the ELF file with name 'filename' */
    ret = tcc_write_elf_file(s1, filename, phnum, phdr, file_offset, sec_order);
    s1->nb_sections = shnum;
    tcc_free(sec_order);
    tcc_free(phdr);
    return ret;
}

/* 25 */
LIBTCCAPI int tcc_output_file(TCCState *s, const char *filename) {
    int ret;
    ret = elf_output_file(s, filename);
    return ret;
}
