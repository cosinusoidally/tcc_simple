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

/* 19 */
/* In an ELF file symbol table, the local symbols must appear below
   the global and weak ones. Since TCC cannot sort it while generating
   the code, we must do it after. All the relocation tables are also
   modified to take into account the symbol table sorting */
static void sort_syms(TCCState *s1, Section *s) {
    int *old_to_new_syms;
    Elf32_Sym *new_syms;
    int nb_syms;
    int i;
    Elf32_Sym *p;
    Elf32_Sym *q;
    Elf32_Rel *rel;
    Section *sr;
    int type;
    int sym_index;

    nb_syms = div_(gs_data_offset(s), sizeof_Elf32_Sym);
    new_syms = tcc_malloc(mul(nb_syms, sizeof_Elf32_Sym));
    old_to_new_syms = tcc_malloc(mul(nb_syms, 4));

    /* first pass for local symbols */
    p = gs_data(s);
    q = new_syms;
    i = 0;
    while(lt(i, nb_syms)) {
        if (eq(ELFW_ST_BIND(ges_st_info(p)), STB_LOCAL)) {
            old_to_new_syms[i] = q - new_syms;
            *q++ = *p;
        }
        p++;
        i = add(i, 1);
    }
    /* save the number of local symbols in section header */
    if(gs_sh_size(s)) {   /* this 'if' makes IDA happy */
        s->sh_info = q - new_syms;
    }

    /* then second pass for non local symbols */
    p = gs_data(s);
    i = 0;
    while(lt(i, nb_syms)) {
        if (neq(ELFW_ST_BIND(ges_st_info(p)), STB_LOCAL)) {
            old_to_new_syms[i] = q - new_syms;
            *q++ = *p;
        }
        p++;
        i = add(i, 1);
    }

    /* we copy the new symbols to the old */
    memcpy(gs_data(s), new_syms, mul(nb_syms, sizeof_Elf32_Sym));
    tcc_free(new_syms);

    /* now we modify all the relocations */
    i = 1;
    while(lt(i, s1->nb_sections)) {
        sr = s1->sections[i];
        if (and(eq(gs_sh_type(sr), SHT_RELX), eq(sr->link, s))) {
            rel = gs_data(sr);
            while(lt(rel, add(sr->data, sr->data_offset))) {
                sym_index = ELFW_R_SYM(rel->r_info);
                type = ELFW_R_TYPE(rel->r_info);
                sym_index = old_to_new_syms[sym_index];
                rel->r_info = ELFW_R_INFO(sym_index, type);
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
    int aehdr;
    Elf32_Ehdr *ehdr;
    int shdr;
    Elf32_Shdr *sh;

    enter();
    aehdr = v_alloca(sizeof_Elf32_Ehdr);
    ehdr = aehdr;
    shdr = v_alloca(sizeof_Elf32_Shdr);

    shnum = gts_nb_sections(s1);

    memset(ehdr, 0, sizeof_Elf32_Ehdr);

    /* align to 4 */
    file_offset = and(add(file_offset, 3), sub(0, 4));

    /* fill header */
    wi8(gee_e_ident(ehdr), ELFMAG0);
    wi8(add(gee_e_ident(ehdr), 1), mkc('E'));
    wi8(add(gee_e_ident(ehdr), 2), mkc('L'));
    wi8(add(gee_e_ident(ehdr), 3), mkc('F'));
    wi8(add(gee_e_ident(ehdr), 4), ELFCLASSW);
    wi8(add(gee_e_ident(ehdr), 5), ELFDATA2LSB);
    wi8(add(gee_e_ident(ehdr), 6), EV_CURRENT);

    see_e_type(ehdr, ET_REL);
    see_e_machine(ehdr, EM_TCC_TARGET);
    see_e_version(ehdr, EV_CURRENT);
    see_e_shoff(ehdr, file_offset);
    see_e_ehsize(ehdr, sizeof_Elf32_Ehdr);
    see_e_shentsize(ehdr, sizeof_Elf32_Shdr);
    see_e_shnum(ehdr, shnum);
    see_e_shstrndx(ehdr, sub(shnum, 1));

    fwrite(ehdr, 1, sizeof_Elf32_Ehdr, f);
    fwrite(phdr, 1, mul(phnum, sizeof_Elf32_Phdr), f);
    offset = add(sizeof_Elf32_Ehdr, mul(phnum, sizeof_Elf32_Phdr));

    sort_syms(s1, symtab_section);
    i = 1;
    while(lt(i, gts_nb_sections(s1))) {
        s = s1->sections[ri32(add(sec_order, mul(i, 4)))];
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
    while (lt(offset, ehdr->e_shoff)) {
        fputc(0, f);
        offset = add(offset, 1);
    }

    i = 0;
    while(lt(i, gts_nb_sections(s1))) {
        sh = shdr;
        memset(sh, 0, sizeof_Elf32_Shdr);
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

    leave(0);
}
