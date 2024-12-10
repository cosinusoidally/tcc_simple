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

int main_(int argc0, char **argv0)
{
    TCCState *s;
    int s_;
    int argc;
    int argv;
    struct filespec *f;
    int f_;

    init_runtime();

    enter();
    argc = v_alloca(4);
    argv = v_alloca(4);

    wi32(argc, argc0);
    wi32(argv, argv0);
    s_ = tcc_new();
    s = s_;
    tcc_parse_args(s, argc, argv, 1);

    f_ = ri32(s->files);
    f = f_;
    sts_filetype(s_,gfs_type(f_));
//    s->filetype = f->type;
    tcc_add_file(s, gfs_name(f_));
//    tcc_add_file(s, f->name);
    sts_filetype(s_, 0);
//    s->filetype = 0;

    tcc_output_file(s_, gts_outfile(s_));

    tcc_delete(s_);
    return leave(0);
}
