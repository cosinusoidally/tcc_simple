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
void next_nomacro1(void)
{
    int t, c, is_long, len;
    TokenSym *ts;
    uint8_t *p, *p1;
    unsigned int h;
    int redo_no_start;

    p = file->buf_ptr;
    while(1) {
        redo_no_start = 0;
        c = *p;
        if((c == ' ') || (c == '\t')) {
            tok = c;
            p++;
            while (isidnum_table[*p - CH_EOF] & IS_SPC)
                ++p;
            redo_no_start = 1;
        } else if(c == '\\'){
            /* first look if it is in fact an end of buffer */
            c = handle_stray1(p);
            p = file->buf_ptr;
            if (c != CH_EOF) {
                redo_no_start = 1;
            } else {
                tok = TOK_EOF;
            }
        } else if(c == '\n'){
            file->line_num++;
            tok_flags |= TOK_FLAG_BOL;
            p++;
            if (0 == (parse_flags & PARSE_FLAG_LINEFEED)) {
                redo_no_start = 1;
            }
        }
        if(redo_no_start == 0) {
            break;
        }
    }

    c = *p;
    if(isid(c)) {
        p1 = p;
        h = 1; /* TOK_HASH_INIT */
        h = TOK_HASH_FUNC(h, c);
        while (c = *++p, isidnum_table[c - CH_EOF] & (IS_ID|IS_NUM))
            h = TOK_HASH_FUNC(h, c);
        len = p - p1;
        if (c != '\\') {
            TokenSym **pts;

            /* fast case : no stray found, so we have the full token
               and we have already hashed it */
            h &= (TOK_HASH_SIZE - 1);
            pts = &hash_ident[h];
            int token_found;
            while(1) {
                token_found = 0;
                ts = *pts;
                if (!ts)
                    break;
                if (ts->len == len && !memcmp(ts->str, p1, len)) {
                    token_found = 1;
                    break;
                }
                pts = &(ts->hash_next);
            }
            if(token_found == 0) {
                ts = tok_alloc_new(pts, (char *) p1, len);
            }
        } else {
            /* slower case */
            cstr_reset(&tokcstr);
            cstr_cat(&tokcstr, (char *) p1, len);
            p--;
            PEEKC(&c, &p);
            while (isidnum_table[c - CH_EOF] & (IS_ID|IS_NUM))
            {
                cstr_ccat(&tokcstr, c);
                PEEKC(&c, &p);
            }
            ts = tok_alloc(tokcstr.data, tokcstr.size);
        }
        tok = ts->tok;
    } else if(isnum(c)){
        t = c;
        PEEKC(&c, &p);
        /* after the first digit, accept digits, alpha, '.' or sign if
           prefixed by 'eEpP' */
        cstr_reset(&tokcstr);
        while(1) {
            cstr_ccat(&tokcstr, t);
            if (!((isidnum_table[c - CH_EOF] & (IS_ID|IS_NUM))
                  || c == '.'
                  || (c == '+' || c == '-'))) {
                break;
            }
            t = c;
            PEEKC(&c, &p);
        }
        /* We add a trailing '\0' to ease parsing */
        cstr_ccat(&tokcstr, '\0');
        tokc.str.size = tokcstr.size;
        tokc.str.data = tokcstr.data;
        tok = TOK_PPNUM;
    } else if((c == '\'') || (c == '\"')){
        is_long = 0;
        cstr_reset(&tokcstr);
        cstr_ccat(&tokcstr, c);
        p = parse_pp_string(p, c, &tokcstr);
        cstr_ccat(&tokcstr, c);
        cstr_ccat(&tokcstr, '\0');
        tokc.str.size = tokcstr.size;
        tokc.str.data = tokcstr.data;
        tok = TOK_PPSTR;
    } else if(c == '='){
        PEEKC(&c, &p);
        tok = '=';
    } else if(c == '/'){
        PEEKC(&c, &p);
        if (c == '*') {
            p = parse_comment(p);
            /* comments replaced by a blank */
            tok = ' ';
            file->buf_ptr = p;
            return;
        }
    } else if((c == '(') || (c == ')') ||
              (c == '{') || (c == '}') ||
              (c == ',') || (c == ';')){
        tok = c;
        p++;
    }
    tok_flags = 0;
    file->buf_ptr = p;
}
