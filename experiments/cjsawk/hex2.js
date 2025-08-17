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

struct entry
{
	struct entry* next;
	unsigned target;
	char* name;
};

var entry_next_o = 0;
var entry_target_o = 4;
var entry_name_o = 8;
var sizeof_entry = 12;

function entry_next_g(o) {
	return ri32(add(o, entry_next_o));
}
function entry_next_s(o, v) {
	wi32(add(o, entry_next_o), v);
}

function entry_target_g(o) {
	return ri32(add(o, entry_target_o));
}
function entry_target_s(o, v) {
	wi32(add(o, entry_target_o), v);
}

function entry_name_g(o) {
	return ri32(add(o, entry_name_o));
}
function entry_name_s(o, v) {
	wi32(add(o, entry_name_o), v);
}

function ri32(o) {
	return OR(OR(AND(ri8(o), 255),
		shl(AND(ri8(add(o, 1)), 255), 8)),
		OR(shl(AND(ri8(add(o, 2)), 255), 16),
		shl(AND(ri8(add(o, 3)), 255), 24)));
}

function wi32(o, v) {
	wi8(o, AND(v, 255));
	v = shr(v, 8);
	wi8(add(o, 1), AND(v, 255));
	v = shr(v, 8);
	wi8(add(o, 2), AND(v, 255));
	v = shr(v, 8);
	wi8(add(o, 3), AND(v, 255));
}

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

function consume_token() {
	consume_token_(0, 0);
}
function consume_token_(    i, c) {
	c = nextc();
	while(eq(0,in_set(c, mks(" \t\n>")))) {
		wi8(add(scratch, i), c);
		i = add(i, 1);
		c = nextc();
		if(eq(EOF, c)) {
			break;
		}
	}
	return c;
}

function Throwaway_token() {
	Throwaway_token_(0);
}
function Throwaway_token_(c) {
	c = nextc();
	while(eq(0,in_set(c, mks(" \t\n>")))) {
		c = nextc();
		if(eq(EOF, c)) {
			break;
		}
	}
	return c;
}

function length(s) {
	length_(s, 0);
}
function length_(s,    i) {
	while(neq(0, ri8(add(s, i)))) {
		i = add(i, 1);
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

function Copy_String(a, b) {
	while(neq(0, ri8(a))) {
		wi8(b, ri8(a));
		a = add(a, 1);
		b = add(b, 1);
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

function GetTarget(c) {
	GetTarget_(c, 0);
}
function GetTarget_(c,    i) {
	i = ri32(add(jump_tables, mul(GetHash(c), 4)));
	while(neq(NULL, i)) {
		if(smatch(c, entry_name_g(i))) {
			return entry_target_g(i);
		}
		i = entry_next_g(i);
	}
	exit(1);
}

function storeLabel(ip) {
	storeLabel_(ip, 0, 0, 0);
}
function storeLabel_(ip,    c, h, entry) {
	entry = v_calloc(1, sizeof_entry);

	/* Ensure we have target address */
	entry_target_s(entry, ip);

	/* Store string */
	c = consume_token();
	entry_name_s(entry, v_calloc(add(length(scratch), 1), 1));
	Copy_String(scratch, entry_name_g(entry));
	Clear_Scratch(scratch);

	/* Prepend to list */
	h = GetHash(entry_name_g(entry));
	entry_next_s(entry, ri32(add(jump_tables, mul(h, 4))));
	wi32(add(jump_tables, mul(h, 4)), entry);

	return c;
}

function outputPointer(displacement, number_of_bytes) {
	int byte;
	unsigned value = displacement;
	while(gt(number_of_bytes, 0)) {
		byte = value % 256;
		value = value / 256;
		fputc(byte, output);
		number_of_bytes = SUB(number_of_bytes, 1);
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
	int base_sep_p = consume_token();

	/* Lookup token */
	int target = GetTarget(scratch);
	int displacement;

	int base = ip;

	/* Change relative base address to :<base> */
	if ('>' == base_sep_p) {
		Clear_Scratch(scratch);
		consume_token();
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
			c = storeLabel(ip);
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

function second_pass() {
	linenumber = 1;
	source_file = fopen(source_filename, mks("r"));

	toggle = FALSE;
	hold = 0;

	int c;
	c = nextc();
	while(neq(EOF, c)) {
		if(eq(mkC(":"), c)) {
			c = Throwaway_token(source_file);
		} else if(in_set(c, mks("!%&"))) {
			storePointer(c, source_file);
		} else {
			process_byte(c, source_file, TRUE);
		}
		c = nextc();
	}
	fclose(source_file);
}

function main(argc, argv) {
	jump_tables = v_calloc(65537, sizeof(struct entry*));
	Base_Address = 0x8048000;
	char* output_filename;
	scratch = v_calloc(add(max_string, 1), 1);

	source_filename = ri32(add(argv, 4));
	output_filename = ri32(add(argv, 8));
	output = fopen(output_filename, mks("w"));

	/* Get all of the labels */
	ip = Base_Address;
	first_pass();

	/* Fix all the references*/
	ip = Base_Address;
	second_pass();

	fclose(output);

	chmod(output_filename, 488); /* 0750 */

	return 0;
}
