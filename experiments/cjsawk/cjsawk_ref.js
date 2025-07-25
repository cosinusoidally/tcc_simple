/* cjsawk partially based on a heavily cut down version of M2-Planet
 * Modifications Copyright (C) 2025 Liam Wilson (under same license)
 * original authors of M2-Planet and original copyright notice:
 * Copyright (C) 2016 Jeremiah Orians
 * Copyright (C) 2018 Jan (janneke) Nieuwenhuizen <janneke@gnu.org>
 * Copyright (C) 2020 deesix <deesix@tuta.io>
 * FIXME (Liam Wilson) I've mangled andrius name as gawk doesn't like the "S"
 * Copyright (C) 2021 Andrius Stikonas <andrius@stikonas.eu>
 * Copyright (C) 2021 Sanne Wouda
 * Copyright (C) 2022 Jan (janneke) Nieuwenhuizen <janneke@gnu.org>
 * This file is part of M2-Planet.
 *
 * M2-Planet is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * M2-Planet is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with M2-Planet.  If not, see <http://www.gnu.org/licenses/>.
 */

// f=read("test.c","binary");
// f=read("../m2min_v3/m2min_v3.js","binary");
f=read("./artifacts/m2min_v3.c","binary");

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

var indent = 0;
var no_indent = 0;

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


function add(a,b) {
  return a + b;
}

function sub(a,b) {
  return a - b;
}

function mks(s) {
  // dummy impl
  return s;
}

function lt(a,b) {
  return a < b;
}

function indented_emit_out(s) {
  var c;
  c = 0;
  if(no_indent) {
    no_indent = 0;
  } else {
    while(lt(c, indent)) {
      emit_out(mks(" "));
      c = add(c, 1);
    }
  }
  emit_out(s);
}

function decrease_indent() {
        indent = sub(indent, 2);
        if(lt(indent,0)) {
                indent = 0;
        }
}

function increase_indent() {
  indent = add(indent, 2);
}

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
  indented_emit_out("DEFINE LOCAL_");
  emit_out(tok);
  emit_out(" ");
/* FIXME clarify this calulation for local frame offset */
  emit_out(to_hex_le(-(1+args.length+locals.length + frame_bias)*4));
  emit_out("\n");
  indented_emit_out("reserve_stack_slot\n");
  nt();
  skip(";");
  dprint("locals: " +locals);
}

function variable_load(a, is_arg) {
  indented_emit_out("local ");
  if(is_arg == TRUE) {
    emit_out("ARG_");
  } else {
    emit_out("LOCAL_");
  }
  emit_out(a);
  emit_out(" ");
  if(char0() != "=") {
    no_indent = 1;
    emit_out("load ");
  }
}

function function_call(s) {
  var passed;
  passed = 0;
  dprint("function call");
  nt();

  indented_emit_out("(");
  increase_indent();

  if(tok != ")") {
    emit_out(mks(" ")); no_indent = 1;
    expression();
    indented_emit_out("push_arg\n");
    passed = 1;
    while(tok == ",") {
      nt();
      expression();
      indented_emit_out("push_arg\n");
      passed = passed + 1;
    }
  } else {
    emit_out(mks(" ")); no_indent = 1;
  }
  skip(")");
  indented_emit_out("do_call %FUNCTION_");
  emit_out(s);
  emit_out(" ");

  if(passed !=0) {
    emit_out("cleanup_args_bytes %");
    emit_out(int2str(4*passed, 10, TRUE));
    emit_out("\n");
  } else {
    no_indent = 1;
  }
  decrease_indent();
  indented_emit_out(")\n");
}

function primary_expr_variable() {
  var s;
  var i;
  s = tok;
  nt();
  i = 0;

  if(tok == "(") {
    return function_call(s);
  }

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


  /* otherwise assume is a global */
  indented_emit_out("global &GLOBAL_");
  emit_out(s);
  emit_out(" ");
  if(tok == "=") {
    return;
  }
  no_indent = 1;
  emit_out("load ");
}

function primary_expr_number() {
  indented_emit_out("constant %");
  emit_out(tok);
  emit_out(" "); no_indent = 1;
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

  emit(tok, strings_list);
  emit("\n", strings_list);

  indented_emit_out("constant &STRING_");
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
    indented_emit_out("store\n");
  }
}

function int2str(a) {
  return a.toString(10);
}

function cleanup_locals() {
  var c;
  c = locals.length;
  if(c > 0) {
    indented_emit_out("cleanup_locals_bytes %");
    emit_out(int2str(4*c, 10 , TRUE));
    emit_out(" ");
    no_indent = 1;
  }
}

function return_result() {
  nt();
  if(tok != ";") {
    expression();
  }
  cleanup_locals();
  indented_emit_out("ret\n");
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
  indented_emit_out("jump_false %END_WHILE_");
  uniqueID_out(number_string);
  emit_out("# THEN_while_");
  uniqueID_out(number_string);
  nt(); /* skip ) */
  statement();
  indented_emit_out("jump %WHILE_");
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
  indented_emit_out("jump_false %ELSE_");
  uniqueID_out(number_string);
  skip(")");
  statement();
  indented_emit_out("jump %_END_IF_");
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
  indented_emit_out("jump %");
  emit_out(break_target_prefix);
  emit_out(func);
  emit_out("_");
  emit_out(break_target_num);
  emit_out("\n");
  skip(";");
}

function process_asm() {
  nt();
  skip("(");
  while(char0() == '"') {
/* FIXME strip off quotes in a non-js way */
    emit_out(tok.slice(1,-1));
    emit_out("\n");
    nt();
  }
  skip(")");
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
    increase_indent();
    emit_out("\n");
    for(i = args.length - 1; i > -1; i = i - 1) {
      indented_emit_out("DEFINE ARG_");
      emit_out(args[i]);
      emit_out(" ");
      /* FIXME explain this frame layout better */
      emit_out(to_hex_le(-(i+1)*4));
      emit_out("\n");
    }
    statement();
    if(output_list[output_list.length-1] !== "ret\n") {
      cleanup_locals();
      indented_emit_out("ret\n");
    }
    decrease_indent();
  }
}

function error() {
  print("line: " + ln + " tok: " + tok);
  exit(1);
}


function program() {
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
