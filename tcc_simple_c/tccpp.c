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

ST_DATA int tok_flags;
ST_DATA int parse_flags;

ST_DATA struct BufferedFile *file;
ST_DATA int ch, tok;
ST_DATA CValue tokc;
ST_DATA const int *macro_ptr;
ST_DATA CString tokcstr; /* current parsed string, if any */

/* display benchmark infos */
ST_DATA int total_lines;
ST_DATA int total_bytes;
ST_DATA int tok_ident;
ST_DATA TokenSym **table_ident;

/* ------------------------------------------------------------------------- */

static TokenSym *hash_ident[TOK_HASH_SIZE];
static char token_buf[STRING_MAX_SIZE + 1];
static CString cstr_buf;
static CString macro_equal_buf;
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

static TokenString *macro_stack;

static const char tcc_keywords[] = 
#define DEF(id, str) str "\0"
#include "tcctok.h"
#undef DEF
;

static void next_nomacro_spc(void);

ST_FUNC void skip(int c)
{
    next();
}

#define tal_free(al, p) tcc_free(p)
#define tal_realloc(al, p, size) tcc_realloc(p, size)
#define tal_new(a,b,c)
#define tal_delete(a)

/* ------------------------------------------------------------------------- */
/* CString handling */
static void cstr_realloc(CString *cstr, int new_size)
{
    int size;

    size = cstr->size_allocated;
    if (size < 8)
        size = 8; /* no need to allocate a too small first string */
    while (size < new_size)
        size = size * 2;
    cstr->data = tal_realloc(cstr_alloc, cstr->data, size);
    cstr->size_allocated = size;
}

/* add a byte */
ST_INLN void cstr_ccat(CString *cstr, int ch)
{
    int size;
    size = cstr->size + 1;
    if (size > cstr->size_allocated)
        cstr_realloc(cstr, size);
    ((unsigned char *)cstr->data)[size - 1] = ch;
    cstr->size = size;
}

ST_FUNC void cstr_cat(CString *cstr, const char *str, int len)
{
    int size;
    if (len <= 0)
        len = strlen(str) + 1 + len;
    size = cstr->size + len;
    if (size > cstr->size_allocated)
        cstr_realloc(cstr, size);
    memmove(((unsigned char *)cstr->data) + cstr->size, str, len);
    cstr->size = size;
}

ST_FUNC void cstr_new(CString *cstr)
{
    memset(cstr, 0, sizeof(CString));
}

/* free string and reset it to NULL */
ST_FUNC void cstr_free(CString *cstr)
{
    tal_free(cstr_alloc, cstr->data);
    cstr_new(cstr);
}

/* reset string to empty */
ST_FUNC void cstr_reset(CString *cstr)
{
    cstr->size = 0;
}

/* ------------------------------------------------------------------------- */
/* allocate a new token */
static TokenSym *tok_alloc_new(TokenSym **pts, const char *str, int len)
{
    TokenSym *ts, **ptable;
    int i;

    /* expand token table if needed */
    i = tok_ident - TOK_IDENT;
    if ((i % TOK_ALLOC_INCR) == 0) {
        ptable = tcc_realloc(table_ident, (i + TOK_ALLOC_INCR) * sizeof(TokenSym *));
        table_ident = ptable;
    }

    ts = tal_realloc(toksym_alloc, 0, sizeof(TokenSym) + len);
    table_ident[i] = ts;
    ts->tok = tok_ident++;
    ts->sym_identifier = NULL;
    ts->len = len;
    ts->hash_next = NULL;
    memcpy(ts->str, str, len);
    ts->str[len] = '\0';
    *pts = ts;
    return ts;
}

#define TOK_HASH_INIT 1
#define TOK_HASH_FUNC(h, c) ((h) + ((h) << 5) + ((h) >> 27) + (c))


/* find a token and add it if not found */
ST_FUNC TokenSym *tok_alloc(const char *str, int len)
{
    TokenSym *ts, **pts;
    int i;
    unsigned int h;
    
    h = TOK_HASH_INIT;
    for(i=0;i<len;i++)
        h = TOK_HASH_FUNC(h, ((unsigned char *)str)[i]);
    h &= (TOK_HASH_SIZE - 1);

    pts = &hash_ident[h];
    for(;;) {
        ts = *pts;
        if (!ts)
            break;
        if (ts->len == len && !memcmp(ts->str, str, len))
            return ts;
        pts = &(ts->hash_next);
    }
    return tok_alloc_new(pts, str, len);
}

ST_FUNC const char *get_tok_str(int v, CValue *cv)
{
    char *p;
    int i, len;

    cstr_reset(&cstr_buf);
    p = cstr_buf.data;

    if (v < tok_ident) {
        return table_ident[v - TOK_IDENT]->str;
    } else if (v >= SYM_FIRST_ANOM) {
        /* special name for anonymous symbol */
        sprintf(p, "L.%u", v - SYM_FIRST_ANOM);
    }
    return cstr_buf.data;
}

/* return the current character, handling end of block if necessary
   (but not stray) */
ST_FUNC int handle_eob(void)
{
    BufferedFile *bf = file;
    int len;

    /* only tries to read if really end of buffer */
    if (bf->buf_ptr >= bf->buf_end) {
        if (bf->fd >= 0) {
            len = IO_BUF_SIZE;
            len = read(bf->fd, bf->buffer, len);
            if (len < 0)
                len = 0;
        } else {
            len = 0;
        }
        total_bytes += len;
        bf->buf_ptr = bf->buffer;
        bf->buf_end = bf->buffer + len;
        *bf->buf_end = CH_EOB;
    }
    if (bf->buf_ptr < bf->buf_end) {
        return bf->buf_ptr[0];
    } else {
        bf->buf_ptr = bf->buf_end;
        return CH_EOF;
    }
}

/* skip the stray and handle the \\n case. Output an error if
   incorrect char after the stray */
static int handle_stray1(uint8_t *p)
{
    int c;

    file->buf_ptr = p;
    if (p >= file->buf_end) {
        c = handle_eob();
        if (c != '\\')
            return c;
    }
}

/* handle just the EOB case, but not stray */
#define PEEKC_EOB(c, p)\
{\
    p++;\
    c = *p;\
    if (c == '\\') {\
        file->buf_ptr = p;\
        c = handle_eob();\
        p = file->buf_ptr;\
    }\
}

/* handle the complicated stray case */
#define PEEKC(c, p)\
{\
    p++;\
    c = *p;\
    if (c == '\\') {\
        c = handle_stray1(p);\
        p = file->buf_ptr;\
    }\
}

/* C comments */
ST_FUNC uint8_t *parse_comment(uint8_t *p)
{
    int c;

    p++;
    while(1) {
        /* fast skip loop */
        while(1) {
            c = *p;
            if (c == '\n' || c == '*' || c == '\\')
                break;
            p++;
            c = *p;
            if (c == '\n' || c == '*' || c == '\\')
                break;
            p++;
        }
        /* now we can handle all the cases */
        if (c == '\n') {
            file->line_num++;
            p++;
        } else if (c == '*') {
            p++;
            while(1) {
                c = *p;
                if (c == '*') {
                    p++;
                } else if (c == '/') {
                    p++;
                    return p;
                } else {
                    break;
                }
            }
        } else {
            /* stray, eob or eof */
            file->buf_ptr = p;
            c = handle_eob();
            p = file->buf_ptr;
            if (c == '\\') {
                p++;
            }
        }
    }
    p++;
    return p;
}

ST_FUNC int set_idnum(int c, int val)
{
    int prev = isidnum_table[c - CH_EOF];
    isidnum_table[c - CH_EOF] = val;
    return prev;
}

/* parse a string without interpreting escapes */
static uint8_t *parse_pp_string(uint8_t *p,
                                int sep, CString *str)
{
    int c;
    p++;
    for(;;) {
        c = *p;
        if (c == sep) {
            break;
        } else if (c == '\\') {
            file->buf_ptr = p;
            c = handle_eob();
            p = file->buf_ptr;
            if (c == '\\') {
                /* escape : just skip \[\r]\n */
                PEEKC_EOB(c, p);
                if (str) {
                    cstr_ccat(str, '\\');
                    cstr_ccat(str, c);
                }
                p++;
            }
        } else {
            if (str)
                cstr_ccat(str, c);
            p++;
        }
    }
    p++;
    return p;
}

/* token string handling */
ST_INLN void tok_str_new(TokenString *s)
{
    s->str = NULL;
    s->len = s->lastlen = 0;
    s->allocated_len = 0;
}

ST_FUNC TokenString *tok_str_alloc(void)
{
    TokenString *str = tal_realloc(tokstr_alloc, 0, sizeof *str);
    tok_str_new(str);
    return str;
}

ST_FUNC void tok_str_free_str(int *str)
{
    tal_free(tokstr_alloc, str);
}

ST_FUNC void tok_str_free(TokenString *str)
{
    tok_str_free_str(str->str);
    tal_free(tokstr_alloc, str);
}

ST_FUNC int *tok_str_realloc(TokenString *s, int new_size)
{
    int *str, size;

    size = s->allocated_len;
    if (size < 16)
        size = 16;
    while (size < new_size)
        size = size * 2;
    if (size > s->allocated_len) {
        str = tal_realloc(tokstr_alloc, s->str, size * sizeof(int));
        s->allocated_len = size;
        s->str = str;
    }
    return s->str;
}

ST_FUNC void tok_str_add(TokenString *s, int t)
{
    int len, *str;

    len = s->len;
    str = s->str;
    if (len >= s->allocated_len)
        str = tok_str_realloc(s, len + 1);
    str[len++] = t;
    s->len = len;
}

ST_FUNC void begin_macro(TokenString *str, int alloc)
{
    str->alloc = alloc;
    str->prev = macro_stack;
    str->prev_ptr = macro_ptr;
    macro_ptr = str->str;
    macro_stack = str;
}

ST_FUNC void end_macro(void)
{
    TokenString *str = macro_stack;
    macro_stack = str->prev;
    macro_ptr = str->prev_ptr;
    tok_str_free(str);
}

static void tok_str_add2(TokenString *s, int t, CValue *cv)
{
    int len, *str;

    len = s->lastlen = s->len;
    str = s->str;

    /* allocate space for worst case */
    if (len + TOK_MAX_SIZE >= s->allocated_len)
        str = tok_str_realloc(s, len + TOK_MAX_SIZE + 1);
    str[len++] = t;
    if(t == TOK_STR) {
        /* Insert the string into the int array. */
        size_t nb_words =
            1 + (cv->str.size + sizeof(int) - 1) / sizeof(int);
        if (len + nb_words >= s->allocated_len)
            str = tok_str_realloc(s, len + nb_words + 1);
        str[len] = cv->str.size;
        memcpy(&str[len + 1], cv->str.data, cv->str.size);
        len += nb_words;
    }
    s->len = len;
}

/* add the current parse token in token string 's' */
ST_FUNC void tok_str_add_tok(TokenString *s)
{
    tok_str_add2(s, tok, &tokc);
}

/* get a token from an integer array and increment pointer
   accordingly. we code it as a macro to avoid pointer aliasing. */
static inline void TOK_GET(int *t, const int **pp, CValue *cv)
{
    int n, *tab, tmp;
    const int *p = *pp;

    tab = cv->tab;

    tmp = (*t = *p++);
    if((tmp == TOK_STR) || (tmp == TOK_PPNUM) || (tmp == TOK_PPSTR)) {
        cv->str.size = *p++;
        cv->str.data = p;
        p += (cv->str.size + sizeof(int) - 1) / sizeof(int);
    }

    *pp = p;
}

/* evaluate escape codes in a string. */
static void parse_escape_string(CString *outstr, const uint8_t *buf, int is_long)
{
    int c, n;
    const uint8_t *p;

    p = buf;
    for(;;) {
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

/* we use 64 bit numbers */
#define BN_SIZE 2

/* parse number in null terminated string 'p' and return it in the
   current token */
static void parse_number(const char *p)
{
    int b, t, shift, frac_bits, s, exp_val, ch;
    char *q;
    unsigned int bn[BN_SIZE];
    double d;

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
    unsigned long long n, n1;
    int lcount, ucount, ov = 0;
    const char *p1;

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
        h = TOK_HASH_INIT;
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
            for(;;) {
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
            PEEKC(c, p);
            while (isidnum_table[c - CH_EOF] & (IS_ID|IS_NUM))
            {
                cstr_ccat(&tokcstr, c);
                PEEKC(c, p);
            }
            ts = tok_alloc(tokcstr.data, tokcstr.size);
        }
        tok = ts->tok;
    } else if(isnum(c)){
        t = c;
        PEEKC(c, p);
        /* after the first digit, accept digits, alpha, '.' or sign if
           prefixed by 'eEpP' */
        cstr_reset(&tokcstr);
        for(;;) {
            cstr_ccat(&tokcstr, t);
            if (!((isidnum_table[c - CH_EOF] & (IS_ID|IS_NUM))
                  || c == '.'
                  || (c == '+' || c == '-'))) {
                break;
            }
            t = c;
            PEEKC(c, p);
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
        PEEKC(c, p);
        tok = '=';
    } else if(c == '/'){
        PEEKC(c, p);
        if (c == '*') {
            p = parse_comment(p);
            /* comments replaced by a blank */
            tok = ' ';
            file->buf_ptr = p;
            return;
        }
    } else {
    switch(c) {
        /* simple tokens */
    case '(':
    case ')':
    case '{':
    case '}':
    case ',':
    case ';':
        tok = c;
        p++;
        break;
    }
    }
    tok_flags = 0;
    file->buf_ptr = p;
}

/* return next token without macro substitution. Can read input from
   macro_ptr buffer */
static void next_nomacro_spc(void)
{
    if (macro_ptr) {
    redo:
        tok = *macro_ptr;
        if (tok) {
            TOK_GET(&tok, &macro_ptr, &tokc);
        }
    } else {
        next_nomacro1();
    }
}

ST_FUNC void next_nomacro(void)
{
    do {
        next_nomacro_spc();
    } while (tok < 256 && (isidnum_table[tok - CH_EOF] & IS_SPC));
}
 
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

ST_FUNC void tccpp_new(TCCState *s)
{
    int i, c;
    const char *p, *r;

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
    
    tok_ident = TOK_IDENT;
    p = tcc_keywords;
    while (*p) {
        r = p;
        for(;;) {
            c = *r++;
            if (c == '\0')
                break;
        }
        tok_alloc(p, r - p - 1);
        p = r;
    }
}

ST_FUNC void tccpp_delete(TCCState *s)
{
    int i, n;

    /* free tokens */
    n = tok_ident - TOK_IDENT;
    for(i = 0; i < n; i++)
        tal_free(toksym_alloc, table_ident[i]);
    tcc_free(table_ident);
    table_ident = NULL;

    /* free static buffers */
    cstr_free(&tokcstr);
    cstr_free(&cstr_buf);
    cstr_free(&macro_equal_buf);
    tok_str_free_str(tokstr_buf.str);

    /* free allocators */
    tal_delete(toksym_alloc);
    toksym_alloc = NULL;
    tal_delete(tokstr_alloc);
    tokstr_alloc = NULL;
    tal_delete(cstr_alloc);
    cstr_alloc = NULL;
}

/* ------------------------------------------------------------------------- */
