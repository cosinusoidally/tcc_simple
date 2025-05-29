f=read("test.c","binary");
// f=read("../m2min_v3/m2min_v3.js","binary");
// f=read("../m2min_v3/artifacts/m2min_v3.c","binary");

var dbg;

if(dbg!=true) {
  dbg = false;
}

if(dbg == true) {
  dprint = print;
} else {
  dprint = function (){};
}

FALSE = false;
TRUE = true;

to=0;
ts=0;
te=0;
ch=f[0];

ln=1;

var current_count;

var tt;

tt_identifier = "identifier";
tt_number = "number";
tt_char = "char";
tt_string = "string";
tt_other = "other";

var tok;

var break_target_prefix;
var break_target_num;

/* needed since main has an additional number of elements initially on the
   stack */
var frame_bias;

function nc() {
  ch=f[to];
  to=to+1;
  if(ch == 10) {
    ln = ln + 1;
  }
}

function char0() {
  return tok[0];
}

function is_whitespace() {
  if((ch == 32) || (ch == 9)) {
    return true;
  }

  if(ch == 10) {
    return true;
  }

  return false;
}

function eat_whitespace() {
  while(is_whitespace()) {
    nc();
  }
}

function is_comment() {
  if(ch == c_fs) {
    return true;
  }
  return false;
}

var c_fs = "//".charCodeAt(0);
var c_star = "*".charCodeAt(0);

function eat_comment() {
  if(ch == c_fs) {
    nc();
    if(ch == c_star) {
      nc();
    } else {
      return;
    }
    while(1) {
      while(ch != c_star) {
        nc();
      }
      nc();
      if(ch == c_fs) {
        nc();
        return;
      }
    }
  }
}

var c_a = "a".charCodeAt(0);
var c_z = "z".charCodeAt(0);
var c_A = "A".charCodeAt(0);
var c_Z = "Z".charCodeAt(0);
var c__ = "_".charCodeAt(0);
var c_0 = "0".charCodeAt(0);
var c_9 = "9".charCodeAt(0);

function is_id() {
  if(((ch>=c_a) && (ch<=c_z) ||
      (ch>=c_A) && (ch<=c_Z)) || (ch == c__)) {
    return true;
  }
  return false;
}

function is_num() {
  if((ch>=c_0) && (ch<=c_9)) {
    return true;
  }
  return false;
}


var c_comma = ",".charCodeAt(0);
var c_semi = ";".charCodeAt(0);
var c_lparen = "(".charCodeAt(0);
var c_rparen = ")".charCodeAt(0);
var c_lcurly = "{".charCodeAt(0);
var c_rcurly = "}".charCodeAt(0);
var c_eq = "=".charCodeAt(0);

function is_other() {
  if((ch==c_comma) || (ch==c_semi) || (ch==c_lparen) || (ch==c_rparen) ||
     (ch==c_lcurly) || (ch==c_rcurly) || (ch==c_eq)) {
    return true;
  }
  return false;
}

var c_ch = "'".charCodeAt(0);
var c_bs = "\\".charCodeAt(0);

function is_char() {
  if(ch==c_ch) {
    return true;
  }
  return false;
}

var c_str = "\"".charCodeAt(0);

function is_string() {
  if(ch==c_str) {
    return true;
  }
  return false;
}

function f_str() {
  var i;
  var b = [];
  for(i=ts; i < te ; i = i + 1) {
    b.push(f[i]);
  }
  b = b.map(function(x){return String.fromCharCode(x)}).join("");
  dprint("tt: " + tt + " f_str " + b);
  tok = b;
}

function get_id() {
  tt = tt_identifier;
  ts = to - 1;
  while(is_num() || is_id()) {
    te = to;
    nc();
  }
  f_str();
}

function get_num() {
  tt = tt_number;
  ts = to - 1;
  while(is_num()) {
    te = to;
    nc();
  }
  f_str();
}

function get_other() {
  tt = tt_other;
  ts = to - 1;
  te = to;
  nc();
  f_str();
}

function get_char() {
  tt = tt_char;
  ts = to - 1;
  nc();
  if(ch == c_bs) {
    nc();
  }
  nc();
  te = to;
  nc();
  f_str();
}

function get_string() {
  tt = tt_string;
  ts = to - 1;
  nc();
  while(ch != c_str) {
    nc();
  }
  te = to;
  nc();
  f_str();
  /* FIXME hack to parse escape codes from JS, replace with cjsawk dialect
     version of this code */
  tok = '"' + JSON.parse("["+tok+"]")[0] + '"';
}

eof = false;

function nt() {
  if(to >= f.length) {
    eof = true;
    return false;
  }

  while(is_whitespace() || is_comment()) {
    if(is_whitespace()) {
      eat_whitespace();
    } else if(is_comment()) {
      eat_comment();
    }
  }

  if(is_id()) {
    get_id();
  } else if(is_num()) {
    get_num();
  } else if(is_other()) {
    get_other();
  } else if(is_char()) {
    get_char();
  } else if(is_string()) {
    get_string();
  } else {
    print("unsupported char");
    print("line: " + ln + " char: " + ch + " " +String.fromCharCode(ch));
    throw "error";
  }
  return true;
}

function skip(s) {
  if(tok == s) {
    nt();
    return;
  }
  /* anything else is an error */
  error();
}

function emit(s, l) {
  l.push(s);
}

globals_list = [];

function declare_global(t) {
  dprint("declare_global: " +t);
  emit(":GLOBAL_" , globals_list);
  emit(t, globals_list);
  emit("\nNULL\n", globals_list);
  globals.push(t);
  skip(";");
}

strings_list = [];

output_list = [];

function emit_out(s) {
  output_list.push(s);
}

function collect_arguments() {
  args = [];
  nt();
  while(tok != ")") {
    if(tok != ",") {
      args.push(tok);
    }
    nt();
  }
}


function to_hex_digit(a) {
  a = 15  & a;
  if(a > 9) {
    a = a - 10 + 65;
  } else {
    a = a + 48;
  }
  return a;
}

function to_hex_le(a) {
  var o;
  var i;
  i = 0;
  o = [];
  while(i < 4) {
    o[(i*2) + 1] = to_hex_digit(a);
    o[(i*2)] = to_hex_digit(a >>> 4);
    a = a >>> 8;
    i = i + 1;
  }
  return (o.map(function(x){return String.fromCharCode(x);})).join("");
}


function collect_local() {
  nt();
  locals.push(tok);
  emit_out("DEFINE LOCAL_");
  emit_out(tok);
  emit_out(" ");
/* FIXME clarify this calulation for local frame offset */
  emit_out(to_hex_le(-(1+args.length+locals.length + frame_bias)*4));
  emit_out("\n");
  emit_out("reserve_stack_slot\n");
  nt();
  skip(";");
  dprint("locals: " +locals);
}

function variable_load(a, is_arg) {
  emit_out("local ");
  if(is_arg == TRUE) {
    emit_out("ARG_");
  } else {
    emit_out("LOCAL_");
  }
  emit_out(a);
  emit_out(" ");
  if(char0() != "=") {
    emit_out("load ");
  }
}

function function_call(s) {
  var passed;
  passed = 0;
  dprint("function call");
  nt();
  emit_out("( ");
  if(tok != ")") {
    expression();
    emit_out("push_arg\n");
    passed = 1;
    while(tok == ",") {
      nt();
      expression();
      emit_out("push_arg\n");
      passed = passed + 1;
    }
  }
  skip(")");
  emit_out("do_call %FUNCTION_");
  emit_out(s);
  emit_out(" ");

  if(passed !=0) {
    emit_out("cleanup_args_bytes %");
    emit_out(int2str(4*passed, 10, TRUE));
    emit_out("\n");
  }
  emit_out(")\n");
}

function primary_expr_variable() {
  var s;
  var i;
  s = tok;
  nt();
  i = 0;
  while(i < locals.length) {
    if(locals[i]==s) {
      variable_load(s, FALSE);
      return;
    }
    i = i + 1;
  }
  i = 0;
  while(i < args.length) {
    if(args[i]==s) {
      variable_load(s, TRUE);
      return;
    }
    i = i + 1;
  }

  i = 0;
  while(i < globals.length) {
    if(globals[i]==s) {
      emit_out("global &GLOBAL_");
      emit_out(s);
      emit_out(" ");
      if(tok == "=") {
        return;
      }
      emit_out("load ");
      return;
    }
    i = i + 1;
  }

  if(tok == "(") {
    function_call(s);
  }
}

function primary_expr_number() {
  emit_out("constant %");
  emit_out(tok);
  emit_out(" ");
  nt();
}

function escape_lookup(c) {
  /* fixme this should do the correct char lookup */
  var c0;
  var c1;
  c0 = c[1];
  if(c0 == "\\") {
    c1 = c[2];
    if(c1 == 0) {
      return 0;
    } else if(c1 == 'a') {
      return 7;
    } else if(c1 == 'b') {
      return 8;
    } else if(c1 == 't') {
      return 9;
    } else if(c1 == 'n') {
      return 10;
    } else if(c1 == 'v') {
      return 11;
    } else if(c1 == 'f') {
      return 12;
    } else if(c1 == 'r') {
      return 13;
    } else if(c1 == 'e') {
      return 27;
    } else if(c1 == '"') {
      return 34;
    } else if(c1 == '\'') {
      return 39;
    } else if(c1 == '\\') {
      return 92;
    }
    error();
  }
  return c0.charCodeAt(0);
}


function primary_expr_char() {
  emit_out("constant %");
  emit_out(int2str(escape_lookup(tok), 1), 10, TRUE);
  emit_out(" ");
  nt();
}

function primary_expr_string() {
  var number_string;
  number_string = int2str(current_count, 10, TRUE);
  current_count = current_count + 1;
  emit(":STRING_", strings_list);
  uniqueID(number_string, strings_list);

  strings_list.push(tok);

  emit_out("constant &STRING_");
  uniqueID_out(number_string);

  nt();
}

function expression() {
  dprint("expression");
  if(char0() == "(") {
    nt();
    expression();
    skip(")");
  } else if(tt == "identifier") {
    primary_expr_variable();
  } else if(tt == "number") {
    primary_expr_number();
  } else if(tt == "char") {
    primary_expr_char();
  } else if(tt == "string") {
    primary_expr_string();
  } else {
    error();
  }

  if(char0() == "=") {
    emit_out("push_address\n");
    nt();
    expression();
    emit_out("store\n");
  }
}

function int2str(a) {
  return a.toString(10);
}

function cleanup_locals() {
  var c;
  c = locals.length;
  if(c > 0) {
    emit_out("cleanup_locals_bytes %");
    emit_out(int2str(4*c, 10 , TRUE));
    emit_out(" ");
  }
}

function return_result() {
  nt();
  if(tok != ";") {
    expression();
  }
  cleanup_locals();
  emit_out("ret\n");
  nt();
}

function uniqueID(id, l) {
  emit(func, l);
  emit("_", l);
  emit(int2str(id, 10, TRUE), l);
  emit("\n", l);
}

function uniqueID_out(id) {
  uniqueID(id, output_list);
}

function process_while() {
  var number_string;
  var nested_break_num;
  nested_break_num = break_target_num;
  dprint("process_while");
  number_string = int2str(current_count, 10, TRUE);
  current_count = current_count + 1;
  break_target_num = number_string;
  emit_out(":WHILE_");
  uniqueID_out(number_string);
  nt();
  skip("(");
  expression();
  emit_out("jump_false %END_WHILE_");
  uniqueID_out(number_string);
  emit_out("# THEN_while_");
  uniqueID_out(number_string);
  nt(); /* skip ) */
  statement();
  emit_out("jump %WHILE_");
  uniqueID_out(number_string);
  emit_out(":END_WHILE_");
  uniqueID_out(number_string);
  break_target_num = nested_break_num;
}

function process_if() {
  var number_string;
  dprint("process_if");
  number_string = int2str(current_count, 10, TRUE);
  current_count = current_count + 1;
  emit_out("# IF_");
  uniqueID_out(number_string);
  nt();
  skip("(");
  expression();
  emit_out("jump_false %ELSE_");
  uniqueID_out(number_string);
  skip(")");
  statement();
  emit_out("jump %_END_IF_");
  uniqueID_out(number_string);
  emit_out(":ELSE_");
  uniqueID_out(number_string);
  if(tok == "else") {
    nt();
    statement();
  }
  emit_out(":_END_IF_");
  uniqueID_out(number_string);
}

function process_break() {
  nt();
  emit_out("jump %");
  emit_out(break_target_prefix);
  emit_out(func);
  emit_out("_");
  emit_out(break_target_num);
  emit_out("\n");
  skip(";");
}

function statement() {
  if(tok == "{") {
    dprint("lcurly");
    nt();
    while(tok != "}") {
      statement();
    }
    skip("}");
    dprint("rcurly");
  } else if(tok == "var" || tok == "int") {
    collect_local();
  } else if(tok == "if") {
    process_if();
  } else if(tok == "while") {
    process_while();
  } else if(tok == "asm") {
    process_asm();
  } else if(tok == "return") {
    return_result();
  } else if(tok == "break") {
    process_break();
  } else {
    expression();
    skip(";");
  }
}

function declare_function(t) {
  var i;
  locals = [];
  dprint("declare_function: " +t);
  current_count = 0;
  func = t;
  if(t == "main") {
    frame_bias = 1;
  } else {
    frame_bias = 0;
  }
  collect_arguments();
  nt();
  if(tok == ";") {
    dprint("function_prototype skip");
  } else if(tok == "{") {
    dprint("function_body");
    emit_out(":FUNCTION_");
    emit_out(t);
    emit_out("\n");
    for(i = args.length - 1; i > -1; i = i - 1) {
      emit_out("DEFINE ARG_");
      emit_out(args[i]);
      emit_out(" ");
      /* FIXME explain this frame layout better */
      emit_out(to_hex_le(-(i+1)*4));
      emit_out("\n");
    }
    statement();
    if(output_list[output_list.length-1] !== "ret\n") {
      cleanup_locals();
      emit_out("ret\n");
    }
  }
}

function error() {
  print("line: " + ln + " tok: " + tok);
  exit(1);
}


function program() {
  globals = [];
  var ltok;

  nc();

  nt();

  while(!eof) {
    if((tok == "int") || (tok == "var") || (tok == "function")) {
      nt(); ltok = tok;
      nt();

      if(tok == ";") {
        declare_global(ltok);
      } else if(tok == "(") {
        declare_function(ltok);
      } else {
        error();
      }
    } else {
      error();
    }
  }
}

function init_globals() {
  break_target_prefix = "END_WHILE_";
}

init_globals();
program();
print("\n# Core program");
print(output_list.join(""));
print("# Program global variables");
print(globals_list.join(""));
print("# Program strings");
print(strings_list.join(""));
print(":ELF_end");
