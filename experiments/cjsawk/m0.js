function docs() {
  mks("This file must be valid C, JS, and AWK. See m0_docs.txt");
  mks("AWK does not support C style comments so we can't use comments here.");
}

var TRUE;
var FALSE;
var NULL;
var EOF;

var max_string;
var STR;

var source_file;
var destination_file;
var token_list;
var define_blob;
var SCRATCH;
var hash_table;

var define_state;

var blob_type_o;
var blob_Text_o;
var blob_Expression_o;
var blob_hash_next_o;
var sizeof_blob;

var ch_dquote;

function blob_type_g(o) {
	return ri32(add(o, blob_type_o));
}
function blob_type_s(o, v) {
	wi32(add(o, blob_type_o), v);
}

function blob_Text_g(o) {
	return ri32(add(o, blob_Text_o));
}
function blob_Text_s(o, v) {
	wi32(add(o, blob_Text_o), v);
}

function blob_Expression_g(o) {
	return ri32(add(o, blob_Expression_o));
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

function Token_next_g(o) {
	return ri32(add(o, Token_next_o));
}
function Token_next_s(o, v) {
	wi32(add(o, Token_next_o), v);
}

function Token_contents_g(o) {
	return ri32(add(o, Token_contents_o));
}
function Token_contents_s(o, v) {
	wi32(add(o, Token_contents_o), v);
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

function hex2char(c) {
	if(lte(c, 9)) {
		return (add(c, 48));
	}
	return (add(c, 55));
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
		result = parse_int(add(a, 2), 16);
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
	return AddHash_(a, s, 0);
}
function AddHash_(a, s,    i) {
	i = GetHash(s);
	blob_hash_next_s(a, ri32(add(hash_table, mul(i, 4))));
	wi32(add(hash_table, mul(i, 4)),a);
}

function NewBlob(size) {
	return NewBlob_(size, 0, 0);
}
function NewBlob_(size,    i, a) {
	a = v_calloc(1, sizeof_blob);
	blob_Text_s(a, v_calloc(add(size, 1), 1));
	while(lte(i, size)) {
		wi8(add(blob_Text_g(a), i), ri8(add(SCRATCH, i)));
		i = add(i, 1);
	}
	AddHash(a, SCRATCH);
	return a;
}

function newToken() {
	return newToken_(0);
}
function newToken_(    p) {
	p = v_calloc(1, sizeof_Token);
	return p;
}

function reverse_list(head) {
	return reverse_list_(head, 0, 0);
}
function reverse_list_(head,    root, NEXT) {
	while(neq(NULL, head)) {
		NEXT = Token_next_g(head);
		Token_next_s(head, root);
		root = head;
		head = NEXT;
	}
	return root;
}

function purge_lineComment() {
	return purge_lineComment_(0);
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

function store_atom(head, c) {
	return store_atom_(head, c, 0, 0);
}
function store_atom_(head, c,    ch, i) {
	ClearScratch();
	ch = c;
	while(1) {
		wi8(add(SCRATCH, i), ch);
		ch = fgetc(source_file);
		i = add(i, 1);
		if(OR(eq(EOF, ch), in_set(ch, mks("\t\n ")))) {
			break;
		}
	}
	Token_contents_s(head, FindBlob());
	if(eq(define_blob, Token_contents_g(head))) {
		define_state = 2;
	}
	if(eq(NULL, Token_contents_g(head))) {
		Token_contents_s(head, NewBlob(i));
	}
	return head;
}

function store_string(c) {
	return store_string_(c, 0, 0, 0);
}
function store_string_(c,    ch, i, a) {
	ClearScratch();
	ch = c;
	while(1) {
		wi8(add(SCRATCH, i), ch);
		i = add(i, 1);
		ch = fgetc(source_file);
		if(eq(ch, c)) {
			break;
		}
	}
	a = FindBlob();
	if(eq(NULL, a)) {
		a = NewBlob(i);
		blob_type_s(a, STR);
	}
	return a;
}

function Tokenize_Line(head) {
	return Tokenize_Line_(head, 0, 0);
}
function Tokenize_Line_(head,    c, p) {
	while(1) {
		c = fgetc(source_file);
		if(in_set(c, mks(";#"))) {
			purge_lineComment();
		} else if(eq(EOF, c)) {
			break;
		} else if(eq(0, in_set(c, mks("\t \n")))) {
			p = newToken();
			Token_next_s(p, head);
			if(OR(eq(c, mkC("'")),eq(c, mkC("\"")))) {
				Token_contents_s(p, store_string(c));
			} else {
				p = store_atom(p, c);
			}
			head = p;
		}
	}
	return head;
}

function express_number(value, c) {
	return express_number_(value, c, 0, 0);
}
function express_number_(value, c,    ch, digits) {
	digits = 8;
	if(eq(mkC("!"), c)) {
		digits = 2;
		value = AND(value, 255);
	}
	ch = hex_to_str_le(value, digits);
	return ch;
}

function hexify_string(p) {
	return hexify_string_(p, 0, 0, 0, 0, 0);
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

function process_tokens(p) {
	return process_tokens_(p, 0, 0, 0, 0);
}
function process_tokens_(p,    i, co, value, c) {
	i = p;
	while(neq(NULL, i)) {
		co = Token_contents_g(i);
		if(eq(define_blob, co)) {
			blob_Expression_s(Token_contents_g(Token_next_g(i)), blob_Text_g(Token_contents_g(Token_next_g(Token_next_g(i)))));
			Token_next_s(i, Token_next_g(Token_next_g(Token_next_g(i))));
		} else if(eq(STR, blob_type_g(co))) {
			if(eq(mkC("'"), ri8(blob_Text_g(co)))) {
				blob_Expression_s(co, add(blob_Text_g(co), 1));
			} else if(eq(mkC("\""), ri8(blob_Text_g(co)))) {
				hexify_string(co);
			}
		} else if(eq(NULL, blob_Expression_g(co))) {
			if(in_set(ri8(blob_Text_g(co)), mks("%!"))) {
				value = strtoint(add(blob_Text_g(co), 1));

				if(OR(eq(mkC("0"),ri8(add(blob_Text_g(co), 1))),neq(0,value))) {
					blob_Expression_s(co, express_number(value, ri8(blob_Text_g(co))));
				}
			}
			if(eq(NULL, blob_Expression_g(co))) {
				c = ri8(blob_Text_g(co));

				if(in_set(c, mks("!%&:"))) {
					blob_Expression_s(co, blob_Text_g(co));
				}
			}
		}
		i = Token_next_g(i);
	}
}

function print_hex(p) {
	return print_hex_(p, 0);
}
function print_hex_(p,    i) {
	i = p;
	while(neq(NULL, i)) {
		if(neq(NULL, blob_Expression_g(Token_contents_g(i)))) {
			fputs(blob_Expression_g(Token_contents_g(i)), destination_file);
			fputc(mkC("\n"), destination_file);
		}
		i = Token_next_g(i);
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

	blob_type_o = 0;
	blob_Text_o = 4;
	blob_Expression_o = 8;
	blob_hash_next_o = 12;
	sizeof_blob = 16;

	Token_next_o = 0;
	Token_contents_o = 4;
	sizeof_Token = 8;

	ch_dquote = 34;
}

function main(argc, argv) {
	init_support();
	init_globals();

	hash_table = v_calloc(65537, sizeof_blob);
	SCRATCH = v_calloc(add(max_string, 1), 1);

	define_blob = v_calloc(1, sizeof_blob);
	blob_Text_s(define_blob, mks("DEFINE"));
	AddHash(define_blob, mks("DEFINE"));

	source_file = fopen(ri32(add(argv, 4)),mks("r"));
	token_list = Tokenize_Line(token_list);
	fclose(source_file);

	destination_file = fopen(ri32(add(argv, 8)), mks("w"));
	token_list = reverse_list(token_list);
	process_tokens(token_list);
	print_hex(token_list);
	fclose(destination_file);

	return 0;
}
