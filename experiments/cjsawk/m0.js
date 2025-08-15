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

var blob_next_o;
var blob_type_o;
var blob_Text_o;
var blob_Expression_o;
var blob_hash_next_o;
var sizeof_blob;

function blob_next_s(o, v) {
	wi32(add(o, blob_next_o), v);
}

function blob_Text_g(o) {
	return ri32(add(o, blob_Text_o));
}
function blob_Text_s(o, v) {
	wi32(add(o, blob_Text_o), v);
}

function blob_Expression_s(o, v) {
	wi32(add(o, blob_Expression_o), v);
}

function blob_hash_next_g(o) {
	return ri32(add(o, blob_hash_next_o));
}
function blob_hash_next_s(o, v) {
	wi32(add(o, blob_hash_next_o), v);
}

var Token_next_o;
var Token_contents_o;
var sizeof_Token;

var TRUE;
var FALSE;

var max_string;
var STR;

int source_file;
int destination_file;
struct Token* token_list;
struct blob* blob_list;
struct blob* define_blob;
char* SCRATCH;
struct blob** hash_table;

var define_state;
var EOF;
var NULL;

/* FIXME split ri32 and wi32 into separate files */

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

function hex2char(c) {
	if(c <= 9) {
		return (c + 48);
	}
	return (c + 55);
}


function hex_to_str_le(v, digits) {
	return hex_to_str_le_(v, digits, 0, 0);
}
function hex_to_str_le_(v, digits,    i, s) {
	s = v_calloc(42, 1);
	i = 0;
	while(lt(i, digits)) {
		wi8(add(s, add(i, 1)), hex2char(AND(v, 15)));
		v = shr(v, 4);
		wi8(add(s, i), hex2char(AND(v, 15)));
		v = shr(v, 4);
		i = add(i, 2);
	}
	return s;
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
		if(eq(0, ri8(add(a, i))) && eq(0, ri8(add(b, i)))) {
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

function parse_int(input, base) {
	return parse_int_(input, base, 0, 0 ,0, 0, 0);
}
function parse_int_(input, base,    n, i, hold, t, negative_p) {
	negative_p = FALSE;

	if(eq(ri8(input), mkC("-"))) {
		negative_p = TRUE;
		i = add(i, 1);
	}
	while(neq(ri8(add(input, i)), 0)) {
		n = mul(n, base);
		t = ri8(add(input, i));
		if(lt(SUB(t, mkC("0")), 10)) {
			hold = SUB(t, mkC("0"));
		} else {
			hold = add(10, SUB(t, mkC("A")));
		}
		n = add(n, hold);
		i = add(i, 1);
	}
	if(negative_p) {
		n = SUB(0, n);
	}
	return n;
}

function strtoint(a) {
	return strtoint_(a, 0);
}
function strtoint_(a,    result) {
	result = 0;
	if(eq(0, ri8(a))) {
		result = 0;
	} else if (AND(eq(mkC("0"), ri8(a)),  eq(mkC("x"), ri8(add(a, 1))))) {
		result = parse_int(a+2, 16);
	} else if (OR(AND(lte(mkC("0"),ri8(a)),gte(mkC("9"),ri8(a))),eq(mkC("-"),ri8(a)))) {
		result = parse_int(a, 10);
	} else {
		result = 0;
	}
	return result;
}

function ClearScratch(    i, c) {
	return ClearScratch_(0, 0);
}
function ClearScratch_(    i, c) {
	i = 0;
	c = ri8(add(SCRATCH, i));
	while(neq(0, c)) {
		wi8(add(SCRATCH, i), 0);
		i = add(i, 1);
		c = ri8(add(SCRATCH, i));
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

function FindBlob() {
	return FindBlob_(0, 0);
}
function FindBlob_(    hash, i) {
	hash = GetHash(SCRATCH);
	i = ri32(add(hash_table, mul(hash, 4)));
	if(gt(define_state, 0)) {
		define_state = SUB(define_state, 1);
		if(eq(define_state, 1)) {
			/* this allows redefinitions of DEFINEs since
			   returning NULL will cause a new definition to be
                           inserted */
			return NULL;
		}
	}
	while(neq(NULL, i)) {
		if(smatch(SCRATCH, blob_Text_g(i))) {
			return i;
		}
		i = blob_hash_next_g(i);
	}
	return NULL;
}

function AddHash(a, s) {
	AddHash_(a, s, 0);
}
function AddHash_(a, s,    i) {
	i = GetHash(s);
	blob_hash_next_s(a, ri32(add(hash_table, mul(i, 4))));
	wi32(add(hash_table, mul(i, 4)),a);
}

function NewBlob(size) {
	NewBlob_(size, 0, 0);
}
function NewBlob_(size,    i, a) {
	a = v_calloc(1, sizeof_blob);
	blob_Text_s(a, v_calloc(add(size, 1), 1));
	while(lte(i, size)) {
		wi8(add(blob_Text_g(a), i), ri8(add(SCRATCH, i)));
		i = add(i, 1);
	}
	blob_next_s(a, blob_list);
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

function purge_lineComment() {
	purge_lineComment_(0);
}
function purge_lineComment_(    c) {
	c = fgetc(source_file);
	while(eq(0, in_set(c, mks("\n\r")))) {
		if(eq(EOF, c)) {
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
		wi8(add(SCRATCH, i), ch);
		ch = fgetc(source_file);
		i = add(i, 1);
		if(OR(eq(EOF, ch), in_set(ch, mks("\t\n ")))) {
			break;
		}
	}
	head->contents = FindBlob();
	if(eq(define_blob, head->contents)) {
		/* see also FindBlob as this modifies its behaviour */
		define_state = 2;
	}
	if(eq(NULL, head->contents)) {
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
	int digits;
	digits = 8;
	if('!' == c) {
		digits = 2;
		value = value & 255;
	}
	ch = hex_to_str_le(value, digits);
	return ch;
}

function hexify_string(p) {
	hexify_string_(p, 0, 0, 0, 0, 0);
}
function hexify_string_(p,    i, size, c, S, d) {
	while(1) {
		c = ri8(add(blob_Text_g(p), i));
		if(eq(c, 0)) {
			break;
		}
		i = add(i, 1);
	}

	size = add(mul(i, 2), 1);

	d = v_calloc(size, 1);
	blob_Expression_s(p, d);
	S = blob_Text_g(p);

	while(neq(0, ri8(S))) {
		S = add(S, 1);
		wi8(d, hex2char(shr(ri8(S), 4)));
		wi8(add(d, 1), hex2char(AND(ri8(S), 15)));
		d = add(d, 2);
	}
}

void process_tokens(struct Token* p) {
	struct Token* i;
	struct blob* co;
	int value;
	char c;
	i = p;
	while(NULL != i) {
		co = i->contents;
		if(define_blob == co) {
			i->next->contents->Expression = i->next->next->contents->Text;
			i->next = i->next->next->next;
		} else if(STR == co->type) {
			if('\'' == co->Text[0]) {
				co->Expression = co->Text + 1;
			} else if('"' == co->Text[0]) {
				hexify_string(co);
			}
		} else if(NULL == co->Expression) {
			if(in_set(co->Text[0], "%!")) {
				value = strtoint(co->Text + 1);

				if(('0' == co->Text[1]) || (0 != value)) {
					co->Expression = express_number(value, co->Text[0]);
				}
			}
			if(NULL == co->Expression) {
				c = co->Text[0];

				if(in_set(c, "!%&:")) {
					co->Expression = co->Text;
				}
			}
		}
		i = i->next;
	}
}

void print_hex(struct Token* p) {
	struct Token* i;
	i = p;
	while(NULL != i) {
		if(NULL != i->contents->Expression) {
			fputs(i->contents->Expression, destination_file);
			fputc('\n', destination_file);
		}
		i = i->next;
	}
}

function init_globals() {
	TRUE = 1;
	FALSE = 0;

	max_string = 4096;
	STR = 2;

	define_state = 0;
	EOF = SUB(0, 1);
	NULL = 0;

	blob_next_o = 0;
	blob_type_o = 4;
	blob_Text_o = 8;
	blob_Expression_o = 12;
	blob_hash_next_o = 16;
	sizeof_blob = 20;

	Token_next_o = 0;
	Token_contents_o = 4;
	sizeof_Token = 8;
}

int main(int argc, char **argv) {
	init_support();
	init_globals();

	hash_table = calloc(65537, sizeof(struct blob*));
	SCRATCH = calloc(max_string + 1, sizeof(char));

	blob_list = calloc(1, sizeof(struct blob));
	define_blob = blob_list;
	define_blob->Text = "DEFINE";
	AddHash(define_blob, "DEFINE");

	source_file = fopen(argv[1], "r");
	token_list = Tokenize_Line(token_list);
	fclose(source_file);

	destination_file = fopen(argv[2], "w");
	token_list = reverse_list(token_list);
	process_tokens(token_list);
	print_hex(token_list);
	fclose(destination_file);

	return 0;
}
