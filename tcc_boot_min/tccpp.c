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
/* global variables */

extern struct BufferedFile *file;
ST_DATA int ch, tok;
ST_DATA CString tokcstr; /* current parsed string, if any */

/* display benchmark infos */
ST_DATA int total_lines;
extern int tok_ident;
extern TokenSym **table_ident;

/* ------------------------------------------------------------------------- */

static TokenSym *hash_ident[TOK_HASH_SIZE];
static char token_buf[STRING_MAX_SIZE + 1];
static CString cstr_buf;
static TokenString tokstr_buf;
static unsigned char isidnum_table[256 - CH_EOF];
static void tok_print(const char *msg, const int *str);

static struct TinyAlloc *toksym_alloc;
static struct TinyAlloc *tokstr_alloc;
static struct TinyAlloc *cstr_alloc;

extern TokenString *macro_stack;

static void next_nomacro_spc(void);

/* addreses of these variables */
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
}

/* 29 */
/* get a token from an integer array and increment pointer
   accordingly. we code it as a macro to avoid pointer aliasing. */
void TOK_GET(int *t, const int **pp, CValue *cv) {
    int n;
    int p;
    p = ri32(pp);

    wi32(t, ri32(p));
    p = add(p, 4);
    if(or(or(eq(t, TOK_STR), eq(t, TOK_PPNUM))), eq(t, TOK_PPSTR)) {
        scv_str_size(cv, ri32(p));
        p = add(p, 4);
        wi32(gcv_str_data(cv), p);
        p = add(p, mul(div_(sub(add(gcv_str_size(cv), sizeof_int), 1), sizeof_int), 4));
    }

    wi32(pp, p);
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

/* 37 */
/* push back current token and set current token to 'last_tok'. Only
   identifier case handled for labels. */
ST_INLN void unget_tok(int last_tok)
{

    TokenString *str = tok_str_alloc();
    tok_str_add2(str, tok, &tokc);
    tok_str_add(str, 0);
    begin_macro(str, 1);
    tok = last_tok;
}

/* 38 */
ST_FUNC void preprocess_start(TCCState *s1, int is_asm) {
    int acstr;
    int i;

    enter();
    acstr = v_alloca(sizeof_CString);

    vtop = sub(vstack, sizeof_SValue);

    set_idnum(mkc('$'), 0);
    set_idnum(mkc('.'), 0);

    cstr_new(acstr);
    cstr_cat(acstr, mks("\""), sub(0,1));
    cstr_cat(acstr, gbf_filename(file), sub(0,1));
    cstr_cat(acstr, mks("\""), 0);

    cstr_reset(acstr);
    cstr_free(acstr);

    parse_flags = 0;
    tok_flags = or(TOK_FLAG_BOL, TOK_FLAG_BOF);

    leave(0);
}

/* 39 */
void tccpp_new(TCCState *s)
{
    int i, c;
    const char *p, *r;
    int tmp;

    /* might be used in error() before preprocess_start() */

    /* init isid table */
    for(i = CH_EOF; i<128; i++)
        set_idnum(i,
            is_space(i) ? IS_SPC
            : isid(i) ? IS_ID
            : isnum(i) ? IS_NUM
            : 0);

    for(i = 128; i<256; i++)
        set_idnum(i, IS_ID);

    memset(hash_ident, 0, TOK_HASH_SIZE * sizeof(TokenSym *));
    cstr_new(&cstr_buf);
    cstr_realloc(&cstr_buf, STRING_MAX_SIZE);
    tok_str_new(&tokstr_buf);
    tok_str_realloc(&tokstr_buf, TOKSTR_MAX_SIZE);

    /* define keywords, FIXME improve this */
    tok_ident = TOK_IDENT;
    TOK_INT    = tok_ident; tmp="int";    tok_alloc(tmp, strlen(tmp));
    TOK_IF     = tok_ident; tmp="if";     tok_alloc(tmp, strlen(tmp));
    TOK_ELSE   = tok_ident; tmp="else";   tok_alloc(tmp, strlen(tmp));
    TOK_WHILE  = tok_ident; tmp="while";  tok_alloc(tmp, strlen(tmp));
    TOK_BREAK  = tok_ident; tmp="break";  tok_alloc(tmp, strlen(tmp));
    TOK_RETURN = tok_ident; tmp="return"; tok_alloc(tmp, strlen(tmp));
}

/* 40 */
ST_FUNC void tccpp_delete(TCCState *s)
{
    int i, n;

    /* free tokens */
    n = tok_ident - TOK_IDENT;
    for(i = 0; i < n; i++)
        tcc_free(table_ident[i]);
    tcc_free(table_ident);
    table_ident = NULL;

    /* free static buffers */
    cstr_free(&tokcstr);
    cstr_free(&cstr_buf);
    tok_str_free_str(tokstr_buf.str);
}

/* ------------------------------------------------------------------------- */
