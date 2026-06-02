function docs() {
  mks("This file must be valid C, JS, and AWK. See hex2_docs.txt");
  mks("AWK does not support C style comments so we can't use comments here.");
}

var output;
var jump_tables;
var Base_Address;
var ip;
var scratch;
var filename;
var linenumber;
var source_file;

var hold;
var toggle;

var source_filename;
var output_filename;

var max_string;

var TRUE;
var FALSE;

var EOF;
var NULL;

var entry_next_o;
var entry_target_o;
var entry_name_o;
var sizeof_entry;

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
function smatch_(a, b,    i) {
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
	return consume_token_(0, 0);
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
	return Throwaway_token_(0);
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

function LENGTH(s) {
	return LENGTH_(s, 0);
}
function LENGTH_(s,    i) {
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
	return GetTarget_(c, 0);
}
function GetTarget_(c,    i) {
	i = ri32(add(jump_tables, mul(GetHash(c), 4)));
	while(neq(NULL, i)) {
		if(smatch(c, entry_name_g(i))) {
			return entry_target_g(i);
		}
		i = entry_next_g(i);
	}
	v_exit(1);
}

function storeLabel(ip) {
	return storeLabel_(ip, 0, 0, 0);
}
function storeLabel_(ip,    c, h, entry) {
	entry = v_calloc(1, sizeof_entry);

	entry_target_s(entry, ip);

	c = consume_token();
	entry_name_s(entry, v_calloc(add(LENGTH(scratch), 1), 1));
	Copy_String(scratch, entry_name_g(entry));
	Clear_Scratch(scratch);

	h = GetHash(entry_name_g(entry));
	entry_next_s(entry, ri32(add(jump_tables, mul(h, 4))));
	wi32(add(jump_tables, mul(h, 4)), entry);

	return c;
}

function outputPointer(displacement, number_of_bytes) {
	return outputPointer_(displacement, number_of_bytes, 0, 0);
}
function outputPointer_(displacement, number_of_bytes,    b, value) {
	value = displacement;
	while(gt(number_of_bytes, 0)) {
		b = mod(value, 256);
		value = AND(shr(value, 8), 16777215);
		fputc(b, output);
		number_of_bytes = SUB(number_of_bytes, 1);
	}
}

function Update_Pointer(ch) {
	if(in_set(ch, mks("%&"))) {
		ip = add(ip, 4);
	} else if(eq(mkC("!"), ch)) {
		ip = add(ip, 1);
	} else {
		v_exit(1);
	}
}

function storePointer(ch) {
	return storePointer_(ch, 0, 0, 0, 0);
}
function storePointer_(ch,    base_sep_p, base, target, displacement) {
	Clear_Scratch(scratch);
	Update_Pointer(ch);
	base_sep_p = consume_token();

	target = GetTarget(scratch);
	displacement;

	base = ip;

	if (eq(mkC(">"), base_sep_p)) {
		Clear_Scratch(scratch);
		consume_token();
		base = GetTarget(scratch);
	}
	displacement = SUB(target, base);

	if(eq(mkC("!"), ch)) {
		outputPointer(displacement, 1);
	} else if(eq(mkC("&"), ch)) {
		outputPointer(target, 4);
	} else if(eq(mkC("%"), ch)) {
		outputPointer(displacement, 4);
	} else {
		v_exit(1);
	}
}

function line_Comment() {
	return line_Comment_(0);
}
function line_Comment_(    c) {
	c = nextc();
	while(eq(0, in_set(c, mks("\n\r")))) {
		if(eq(EOF, c)) {
			break;
		}
		c = nextc();
	}
	linenumber = add(linenumber, 1);
}

function hex(c) {
	if (in_set(c, mks("0123456789"))) {
		return SUB(c, 48);
	} else if (in_set(c, mks("abcdef"))) {
		return SUB(c, 87);
	} else if (in_set(c, mks("ABCDEF"))) {
		return SUB(c, 55);
	} else if (in_set(c, mks("#;"))) {
		line_Comment();
	} else if (eq(mkC("\n"), c)) {
		linenumber = add(linenumber, 1);
	}
	return SUB(0, 1);
}

function process_byte(c, write) {
	if(lte(0, hex(c))) {
		if(toggle) {
			if(write) {
				fputc(add(mul(hold, 16), hex(c)), output);
			}
			ip = add(ip, 1);
			hold = 0;
		} else {
			hold = hex(c);
		}
		if(toggle) {
			toggle = FALSE;
		} else {
			toggle = TRUE;
		}
	}
}

function first_pass() {
	return first_pass_(0);
}
function first_pass_(    c) {
	linenumber = 1;
	source_file = v_fopen(source_filename, mks("r"));

	toggle = FALSE;
	c = nextc();
	while(neq(EOF, c)) {
		if(eq(mkC(":"), c)) {
			c = storeLabel(ip);
		}
		if(in_set(c, mks("!%&"))) {
			Update_Pointer(c);
			c = Throwaway_token();
			if (eq(mkC(">"), c)) {
				c = Throwaway_token();
			}
		} else {
			process_byte(c, FALSE);
		}
		c = nextc();
	}
	v_fclose(source_file);
}

function second_pass() {
	return second_pass_(0);
}
function second_pass_(    c) {
	linenumber = 1;
	source_file = v_fopen(source_filename, mks("r"));

	toggle = FALSE;
	hold = 0;

	c = nextc();
	while(neq(EOF, c)) {
		if(eq(mkC(":"), c)) {
			c = Throwaway_token();
		} else if(in_set(c, mks("!%&"))) {
			storePointer(c);
		} else {
			process_byte(c, TRUE);
		}
		c = nextc();
	}
	v_fclose(source_file);
}

function init_globals() {
	max_string  = 4096;

	TRUE = 1;
	FALSE = 0;

	EOF = SUB(0, 1);
	NULL = 0;

	entry_next_o = 0;
	entry_target_o = 4;
	entry_name_o = 8;
	sizeof_entry = 12;
}

function main(argc, argv) {
	init_support();
	init_globals();

	jump_tables = v_calloc(65537, 4);
	Base_Address = 134512640;

	if(eq(argc, 4)) {
		mks("hack for xp_linux win32 support see xp_linux readme");
		Base_Address = SUB(12288, 5632);
	}
	if(eq(argc, 5)) {
		mks("2nd hack for xp_linux win32 support see xp_linux readme");
		Base_Address = add(4194304, SUB(12288, 5632));
	}

	scratch = v_calloc(add(max_string, 1), 1);

	source_filename = ri32(add(argv, 4));
	output_filename = ri32(add(argv, 8));
	output = v_fopen(output_filename, mks("w"));

	ip = Base_Address;
	first_pass();

	ip = Base_Address;
	second_pass();

	v_fclose(output);

	chmod(output_filename, 488);

	return 0;
}
