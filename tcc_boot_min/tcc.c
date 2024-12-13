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

extern int TCC_OPTION_c;
extern int TCC_OPTION_o;
extern int TCC_OPTION_nostdinc;

extern int TCC_OPTION_HAS_ARG; /* 0x0001 */
extern int TCC_OPTION_NOSEP;   /* 0x0002 cannot have space before option and arg */

extern TCCOption *tcc_options;
