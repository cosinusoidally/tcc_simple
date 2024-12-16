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
    Elf32_Rel *rel;
    Elf32_Rel *rel_end;

    s = gts_symtab(s1);

    first_sym = div_(gs_sh_offset(s), sizeof_Elf32_Sym);
    nb_syms = sub(div_(gs_data_offset(s), sizeof_Elf32_Sym), first_sym);
    ss_data_offset(s, gs_sh_offset(s));
    s->link->data_offset = gs_sh_offset(gs_link(s));
    ss_hash(s, gs_reloc(s));
    ss_reloc(s, 0);
    tr = tcc_mallocz(mul(nb_syms, sizeof *tr));

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

/* 10 */
/* reserve at least 'size' bytes in section 'sec' from
   sec->data_offset. */
ST_FUNC void *section_ptr_add(Section *sec, addr_t size) {
    return add(sec->data, section_add(sec, size, 1));
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

/* 22 */
/* Create an ELF file on disk.
   This function handle ELF specific layout requirements */
void tcc_output_elf(TCCState *s1, FILE *f, int phnum, Elf32_Phdr *phdr,
                           int file_offset, int *sec_order) {
    int i;
    int shnum;
    int offset;
    int size;
    Section *s;
    Elf32_Ehdr ehdr;
    Elf32_Shdr shdr;
    Elf32_Shdr *sh;

    shnum = gts_nb_sections(s1);

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
    ehdr.e_ehsize = sizeof_Elf32_Ehdr;
    ehdr.e_shentsize = sizeof(Elf32_Shdr);
    ehdr.e_shnum = shnum;
    ehdr.e_shstrndx = sub(shnum, 1);

    fwrite(&ehdr, 1, sizeof_Elf32_Ehdr, f);
    fwrite(phdr, 1, mul(phnum, sizeof_Elf32_Phdr), f);
    offset = add(sizeof_Elf32_Ehdr, mul(phnum, sizeof_Elf32_Phdr));

    sort_syms(s1, symtab_section);
    i = 1;
    while(lt(i, gts_nb_sections(s1))) {
        s = s1->sections[sec_order[i]];
        if (neq(gs_sh_type(s), SHT_NOBITS)) {
            while (lt(offset, gs_sh_offset(s))) {
                fputc(0, f);
                offset = add(offset, 1);
            }
            size = gs_sh_size(s);
            if (size) {
                fwrite(gs_data(s), 1, size, f);
            }
            offset = add(offset, size);
        }
        i = add(i, 1);
    }

    /* output section headers */
    while (lt(offset, ehdr.e_shoff)) {
        fputc(0, f);
        offset = add(offset, 1);
    }

    i = 0;
    while(lt(i, gts_nb_sections(s1))) {
        sh = &shdr;
        memset(sh, 0, sizeof(Elf32_Shdr));
        s = s1->sections[i];
        if (s) {
            sh->sh_name = gs_sh_name(s);
            sh->sh_type = gs_sh_type(s);
            sh->sh_flags = gs_sh_flags(s);
            sh->sh_entsize = s->sh_entsize;
            sh->sh_info = s->sh_info;
            if (s->link) {
                sh->sh_link = s->link->sh_num;
            }
            sh->sh_addralign = gs_sh_addralign(s);
            sh->sh_addr = s->sh_addr;
            sh->sh_offset = gs_sh_offset(s);
            sh->sh_size = gs_sh_size(s);
        }
        fwrite(sh, 1, sizeof(Elf32_Shdr), f);
        i = add(i, 1);
    }
}
