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

/* Globals */
int output;
struct entry** jump_tables;
int Base_Address;
int ip;
char* scratch;
char* filename;
int linenumber;
int source_file;

/* For processing bytes */
int hold;
int toggle;

char* source_filename;

#define max_string 4096
#define TRUE 1
#define FALSE 0

int EOF=-1;
int NULL=0;

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

function nextc() {
	return v_fgetc(source_file);
}

function smatch(a, b) {
	return smatch_(a, b, 0);
}
function smatch_(a, b, i) {
	i = SUB(0, 1);
	while(1) {
		i = add(i, 1);
		if(neq(ri8(add(a, i)), ri8(add(b, i)))) {
			return FALSE;
		}
		if(AND(eq(0, ri8(add(a, i))), eq(0, ri8(add(b, i))))) {
			return TRUE;
		}
	}
}

function in_set(c, s) {
	while(neq(0, ri8(s))) {
		if(eq(c, ri8(s))) {
			return TRUE;
		}
		s = add(s, 1);
	}
	return FALSE;
}

int consume_token(int source_file) {
	int i = 0;
	int c = nextc();
	while(!in_set(c, " \t\n>")) {
		scratch[i] = c;
		i = i + 1;
		c = nextc();
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
		c = nextc();
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

function Clear_Scratch(s) {
	return Clear_Scratch_(s,0, 0);
}
function Clear_Scratch_(s,    i, c) {
	i = 0;
	c = ri8(add(s, i));
	while(neq(0, c)) {
		wi8(add(s, i), 0);
		i = add(i, 1);
		c = ri8(add(s, i));
	}
}

void Copy_String(char* a, char* b) {
	while(0 != a[0]) {
		b[0] = a[0];
		a = a + 1;
		b = b + 1;
	}
}

function GetHash(s) {
	return GetHash_(s, 0);
}
function GetHash_(s,    i) {
	i = 5381;
	while(neq(0, ri8(s))) {
		i = add(shl(i, 5), add(i, ri8(s)));
		s = add(s, 1);
	}
	return AND(i, 65535);
}

unsigned GetTarget(char* c) {
	struct entry* i;
	for(i = jump_tables[GetHash(c)]; NULL != i; i = i->next) {
		if(smatch(c, i->name)) {
			return i->target;
		}
	}
	exit(1);
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
		exit(1);
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
		exit(1);
	}
}

void line_Comment(int source_file) {
	int c = nextc();
	while(!in_set(c, "\n\r")) {
		if(EOF == c) break;
		c = nextc();
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
	source_file = fopen(source_filename, "r");

	toggle = FALSE;
	int c;
	for(c = nextc(); EOF != c; c = nextc()) {
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
	source_file = fopen(source_filename, "r");

	toggle = FALSE;
	hold = 0;

	int c;
	for(c = nextc(); EOF != c; c = nextc()) {
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

	return 0;
}
