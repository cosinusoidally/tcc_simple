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
    char *name;
    int index;
    int flags;
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

/* 10 */
PUB_FUNC int tcc_parse_args(TCCState *s, int *pargc, char ***pargv, int optind)
{
    const TCCOption *popt;
    char *optarg;
    char *r;
    char *p1;
    char *r1;
    char **argv = *pargv;
    int argc = *pargc;

    while (lt(optind, argc)) {
        r = ri32(add(argv, mul(optind, 4)));
        optind = add(optind, 1);
        if (or(neq(ri8(r), mkc('-')), eq(ri8(add(r, 1)), 0))) {
            args_parser_add_file(s, r, gts_filetype(s));
        } else {
            /* find option in table */
            popt = tcc_options;
            while(1) {
                p1 = popt->name;
                r1 = add(r, 1);
                if (strstart(p1, &r1)) {
                    optarg = r1;
                    if (and(popt->flags, TCC_OPTION_HAS_ARG)) {
                        if ((*r1 == 0) && !(popt->flags & TCC_OPTION_NOSEP)) {
                            optarg = argv[optind++];
                        }
                    }
                    break;
                }
                popt = popt + 1;
            }

            if(eq(popt->index, TCC_OPTION_o)) {
                sts_outfile(s, tcc_strdup(optarg));
            }
        }
    }

    return 0;
}
