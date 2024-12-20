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

/* ------------------------------------------------------------------------- */

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

/* 33 */
/* return next token without macro substitution */
int next_nomacro1() {
    int t;
    int c;
    int len;
    int ts;
    int p;
    int p1;
    int h;
    int redo_no_start;
    int pts;

    int tc;
    int tp;

    enter();
    tc = v_alloca(4);
    tp = v_alloca(4);

    p = gbf_buf_ptr(file);
    while(1) {
        redo_no_start = 0;
        c = ri8(p);
        if(or(eq(c, mkc(' ')), (eq(c, mkc('\t'))))) {
            tok = c;
            p = add(p, 1);
            while (and(ri8(add(aisidnum_table, sub(ri8(p), CH_EOF_))), IS_SPC)) {
                p = add(p, 1);
            }
            redo_no_start = 1;
        } else if(eq(c, mkc('\\'))){
            /* first look if it is in fact an end of buffer */
            c = handle_stray1(p);
            p = gbf_buf_ptr(file);
            if (neq(c, CH_EOF_)) {
                redo_no_start = 1;
            } else {
                tok = TOK_EOF;
            }
        } else if(eq(c, mkc('\n'))){
            sbf_line_num(file, add(gbf_line_num(file), 1));
            tok_flags = or(tok_flags, TOK_FLAG_BOL);
            p = add(p, 1);
            if (eq(0, and(parse_flags, PARSE_FLAG_LINEFEED))) {
                redo_no_start = 1;
            }
        }
        if(eq(redo_no_start, 0)) {
            break;
        }
    }

    c = ri8(p);
    if(isid(c)) {
        p1 = p;
        h = 1; /* TOK_HASH_INIT */
        h = TOK_HASH_FUNC(h, c);
        while (1) {
            p = add(p, 1);
            c = ri8(p);
            if(eq(p, 0)) {
                break;
            } else {
                if(eq(0, and(ri8(add(aisidnum_table, sub(c, CH_EOF_))), or(IS_ID,IS_NUM)))) {
                    break;
                }
            }
            h = TOK_HASH_FUNC(h, c);
        }
        len = sub(p, p1);
        if (neq(c, mkc('\\'))) {

            /* fast case : no stray found, so we have the full token
               and we have already hashed it */
            h = and(h, sub(aTOK_HASH_SIZE, 1));
            pts = add(ahash_ident, mul(h, 4));
            int token_found;
            while(1) {
                token_found = 0;
                ts = ri32(pts);
                if (eq(ts, 0)) {
                    break;
                }
                if (eq(gtks_len(ts), len)) {
                    if(eq(0, memcmp(gtks_str(ts), p1, len))) {
                        token_found = 1;
                        break;
                    }
                }
                pts = atks_hash_next(ts);
            }
            if(eq(token_found, 0)) {
                ts = tok_alloc_new(pts, p1, len);
            }
        } else {
            /* slower case */
            cstr_reset(atokcstr);
            cstr_cat(atokcstr, p1, len);
            p = sub(p, 1);
            wi8(tc, c); wi32(tp, p); /* LJW hack to avoid using & */
            PEEKC(tc, tp);
            c = ri32(tc); p = ri32(tp);
            while (and(ri8(add(aisidnum_table, sub(c, CH_EOF))), or(IS_ID, IS_NUM))) {
                cstr_ccat(&tokcstr, c);
                wi8(tc, c); wi32(tp, p); /* LJW hack to avoid using & */
                PEEKC(tc, tp);
                c = ri32(tc); p = ri32(tp);
            }
            ts = tok_alloc(gcs_data(atokcstr), gcs_size(atokcstr));
        }
        tok = gtks_tok(ts);
    } else if(isnum(c)){
        t = c;
        PEEKC(&c, &p);
        /* after the first digit, accept digits, alpha, '.' or sign if
           prefixed by 'eEpP' */
        cstr_reset(atokcstr);
        while(1) {
            cstr_ccat(atokcstr, t);
            if (eq(0, or(and(ri8(add(aisidnum_table, sub(c, CH_EOF_))), or(IS_ID, IS_NUM)),
                  or(eq(c, mkc('.')),
                  or(eq(c, mkc('+')), eq(c, mkc('-'))))))) {
                break;
            }
            t = c;
            PEEKC(&c, &p);
        }
        /* We add a trailing '\0' to ease parsing */
        cstr_ccat(atokcstr, 0);
        scv_str_size(atokc, gcs_size(atokcstr));
        scv_str_data(atokc, gcs_data(atokcstr));
        tok = TOK_PPNUM;
    } else if((c == '\'') || (c == '\"')){
        cstr_reset(atokcstr);
        cstr_ccat(atokcstr, c);
        p = parse_pp_string(p, c, atokcstr);
        cstr_ccat(atokcstr, c);
        cstr_ccat(atokcstr, 0);
        scv_str_size(atokc, gcs_size(atokcstr));
        scv_str_data(atokc, gcs_data(atokcstr));
        tok = TOK_PPSTR;
    } else if(eq(c, mkc('='))){
        PEEKC(&c, &p);
        tok = mkc('=');
    } else if(eq(c, mkc('/'))){
        PEEKC(&c, &p);
        if (eq(c, mkc('*'))) {
            p = parse_comment(p);
            /* comments replaced by a blank */
            tok = mkc(' ');
            sbf_buf_ptr(file, p);
            return leave(0);
        }
    } else if(or(or(eq(c, mkc('(')), eq(c, mkc(')'))),
              or(or(eq(c, mkc('{')), eq(c, mkc('}'))),
              or(eq(c, mkc(',')), eq(c, mkc(';')))))){
        tok = c;
        p = add(p, 1);
    }
    tok_flags = 0;
    sbf_buf_ptr(file, p);

    return leave(0);
}
