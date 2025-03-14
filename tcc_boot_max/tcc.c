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

#include "tcc.h"
#include "libtcc.c"

static const char help[] =
    "Tiny C Compiler "TCC_VERSION" - Copyright (C) 2001-2006 Fabrice Bellard\n"
    ;

static const char help2[] =
    "Tiny C Compiler "TCC_VERSION" - More Options\n"
    ;

static const char version[] =
    "tcc version "TCC_VERSION" ("
        "i386"
        " Linux"
    ")\n"
    ;

/* this is tcc_boot_max support code. It is used to allow us to take addresses
   of global variables that are delared in the code that has not yet been
   ported to the simplified dialect of C. */
int init_globals() {
  puts("init_globals");
  /* stub atm , will eventually include stuff like:
     aglobal_stack = &global_stack;
     which is useful support code while we incrementally port to the simple c
     dialect.
  */
}

int main(int argc0, char **argv0)
{
    TCCState *s;
    int ret, opt, n = 0, t = 0;
    unsigned start_time = 0;
    const char *first_file;
    int argc; char **argv;
    FILE *ppfp = stdout;

    argc = argc0, argv = argv0;

    init_runtime();

    s = tcc_new();
    opt = tcc_parse_args(s, &argc, &argv, 1);

    n = s->nb_files;

    tcc_set_output_type(s, s->output_type);
    s->ppfp = ppfp;

    /* compile or add each files or library */
    for (first_file = NULL, ret = 0;;) {
        struct filespec *f = s->files[s->nb_files - n];
        s->filetype = f->type;
        s->alacarte_link = f->alacarte;
        if (!first_file) {
            first_file = f->name;
        }
        tcc_add_file(s, f->name);
        s->filetype = 0;
        s->alacarte_link = 1;
        if (--n == 0) {
            break;
        }
    }

    tcc_output_file(s, s->outfile);

    tcc_delete(s);
    return ret;
}
