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

/********************************************************/
/* I/O layer */
/* 1 */
ST_FUNC void tcc_open_bf(TCCState *s1, const char *filename, int initlen) {
    BufferedFile *bf;
    int buflen;

    if(neq(initlen,0)) {
        buflen = initlen;
    } else {
        buflen = IO_BUF_SIZE;
    }

    bf = tcc_mallocz(sizeof(BufferedFile) + buflen);
    bf->buf_ptr = bf->buffer;
    bf->buf_end = bf->buffer + initlen;
    bf->buf_end[0] = CH_EOB; /* put eob symbol */
    pstrcpy(bf->filename, sizeof(bf->filename), filename);
    bf->line_num = 1;
    bf->fd = -1;
    bf->prev = file;
    file = bf;
    tok_flags = TOK_FLAG_BOL | TOK_FLAG_BOF;
}

/* 2 */
ST_FUNC void tcc_close(void)
{
    BufferedFile *bf = file;
    if (bf->fd > 0) {
        close(bf->fd);
        total_lines += bf->line_num;
    }
    file = bf->prev;
    tcc_free(bf);
}

/* 3 */
ST_FUNC int tcc_open(TCCState *s1, const char *filename)
{
    int fd;
    fd = open(filename, O_RDONLY | O_BINARY);
    tcc_open_bf(s1, filename, 0);
    file->fd = fd;
    return fd;
}

int init_globals() {
    aglobal_stack = &global_stack;
    alocal_stack = &local_stack;
}

/* 5 */
LIBTCCAPI void tcc_delete(TCCState *s1) {
    /* free sections */
    tccelf_delete(s1);

    tcc_free(gts_outfile(s1));
    dynarray_reset(&s1->files, &s1->nb_files);
    dynarray_reset(&s1->argv, &s1->argc);

    tcc_free(s1);
}

/* 6 */
ST_FUNC int tcc_add_file_internal(TCCState *s1, const char *filename, int flags)
{
    int ret;

    /* open the file */
    ret = tcc_open(s1, filename);

    ret = tcc_compile(s1);
    tcc_close();
    return ret;
}

/* 7 */
LIBTCCAPI int tcc_add_file(TCCState *s, const char *filename)
{
    int filetype = s->filetype;
    return tcc_add_file_internal(s, filename, 0);
}

#define WD_ALL    0x0001 /* warning is activated when using -Wall */
#define FD_INVERT 0x0002 /* invert value before storing */

/* 8 */
static int strstart(const char *val, const char **str)
{
    const char *p, *q;
    p = *str;
    q = val;
    while (*q) {
        if (*p != *q)
            return 0;
        p++;
        q++;
    }
    *str = p;
    return 1;
}

typedef struct TCCOption {
    const char *name;
    uint16_t index;
    uint16_t flags;
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

/* 9 */
static void args_parser_add_file(TCCState *s, const char* filename, int filetype)
{
    struct filespec *f = tcc_malloc(sizeof *f + strlen(filename));
    f->type = filetype;
    strcpy(f->name, filename);
    dynarray_add(&s->files, &s->nb_files, f);
}

/* 10 */
PUB_FUNC int tcc_parse_args(TCCState *s, int *pargc, char ***pargv, int optind)
{
    const TCCOption *popt;
    const char *optarg, *r;
    char **argv = *pargv;
    int argc = *pargc;


    while (lt(optind, argc)) {
        r = ri32(add(argv, mul(optind, 4)));
        optind = add(optind, 1);
        if (or(neq(ri8(r), mkc('-')), eq(ri8(add(r, 1)), 0))) {
            args_parser_add_file(s, r, s->filetype);
        } else {

            /* find option in table */
            for(popt = tcc_options; ; ) {
                const char *p1 = popt->name;
                const char *r1 = r + 1;
                if (strstart(p1, &r1)) {
                    optarg = r1;
                    if (popt->flags & TCC_OPTION_HAS_ARG) {
                        if (*r1 == '\0' && !(popt->flags & TCC_OPTION_NOSEP)) {
                            optarg = argv[optind++];
                        }
                    }
                    break;
                }
                popt = popt + 1;
            }

            if(popt->index == TCC_OPTION_o) {
                s->outfile = tcc_strdup(optarg);
            }
        }
    }

    return 0;
}
