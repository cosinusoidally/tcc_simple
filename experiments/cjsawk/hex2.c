/* Copyright (C) 2016,2017 Jeremiah Orians
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

#include <stdlib.h>
#include <sys/stat.h>

/* Globals */
int output;
struct entry** jump_tables;
int Base_Address;
int ip;
char* scratch;
char* filename;
int linenumber;

/* For processing bytes */
int hold;
int toggle;

char* source_filename;

#define max_string 4096
#define TRUE 1
#define FALSE 0

int EOF=-1;

struct input_files
{
	struct input_files* next;
	char* filename;
};

struct entry
{
	struct entry* next;
	unsigned target;
	char* name;
};

int match(char* a, char* b) {
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


int in_set(int c, char* s) {
	/* NULL set is always false */
	if(NULL == s) return FALSE;

	while(0 != s[0]) {
		if(c == s[0]) return TRUE;
		s = s + 1;
	}
	return FALSE;
}

int consume_token(int source_file) {
	int i = 0;
	int c = fgetc(source_file);
	while(!in_set(c, " \t\n>")) {
		scratch[i] = c;
		i = i + 1;
		c = fgetc(source_file);
		if(EOF == c) {
			break;
		}
	}
	return c;
}

int Throwaway_token(int source_file) {
	int c;
	do
	{
		c = fgetc(source_file);
		if(EOF == c) {
			break;
		}
	} while(!in_set(c, " \t\n>"));

	return c;
}

int length(char* s) {
	int i = 0;
	while(0 != s[i]) {
		i = i + 1;
	}
	return i;
}

void Clear_Scratch(char* s) {
	do {
		s[0] = 0;
		s = s + 1;
	} while(0 != s[0]);
}

void Copy_String(char* a, char* b) {
	while(0 != a[0]) {
		b[0] = a[0];
		a = a + 1;
		b = b + 1;
	}
}

int GetHash(char* s) {
	int i = 5381;
	while(0 != s[0]) {
		i = i * 31 + s[0];
		s = s + 1;
	}
	return (i & 0xFFFF);
}

unsigned GetTarget(char* c) {
	struct entry* i;
	for(i = jump_tables[GetHash(c)]; NULL != i; i = i->next) {
		if(match(c, i->name)) {
			return i->target;
		}
	}
	exit(EXIT_FAILURE);
}

int storeLabel(int source_file, int ip) {
	struct entry* entry = calloc(1, sizeof(struct entry));

	/* Ensure we have target address */
	entry->target = ip;

	/* Store string */
	int c = consume_token(source_file);
	entry->name = calloc(length(scratch) + 1, sizeof(char));
	Copy_String(scratch, entry->name);
	Clear_Scratch(scratch);

	/* Prepend to list */
	int h = GetHash(entry->name);
	entry->next = jump_tables[h];
	jump_tables[h] = entry;

	return c;
}

void outputPointer(int displacement, int number_of_bytes) {
	unsigned byte;
	unsigned value = displacement;
	while(number_of_bytes > 0) {
		byte = value % 256;
		value = value / 256;
		fputc(byte, output);
		number_of_bytes = number_of_bytes - 1;
	}
}

void Update_Pointer(char ch) {
	/* Calculate pointer size*/
	if(in_set(ch, "%&")) {
		ip = ip + 4; /* Deal with % and & */
	} else if('!' == ch) {
		ip = ip + 1; /* Deal with ! */
	} else {
		exit(EXIT_FAILURE);
	}
}

void storePointer(char ch, int source_file) {
	/* Get string of pointer */
	Clear_Scratch(scratch);
	Update_Pointer(ch);
	int base_sep_p = consume_token(source_file);

	/* Lookup token */
	int target = GetTarget(scratch);
	int displacement;

	int base = ip;

	/* Change relative base address to :<base> */
	if ('>' == base_sep_p) {
		Clear_Scratch(scratch);
		consume_token (source_file);
		base = GetTarget (scratch);
	}
	displacement = (target - base);

	/* output calculated difference */
	if('!' == ch) {
		outputPointer(displacement, 1); /* Deal with ! */
	} else if('&' == ch) {
		outputPointer(target, 4); /* Deal with & */
	} else if('%' == ch) {
		outputPointer(displacement, 4);  /* Deal with % */
	} else {
		exit(EXIT_FAILURE);
	}
}

void line_Comment(int source_file) {
	int c = fgetc(source_file);
	while(!in_set(c, "\n\r")) {
		if(EOF == c) break;
		c = fgetc(source_file);
	}
	linenumber = linenumber + 1;
}

int hex(int c, int source_file) {
	if (in_set(c, "0123456789")) {
		return (c - 48);
	} else if (in_set(c, "abcdef")) {
		return (c - 87);
	} else if (in_set(c, "ABCDEF")) {
		return (c - 55);
	} else if (in_set(c, "#;")) {
		line_Comment(source_file);
	} else if ('\n' == c) {
		linenumber = linenumber + 1;
	}
	return -1;
}

void process_byte(char c, int source_file, int write) {
	if(0 <= hex(c, source_file)) {
		if(toggle) {
			if(write) {
				fputc(((hold * 16)) + hex(c, source_file), output);
			}
			ip = ip + 1;
			hold = 0;
		} else {
			hold = hex(c, source_file);
		}
		toggle = !toggle;
	}
}

void first_pass() {
	linenumber = 1;
	int source_file = fopen(source_filename, "r");

	toggle = FALSE;
	int c;
	for(c = fgetc(source_file); EOF != c; c = fgetc(source_file)) {
		/* Check for and deal with label */
		if(':' == c) {
			c = storeLabel(source_file, ip);
		}

		/* check for and deal with relative/absolute pointers to labels */
		if(in_set(c, "!%&")) {
			/* deal with 1byte pointer !; 4byte pointers (% and &) */
			Update_Pointer(c);
			c = Throwaway_token(source_file);
			if ('>' == c) { /* deal with label>base */
				c = Throwaway_token(source_file);
			}
		} else {
			process_byte(c, source_file, FALSE);
		}
	}
	fclose(source_file);
}

void second_pass() {
	linenumber = 1;
	int source_file = fopen(source_filename, "r");

	toggle = FALSE;
	hold = 0;

	int c;
	for(c = fgetc(source_file); EOF != c; c = fgetc(source_file)) {
		if(':' == c) {
			c = Throwaway_token(source_file);
		} else if(in_set(c, "!%&")) {
			storePointer(c, source_file);
		} else {
			process_byte(c, source_file, TRUE);
		}
	}
	fclose(source_file);
}

int main(int argc, char **argv) {
	jump_tables = calloc(65537, sizeof(struct entry*));
	Base_Address = 0x8048000;
	struct input_files* input = NULL;
	char* output_filename;
	scratch = calloc(max_string + 1, sizeof(char));
	struct input_files* temp;

	source_filename = argv[1];
	output_filename = argv[2];
	output = fopen(output_filename, "w");

	/* Get all of the labels */
	ip = Base_Address;
	first_pass();

	/* Fix all the references*/
	ip = Base_Address;
	second_pass();

	fclose(output);

	chmod(output_filename, 0750);

	return EXIT_SUCCESS;
}
