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
void sort_syms(TCCState *s1, Section *s) {
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
            wi32(add(old_to_new_syms, mul(i, 4)), q - new_syms);
            *q = *p;
            q = add(q, sizeof_Elf32_Sym);
        }
        p = add(p, sizeof_Elf32_Sym);
        i = add(i, 1);
    }
    /* save the number of local symbols in section header */
    if(gs_sh_size(s)) {   /* this 'if' makes IDA happy */
        ss_sh_info(s, q - new_syms);
    }

    /* then second pass for non local symbols */
    p = gs_data(s);
    i = 0;
    while(lt(i, nb_syms)) {
        if (neq(ELFW_ST_BIND(ges_st_info(p)), STB_LOCAL)) {
            wi32(add(old_to_new_syms, mul(i, 4)), q - new_syms);
            *q = *p;
            q = add(q, sizeof_Elf32_Sym);
        }
        p = add(p, sizeof_Elf32_Sym);
        i = add(i, 1);
    }

    /* we copy the new symbols to the old */
    memcpy(gs_data(s), new_syms, mul(nb_syms, sizeof_Elf32_Sym));
    tcc_free(new_syms);

    /* now we modify all the relocations */
    i = 1;
    while(lt(i, gts_nb_sections(s1))) {
        sr = ri32(add(gts_sections(s1), mul(i, 4)));
        if (and(eq(gs_sh_type(sr), SHT_RELX), eq(gs_link(sr), s))) {
            rel = gs_data(sr);
            while(lt(rel, add(gs_data(sr), gs_data_offset(sr)))) {
                sym_index = ELFW_R_SYM(ger_r_info(rel));
                type = ELFW_R_TYPE(ger_r_info(rel));
                sym_index = ri32(add(old_to_new_syms, mul(sym_index, 4)));
                ser_r_info(rel, ELFW_R_INFO(sym_index, type));
                rel = add(rel, sizeof_Elf32_Rel);
            }
        }
        i = add(i, 1);
    }

    tcc_free(old_to_new_syms);
}
