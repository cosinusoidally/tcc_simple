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

ST_DATA int parse_flags;

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
static int pp_debug_tok, pp_debug_symv;
static int pp_once;
static int pp_expr;
static int pp_counter;
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

int init_tccpp_globals(){
  aTOK_HASH_SIZE = TOK_HASH_SIZE;
  ahash_ident = hash_ident;
  acstr_buf = &cstr_buf;
  CH_EOF_ = CH_EOF;
  aisidnum_table = isidnum_table;
  atokc = &tokc;
}

/* 29 */
/* get a token from an integer array and increment pointer
   accordingly. we code it as a macro to avoid pointer aliasing. */
static inline void TOK_GET(int *t, const int **pp, CValue *cv) {
    int n;
    const int *p;
    p = *pp;

    *t = *p;
    p = add(p, 4);
    if((t == TOK_STR) || (t == TOK_PPNUM) || (t == TOK_PPSTR)) {
        scv_str_size(cv, wi32(p));
        p = add(p, 4);
        wi32(gcv_str_data(cv), p);
        p = add(p, mul(div_(sub(add(gcv_str_size(cv), sizeof_int), 1), sizeof_int), 4));
    }

    wi32(pp, p);
}

/* 30 */
/* evaluate escape codes in a string. */
static void parse_escape_string(CString *outstr, const uint8_t *buf, int is_long)
{
    int c, n;
    const uint8_t *p;

    p = buf;
    while(1) {
        c = *p;
        if (c == '\0')
            break;
        if (c == '\\') {
            p++;
            /* escape */
            c = *p;
            if(c == 'n') {
                c = '\n';
            } else if(c == 'r') {
                c = '\r';
            } else if(c == 't') {
                c = '\t';
            }
        }
        p++;
        cstr_ccat(outstr, c);
    }
    /* add a trailing '\0' */
    cstr_ccat(outstr, '\0');
}

/* 31 */
static void parse_string(const char *s, int len)
{
    uint8_t *p;
    int is_long, sep;

    is_long=0;
    sep = *s++;
    len -= 2;
    p = tcc_malloc(len + 1);
    memcpy(p, s, len);
    p[len] = 0;

    cstr_reset(&tokcstr);
    parse_escape_string(&tokcstr, p, is_long);
    tcc_free(p);

    if (sep == '\'') {
        int c;
        tok = TOK_CCHAR;
        c = ((char *)tokcstr.data)[0];
        tokc.i = c;
    } else {
        tokc.str.size = tokcstr.size;
        tokc.str.data = tokcstr.data;
        tok = TOK_STR;
    }
}

/* 32 */
/* parse number in null terminated string 'p' and return it in the
   current token */
static void parse_number(const char *p)
{
    int b, t, shift, s, ch;
    char *q;
    unsigned long long n, n1;

    /* number */
    q = token_buf;
    ch = *p++;
    t = ch;
    ch = *p++;
    *q++ = t;
    b = 10;
    if (t == '0') {
        if (ch == 'x' || ch == 'X') {
            q--;
            ch = *p++;
            b = 16;
        }
    }
    /* parse all digits. cannot check octal numbers at this stage
       because of floating point constants */
    while (1) {
        if (ch >= 'A' && ch <= 'F')
            t = ch - 'A' + 10;
        else if (isnum(ch))
            t = ch - '0';
        else
            break;
        *q++ = ch;
        ch = *p++;
    }

    /* integer number */
    *q = '\0';
    q = token_buf;
    if (b == 10 && *q == '0') {
        b = 8;
        q++;
    }
    n = 0;
    while(1) {
        t = *q++;
        /* no need for checks except for base 10 / 8 errors */
        if (t == '\0')
            break;
        else if (t >= 'A')
            t = t - 'A' + 10;
        else
            t = t - '0';
        n1 = n;
        n = n * b + t;
    }

    tok = TOK_CINT;
    ++tok; /* TOK_CU... */
    tokc.i = n;
}

/* 33 */
/* return next token without macro substitution */
static inline void next_nomacro1(void)
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

/* 34 */
/* return next token without macro substitution. Can read input from
   macro_ptr buffer */
static void next_nomacro_spc(void)
{
    if (macro_ptr) {
        tok = *macro_ptr;
        if (tok) {
            TOK_GET(&tok, &macro_ptr, &tokc);
        }
    } else {
        next_nomacro1();
    }
}

/* 35 */
ST_FUNC void next_nomacro(void)
{
    do {
        next_nomacro_spc();
    } while (tok < 256 && (isidnum_table[tok - CH_EOF] & IS_SPC));
}

/* 36 */
/* return next token with macro substitution */
ST_FUNC void next(void)
{
    int redo;
    while(1) {
        redo = 0;
        if (parse_flags & PARSE_FLAG_SPACES)
            next_nomacro_spc();
        else
            next_nomacro();

        if (macro_ptr) {
            if (tok == 0) {
                /* end of macro or unget token string */
                end_macro();
                redo = 1;
            }
        }
        if(redo == 0) {
            break;
        }
    }
    /* convert preprocessor tokens into C tokens */
    if (tok == TOK_PPNUM) {
        if  (parse_flags & PARSE_FLAG_TOK_NUM)
            parse_number((char *)tokc.str.data);
    } else if (tok == TOK_PPSTR) {
        if (parse_flags & PARSE_FLAG_TOK_STR)
            parse_string((char *)tokc.str.data, tokc.str.size - 1);
    }
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
ST_FUNC void preprocess_start(TCCState *s1, int is_asm)
{
    CString cstr;
    int i;

    pp_expr = 0;
    pp_counter = 0;
    pp_debug_tok = pp_debug_symv = 0;
    pp_once++;
    pvtop = vtop = vstack - 1;

    set_idnum('$', 0);
    set_idnum('.', is_asm ? IS_ID : 0);

    cstr_new(&cstr);
    cstr_cat(&cstr, "\"", -1);
    cstr_cat(&cstr, file->filename, -1);
    cstr_cat(&cstr, "\"", 0);

    cstr_reset(&cstr);
    cstr_free(&cstr);

    parse_flags = 0;
    tok_flags = TOK_FLAG_BOL | TOK_FLAG_BOF;
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
