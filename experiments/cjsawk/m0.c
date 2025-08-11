/* -*- c-file-style: "linux";indent-tabs-mode:t -*- */
/* Copyright (C) 2016 Jeremiah Orians
 * Copyright (C) 2017 Jan Nieuwenhuizen <janneke@gnu.org>
 * This file a mix of files from:
 *   mescc-tools
 *   M2-Planet
 *
 * with modifications (C) 2025 Liam Wilson
 *
 * mescc-tools is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * mescc-tools is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this file.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

/* Internal processing Constants */
#define max_string 4096
#define PROCESSED 1
#define STR 2
#define NEWLINE 3

struct blob
{
	struct blob* next;
	int type;
	char* Text;
	char* Expression;
	struct blob* hash_next;
};

struct Token
{
	struct Token* next;
	struct blob* contents;
};

/* Globals */
FILE* source_file;
FILE* destination_file;
int linenumber;
char* filename;
struct Token* token_list;
struct blob* blob_list;
struct blob* define_blob;
struct blob* newline_blob;
int blob_count;
char* SCRATCH;
struct blob** hash_table;

int define_state = 0;

int hex2char(int c) {
	if((c >= 0) && (c <= 9)) {
		return (c + 48);
	} else if((c >= 10) && (c <= 15)) {
		return (c + 55);
	} else {
		return -1;
	}
}

char* hex_to_str_le(int value, int digits) {
	int i;
	int v = value;
	char *s = calloc(42, sizeof(char));
	for(i = 0; i < digits; i=i+2) {
		s[i + 1] = hex2char(v & 15);
		v = v >> 4;
		s[i] = hex2char(v & 15);
		v = v >> 4;
	}
	return s;
}

void require(int bool, char* error) {
	if(!bool) {
		fputs(error, stderr);
		exit(EXIT_FAILURE);
	}
}

int match(char* a, char* b)
{
	if((NULL == a) && (NULL == b)) return TRUE;
	if(NULL == a) return FALSE;
	if(NULL == b) return FALSE;

	int i = -1;
	do
	{
		i = i + 1;
		if(a[i] != b[i])
		{
			return FALSE;
		}
	} while((0 != a[i]) && (0 !=b[i]));
	return TRUE;
}


int in_set(int c, char* s)
{
	/* NULL set is always false */
	if(NULL == s) return FALSE;

	while(0 != s[0])
	{
		if(c == s[0]) return TRUE;
		s = s + 1;
	}
	return FALSE;
}

/* INTERNAL ONLY */
int __index_number(char* s, char c)
{
	int i = 0;
	while(s[i] != c)
	{
		i = i + 1;
		if(0 == s[i]) return -1;
	}
	return i;
}

/* INTERNAL ONLY */
int __toupper(int c)
{
	if(in_set(c, "abcdefghijklmnopqrstuvwxyz")) return (c & 0xDF);
	return c;
}

/* INTERNAL ONLY */
int __set_reader(char* set, int mult, char* input)
{
	int n = 0;
	int i = 0;
	int hold;
	int negative_p = FALSE;

	if(input[0] == '-')
	{
		negative_p = TRUE;
		i = i + 1;
	}

	while(in_set(input[i], set))
	{
		n = n * mult;
		hold = __index_number(set, __toupper(input[i]));

		/* Input managed to change between in_set and index_number */
		if(-1 == hold) return 0;
		n = n + hold;
		i = i + 1;
	}

	/* loop exited before NULL and thus invalid input */
	if(0 != input[i]) return 0;

	if(negative_p)
	{
		n = 0 - n;
	}

	return n;
}

int strtoint(char *a)
{
	int result = 0;
	/* If NULL string */
	if(0 == a[0])
	{
		result = 0;
	}
	/* Deal with binary */
	else if ('0' == a[0] && 'b' == a[1])
	{
		result = __set_reader("01", 2, a+2);
	}
	/* Deal with hex */
	else if ('0' == a[0] &&  'x' == a[1])
	{
		result = __set_reader("0123456789ABCDEFabcdef", 16, a+2);
	}
	/* Deal with octal */
	else if('0' == a[0])
	{
		result = __set_reader("01234567", 8, a+1);
	}
	/* Deal with decimal */
	else
	{
		result = __set_reader("0123456789", 10, a);
	}

	/* Deal with sign extension for 64bit hosts */
	if(0 != (0x80000000 & result)) result = (0xFFFFFFFF << 31) | result;
	return result;
}

char* int2str(int x, int base, int signed_p)
{
	require(1 < base, "int2str doesn't support a base less than 2\n");
	require(37 > base, "int2str doesn't support a base more than 36\n");
	/* Be overly conservative and save space for 32binary digits and padding null */
	char* p = calloc(34, sizeof(char));
	/* if calloc fails return null to let calling code deal with it */
	if(NULL == p) return p;

	p = p + 32;
	unsigned i;
	int sign_p = FALSE;
	char* table = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	if(signed_p && (10 == base) && (0 != (x & 0x80000000)))
	{
		/* Truncate to 31bits */
		i = -x & 0x7FFFFFFF;
		if(0 == i) return "-2147483648";
		sign_p = TRUE;
	} /* Truncate to 32bits */
	else i = x & (0x7FFFFFFF | (1 << 31));

	do
	{
		p[0] = table[i % base];
		p = p - 1;
		i = i / base;
	} while(0 < i);

	if(sign_p)
	{
		p[0] = '-';
		p = p - 1;
	}

	return p + 1;
}

void line_error() {
	fputs(filename, stderr);
	fputs(":", stderr);
	fputs(int2str(linenumber,10, FALSE), stderr);
	fputs(" :", stderr);
}

void ClearScratch() {
	int i = 0;
	int c = SCRATCH[i];
	while(0 != c) {
		SCRATCH[i] = 0;
		i = i + 1;
		c = SCRATCH[i];
	}
}

int GetHash(char* s) {
	int i = 5381;
	while(0 != s[0]) {
		i = (i << 5) + i + s[0];
		s = s + 1;
	}
	return i & 0xFFFF;
}

struct blob* FindBlob() {
	int hash = GetHash(SCRATCH);
	struct blob* i = hash_table[hash];

	if(define_state>0) {
		define_state=define_state - 1;
		if(define_state == 1) {
			/* this allows redefinitions of DEFINEs since
			   returning NULL will cause a new definition to be
                           inserted */
			return NULL;
		}
	}

	while(NULL != i) {
		if(match(SCRATCH, i->Text)) return i;
		i = i->hash_next;
	}
	return NULL;
}

void AddHash(struct blob* a, char* s) {
	int i = GetHash(s);
	a->hash_next = hash_table[i];
	hash_table[i] = a;
}

void NewBlob(int size) {
	blob_count = blob_count + 1;
	struct blob* a = calloc(1, sizeof(struct blob));
	require(NULL != a, "Exhausted available memory\n");
	a->Text = calloc(size + 1, sizeof(char));
	require(NULL != a->Text, "Exhausted available memory\n");

	int i = 0;
	while(i <= size) {
		a->Text[i] = SCRATCH[i];
		i = i + 1;
	}
	a->next = blob_list;
	blob_list = a;
	AddHash(a, SCRATCH);
}

struct Token* newToken() {
	struct Token* p;
	p = calloc (1, sizeof (struct Token));
	return p;
}

struct Token* reverse_list(struct Token* head) {
	struct Token* root = NULL;
	struct Token* next;
	while(NULL != head) {
		next = head->next;
		head->next = root;
		root = head;
		head = next;
	}
	return root;
}

void purge_lineComment() {
	int c = fgetc(source_file);
	while(!in_set(c, "\n\r")) {
		if(EOF == c) break;
		c = fgetc(source_file);
	}
}

struct Token* append_newline(struct Token* head) {
	linenumber = linenumber + 1;
	if(NULL == head) {
		return NULL;
	}
	if(NEWLINE == head->contents->type) {
		/* Don't waste whitespace*/
		return head;
	}

	struct Token* lf = newToken();
	lf->contents = newline_blob;
	lf->next = head;
	return lf;
}


struct Token* store_atom(struct Token* head, char c) {
	ClearScratch();
	int ch = c;
	int i = 0;
	while(1) {
		SCRATCH[i] = ch;
		ch = fgetc(source_file);
		i = i + 1;
		if((EOF == ch) || in_set(ch, "\t\n ")) {
			break;
		}
	}

	head->contents = FindBlob();

	if(define_blob == head->contents) {
		/* see also FindBlob as this modifies its behaviour */
		define_state = 2;
	}

	if(NULL == head->contents) {
		NewBlob(i);
		head->contents = blob_list;
	}

	if('\n' == ch) {
		return append_newline(head);
	}
	return head;
}

struct blob* store_string(char c) {
	ClearScratch();

	int ch = c;
	int i = 0;
	while(1) {
		SCRATCH[i] = ch;
		i = i + 1;
		if('\n' == ch) {
			linenumber = linenumber + 1;
		}
		ch = fgetc(source_file);
		require(EOF != ch, "Unmatched \"!\n");
		if(ch == c) {
			break;
		}
	}

	struct blob* a = FindBlob();
	if(NULL == a) {
		NewBlob(i);
		a = blob_list;
		a->type = STR;
	}

	return a;
}

struct Token* Tokenize_Line(struct Token* head) {
	int c;
	struct Token* p;
	linenumber = 1;

	while(1) {
		c = fgetc(source_file);
		if(in_set(c, ";#")) {
			purge_lineComment();
			head = append_newline(head);
		} else if(in_set(c, "\t ")) {
			/* nothing */
		} else if('\n' == c) {
			head = append_newline(head);
		} else if(EOF == c) {
			head = append_newline(head);
			break;
		} else {
			p = newToken();
			p->next = head;
			if(in_set(c, "'\"")) {
				p->contents = store_string(c);
			} else {
				p = store_atom(p, c);
			}
			head = p;
		}
	}
	return head;
}

char* express_number(int value, char c) {
	char* ch;
	int size;
	int number_of_bytes;
	if('!' == c) {
		number_of_bytes = 1;
		value = value & 0xFF;
	} else if('@' == c) {
		number_of_bytes = 2;
		value = value & 0xFFFF;
	} else if('%' == c) {
		number_of_bytes = 4;
		value = value & 0xFFFFFFFF;
	} else {
		fputs("Given symbol ", stderr);
		fputc(c, stderr);
		fputs(" to express immediate value ", stderr);
		fputs(int2str(value, 10, TRUE), stderr);
		fputc('\n', stderr);
		exit(EXIT_FAILURE);
	}

	size = number_of_bytes * 2;

	ch = hex_to_str_le(value, size);

	return ch;
}

void line_macro(struct Token* p) {
	struct Token* i;
	struct blob* co;
	int value;
	char c;
	for(i = p; NULL != i; i = i->next) {
		co = i->contents;
		if(define_blob == co) {
			require(NULL != i->next, "Macro name must exist\n");
			require(NULL != i->next->next, "Macro value must exist\n");
			if(PROCESSED == i->next->contents->type) {
				line_error();
				fputs("Multiple definitions for macro ", stderr);
				fputs(i->next->contents->Text, stderr);
				fputs("\n", stderr);
				exit(EXIT_FAILURE);
			}

			i->contents = newline_blob;

			if (STR == i->next->next->contents->type) {
				i->contents->Expression = i->next->next->contents->Text + 1;
			} else {
				i->next->contents->Expression = i->next->next->contents->Text;
			}
			i->next = i->next->next->next;
		} else if(STR == co->type) {
			if('\'' == co->Text[0]) {
				co->Expression = co->Text + 1;
			} else if('"' == co->Text[0]) {
				hexify_string(co);
			}
		} else if((PROCESSED == co->type) || (NEWLINE == co->type)) {
			/* nothing */
		} else if(NULL == co->Expression) {
			if(in_set(co->Text[0], "%~@!")) {
				value = strtoint(co->Text + 1);

				if(('0' == co->Text[1]) || (0 != value)) {
					co->Expression = express_number(value, co->Text[0]);
				}
			}
			if((NULL == co->Expression) && !(co->type & PROCESSED)) {
				c = co->Text[0];

				if(in_set(c, "!@$~%&:^")) {
					co->Expression = co->Text;
				}
			}
		}
	}
}

void hexify_string(struct blob* p) {
	char* table = "0123456789ABCDEF";
	int i = strlen(p->Text);
	int size;

	size = (i * 2) + 1;

	require(1 != size, "hexify_string lacked a valid bytemode\n");
	char* d = calloc(size, sizeof(char));
	require(NULL != d, "Exhausted available memory\n");
	p->Expression = d;
	char* S = p->Text;

	while(0 != S[0]) {
		S = S + 1;
		d[0] = table[S[0] >> 4];
		d[1] = table[S[0] & 0xF];
		d[2] = 0;
		d = d + 2;
	}
}

void preserve_other(struct blob* p) {
	struct blob* i;
	char c;
	for(i = p; NULL != i; i = i->next) {
		if((NULL == i->Expression) && !(i->type & PROCESSED)) {
			c = i->Text[0];

			if(in_set(c, "!@$~%&:^")) {
				i->Expression = i->Text;
			}
		}
	}
}

void print_hex(struct Token* p) {
	struct Token* i;
	for(i = p; NULL != i; i = i->next) {
		if(NEWLINE == i->contents->type) {
			if(NULL == i->next) {
				fputc('\n', destination_file);
			} else if(NEWLINE != i->next->contents->type) {
				fputc('\n', destination_file);
			}
		} else if(NULL != i->contents->Expression) {
			fputs(i->contents->Expression, destination_file);
			if(NEWLINE != i->next->contents->type) {
				fputc('\n', destination_file);
			}
		} else {
			line_error();
			fputs("Received invalid other; ", stderr);
			fputs(i->contents->Text, stderr);
			fputs("\n", stderr);
			exit(EXIT_FAILURE);
		}
	}
}

/* Standard C main program */
int main(int argc, char **argv) {
	destination_file = stdout;
	blob_count = 2;
	hash_table = calloc(65537, sizeof(struct blob*));
	require(NULL != hash_table, "failed to allocate hash_table\n");

	/* Create newline blob */
	newline_blob = calloc(1, sizeof(struct blob));
	require(NULL != newline_blob, "failed to allocate newline_blob\n");
	newline_blob->Text = "\n";
	newline_blob->Expression = "\n";
	newline_blob->type = NEWLINE;
	AddHash(newline_blob, "\n");

	/* Start the blob list with DEFINE and newline */
	blob_list = calloc(1, sizeof(struct blob));
	require(NULL != blob_list, "failed to allocate DEFINE blob\n");
	blob_list->Text = "DEFINE";
	define_blob = blob_list;
	blob_list->next = newline_blob;
	AddHash(define_blob, "DEFINE");

	/* Initialize scratch */
	SCRATCH = calloc(max_string + 1, sizeof(char));
	require(NULL != SCRATCH, "failed to allocate SCRATCH buffer");

	filename = argv[1];
	source_file = fopen(filename, "r");

	token_list = Tokenize_Line(token_list);

	fclose(source_file);

	destination_file = fopen(argv[2], "w");

	token_list = reverse_list(token_list);
	line_macro(token_list);
	preserve_other(blob_list);
	print_hex(token_list);

	fclose(destination_file);

	return EXIT_SUCCESS;
}
