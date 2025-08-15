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
#define STR 2

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
struct Token* token_list;
struct blob* blob_list;
struct blob* define_blob;
int blob_count;
char* SCRATCH;
struct blob** hash_table;

int define_state = 0;

int hex2char(int c) {
	if(c <= 9) {
		return (c + 48);
	}
	return (c + 55);
}

char* hex_to_str_le(int value, int digits) {
	int i;
	int v = value;
	char *s = calloc(42, sizeof(char));
	i = 0;
	while(i < digits) {
		s[i + 1] = hex2char(v & 15);
		v = v >> 4;
		s[i] = hex2char(v & 15);
		v = v >> 4;
		i = i + 2;
	}
	return s;
}

int match(char* a, char* b) {
	int i = -1;
	while(1) {
		i = i + 1;
		if(a[i] != b[i]) {
			return FALSE;
		}
		if((0 == a[i]) && (0 ==b[i])) {
			return TRUE;
		}
	}
}


int in_set(int c, char* s) {
	while(0 != s[0]) {
		if(c == s[0]) {
			return TRUE;
		}
		s = s + 1;
	}
	return FALSE;
}

int parse_int(char* input, int base) {
	int n = 0;
	int i = 0;
	int hold;
	int t;
	int negative_p = FALSE;

	if(input[0] == '-') {
		negative_p = TRUE;
		i = i + 1;
	}
	while(input[i] != 0) {
		n = n * base;
		t = input[i];
		if((t - '0') < 10) {
			hold = t - '0';
		} else {
			hold = 10 + (t - 'A');
		}
		n = n + hold;
		i = i + 1;
	}
	if(negative_p) {
		n = 0 - n;
	}
	return n;
}

int strtoint(char *a) {
	int result = 0;
	if(0 == a[0]) {
		result = 0;
	} else if ('0' == a[0] &&  'x' == a[1]) {
		result = parse_int(a+2, 16);
	} else if ((('0' <= a[0]) &&  ('9' >= a[0])) || ('-' == a[0])) {
		result = parse_int(a, 10);
	} else {
		result = 0;
	}
	return result;
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
		if(match(SCRATCH, i->Text)) {
			return i;
		}
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
	int i;
	blob_count = blob_count + 1;
	struct blob* a = calloc(1, sizeof(struct blob));
	a->Text = calloc(size + 1, sizeof(char));

	i = 0;
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
		if(EOF == c) {
			break;
		}
		c = fgetc(source_file);
	}
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

	return head;
}

struct blob* store_string(char c) {
	ClearScratch();

	int ch = c;
	int i = 0;
	while(1) {
		SCRATCH[i] = ch;
		i = i + 1;
		ch = fgetc(source_file);
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

	while(1) {
		c = fgetc(source_file);
		if(in_set(c, ";#")) {
			purge_lineComment();
		} else if(EOF == c) {
			break;
		} else if(!in_set(c, "\t \n")) {
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
	}

	size = number_of_bytes * 2;

	ch = hex_to_str_le(value, size);

	return ch;
}

void hexify_string(struct blob* p) {
	char* table = "0123456789ABCDEF";
	int i = 0;
	int size;
	char c;

	while(1) {
		c = p->Text[i];
		if(c == 0) {
			break;
		}
		i = i + 1;
	}

	size = (i * 2) + 1;

	char* d = calloc(size, sizeof(char));
	p->Expression = d;
	char* S = p->Text;

	while(0 != S[0]) {
		S = S + 1;
		d[0] = table[S[0] >> 4];
		d[1] = table[S[0] & 15];
		d = d + 2;
	}
}

void process_tokens(struct Token* p) {
	struct Token* i;
	struct blob* co;
	int value;
	char c;
	for(i = p; NULL != i; i = i->next) {
		co = i->contents;
		if(define_blob == co) {
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
		} else if(NULL == co->Expression) {
			if(in_set(co->Text[0], "%~@!")) {
				value = strtoint(co->Text + 1);

				if(('0' == co->Text[1]) || (0 != value)) {
					co->Expression = express_number(value, co->Text[0]);
				}
			}
			if(NULL == co->Expression) {
				c = co->Text[0];

				if(in_set(c, "!@$~%&:^")) {
					co->Expression = co->Text;
				}
			}
		}
	}
}

void print_hex(struct Token* p) {
	struct Token* i;
	for(i = p; NULL != i; i = i->next) {
		if(NULL != i->contents->Expression) {
			fputs(i->contents->Expression, destination_file);
			fputc('\n', destination_file);
		}
	}
}

int main(int argc, char **argv) {
	blob_count = 1;
	hash_table = calloc(65537, sizeof(struct blob*));

	blob_list = calloc(1, sizeof(struct blob));
	define_blob = blob_list;
	define_blob->Text = "DEFINE";
	AddHash(define_blob, "DEFINE");

	SCRATCH = calloc(max_string + 1, sizeof(char));

	source_file = fopen(argv[1], "r");

	token_list = Tokenize_Line(token_list);

	fclose(source_file);

	destination_file = fopen(argv[2], "w");

	token_list = reverse_list(token_list);
	process_tokens(token_list);

	print_hex(token_list);

	fclose(destination_file);

	return EXIT_SUCCESS;
}
