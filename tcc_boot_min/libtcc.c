/*
 *  TCC - Tiny C Compiler
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

#include "tccpp.c"
#include "tccgen.c"
#include "tccelf.c"

int init_globals() {
    aglobal_stack = &global_stack;
    alocal_stack = &local_stack;
}

typedef struct TCCOption {
    const char *name;
    uint16_t index;
    uint16_t flags;
} TCCOption;

enum {
    TCC_OPTION_c,
    TCC_OPTION_o,
    TCC_OPTION_nostdinc,
};

#define TCC_OPTION_HAS_ARG 0x0001
#define TCC_OPTION_NOSEP   0x0002 /* cannot have space before option and arg */

static const TCCOption tcc_options[] = {
    { "c", TCC_OPTION_c, 0 },
    { "o", TCC_OPTION_o, TCC_OPTION_HAS_ARG },
    { "nostdinc", TCC_OPTION_nostdinc, 1 },
    { NULL, 0, 0 },
};

/* 9 */
void args_parser_add_file(TCCState *s, const char* filename, int filetype) {
    struct filespec *f;
    f = tcc_malloc(add(sizeof_filespec, strlen(filename)));
    f->type = filetype;
    strcpy(f->name, filename);
    dynarray_add(ats_files(s), ats_nb_files(s), f);
}

/* 10 */
PUB_FUNC int tcc_parse_args(TCCState *s, int *pargc, char ***pargv, int optind)
{
    const TCCOption *popt;
    char *optarg;
    char *r;
    char **argv = *pargv;
    int argc = *pargc;


    while (lt(optind, argc)) {
        r = ri32(add(argv, mul(optind, 4)));
        optind = add(optind, 1);
        if (or(neq(ri8(r), mkc('-')), eq(ri8(add(r, 1)), 0))) {
            args_parser_add_file(s, r, s->filetype);
        } else {

            /* find option in table */
            for(popt = tcc_options; ; ) {
                const char *p1 = popt->name;
                const char *r1 = r + 1;
                if (strstart(p1, &r1)) {
                    optarg = r1;
                    if (popt->flags & TCC_OPTION_HAS_ARG) {
                        if (*r1 == '\0' && !(popt->flags & TCC_OPTION_NOSEP)) {
                            optarg = argv[optind++];
                        }
                    }
                    break;
                }
                popt = popt + 1;
            }

            if(popt->index == TCC_OPTION_o) {
                s->outfile = tcc_strdup(optarg);
            }
        }
    }

    return 0;
}
