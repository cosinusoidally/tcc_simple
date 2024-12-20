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
#include "tccgen.c"

static TokenSym *hash_ident[TOK_HASH_SIZE];
static char token_buf[STRING_MAX_SIZE + 1];
static CString cstr_buf;
static unsigned char isidnum_table[256 - CH_EOF];

extern TokenString *macro_stack;

/* LJW HACK addreses of these variables */
/* actually one or two are not addreses */
extern int aTOK_HASH_SIZE;
extern int ahash_ident;
extern int acstr_buf;
extern int CH_EOF_;
extern int aisidnum_table;
extern int atokc;
extern int atokcstr;
extern int atoken_buf;
extern int avstack;

int init_tccpp_globals(){
  aTOK_HASH_SIZE = TOK_HASH_SIZE;
  ahash_ident = hash_ident;
  acstr_buf = &cstr_buf;
  CH_EOF_ = CH_EOF;
  aisidnum_table = isidnum_table;
  atokc = &tokc;
  atokcstr = &tokcstr;
  atoken_buf = &token_buf;
  avstack = vstack;
  aSTRING_MAX_SIZE = STRING_MAX_SIZE;
}

int init_globals() {
    aglobal_stack = &global_stack;
    alocal_stack = &local_stack;
}
