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

    sr = gs_reloc(s);
    if (eq(sr, 0)) {
        /* if no relocation section, create it */
        snprintf(buf, buf_size, REL_SECTION_FMT, gs_name(s));
        /* if the symtab is allocated, then we consider the relocation
           are also */
        sr = new_section(tcc_state, buf, SHT_RELX, gs_sh_flags(symtab));
        ss_sh_entsize(sr, sizeof_Elf32_Rel);
        ss_link(sr, symtab);
        sr->sh_info = s->sh_num;
        ss_reloc(s, sr);
    }
    rel = section_ptr_add(sr, sizeof_Elf32_Rel);
    rel->r_offset = offset;
    rel->r_info = ELFW_R_INFO(symbol, type);

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
