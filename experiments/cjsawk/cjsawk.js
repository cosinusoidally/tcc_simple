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

var indent;
var no_indent;

var to;
var ch;

var ln;

var current_count;

var tt;

var tt_identifier;
var tt_number;
var tt_char;
var tt_string;
var tt_other;

var tok;

var break_target_prefix;
var break_target_num;

/* needed since main has an additional number of elements initially on the
   stack */
var frame_bias;

var eof;

var strings_list;
var output_list;
var globals_list;
var args;
var locals;

var hold_string;

var ra_sizeof;
var ra_capacity_o;
var ra_len_o;
var ra_data_o;

function ri32(o) {
  return or(or(and(ri8(o), 255),
            shl(and(ri8(add(o, 1)), 255), 8)),
            or(shl(and(ri8(add(o, 2)), 255), 16),
            shl(and(ri8(add(o, 3)), 255), 24)));
}

function wi32(o, v) {
  wi8(o, and(v, 255));
  v = shr(v, 8);
  wi8(add(o, 1), and(v, 255));
  v = shr(v, 8);
  wi8(add(o, 2), and(v, 255));
  v = shr(v, 8);
  wi8(add(o, 3), and(v, 255));
}

function match(a, b) {
  var i;
  if(and(eq(NULL, a), eq(NULL, b))) {
    return TRUE;
  }
  if(eq(NULL, a)) {
    return FALSE;
  }
  if(eq(NULL, b)) {
    return FALSE;
  }

  i = sub(0, 1);
  while(1) {
    i = add(i, 1);
    if(neq(ri8(add(a, i)), ri8(add(b, i)))) {
      return FALSE;
    }
    if(eq(0,and(neq(0, ri8(add(a,i))), neq(0, ri8(add(b, i)))))){
      break;
    }
  }
  return TRUE;
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
  ch = v_getc();
  to = add(to, 1);
  if(eq(ch, 10)) {
    ln = add(ln, 1);
  }
}

function nch() {
  ra_push8(hold_string, ch);
  nc();
}

function char0() {
  return ri8(tok);
}

function is_whitespace() {
  if(or(eq(ch, 32), eq(ch, 9))) {
    return TRUE;
  }

  if(eq(ch, 10)) {
    return TRUE;
  }

  return FALSE;
}

function eat_whitespace() {
  while(is_whitespace()) {
    nc();
  }
}

function is_comment() {
  if(eq(ch, mkc('/'))) {
    return TRUE;
  }
  return FALSE;
}

function eat_comment() {
  if(eq(ch, mkc('/'))) {
    nc();
    if(eq(ch, mkc('*'))) {
      nc();
    } else {
      return;
    }
    while(1) {
      while(neq(ch, mkc('*'))) {
        nc();
      }
      nc();
      if(eq(ch, mkc('/'))) {
        nc();
        return;
      }
    }
  }
}

function is_id() {
  if(or(and(gte(ch, mkc('a')), lte(ch, mkc('z'))),
     or(and(gte(ch, mkc('A')), lte(ch, mkc('Z'))), eq(ch, mkc('_'))))) {
    return TRUE;
  }
  return FALSE;
}

function is_num() {
  if(and(gte(ch, mkc('0')), lte(ch,mkc('9')))) {
    return TRUE;
  }
  return FALSE;
}

function is_other() {
  if(or(eq(ch,mkc(',')),or(eq(ch,mkc(';')),or(eq(ch,mkc('(')),
     or(eq(ch,mkc(')')),or(eq(ch,mkc('{')),or(eq(ch,mkc('}')),
     eq(ch,mkc('='))))))))) {
    return TRUE;
  }
  return FALSE;
}

function is_char() {
  if(eq(ch, mkc('\''))) {
    return TRUE;
  }
  return FALSE;
}

function is_string() {
  if(eq(ch, mkc('"'))) {
    return TRUE;
  }
  return FALSE;
}

function get_id() {
  tt = tt_identifier;
  while(or(is_num(), is_id())) {
    nch();
  }
}

function get_num() {
  tt = tt_number;
  while(is_num()) {
    nch();
  }
}

function get_other() {
  tt = tt_other;
  nch();
}

function get_char() {
  tt = tt_char;
  nch();
  if(eq(ch, mkc('\\'))) {
    nch();
  }
  nch();
  nc();
}

function get_string() {
  tt = tt_string;
  nch();
  while(neq(ch, mkc('"'))) {
    nch();
  }
  nch();
  tok = ra_data_g(hold_string);
  /* FIXME hack to parse escape codes from JS, replace with cjsawk dialect
     version of this code */
  tok = mks('"' + JSON.parse("["+mk_js_string(tok)+"]")[0]);
}

function nt() {
  /* FIXME shouldn't use f_len, nc should indicate if eof */
  if(gte(to, f_len)) {
    eof = TRUE;
    return FALSE;
  }

  while(or(is_whitespace(), is_comment())) {
    if(is_whitespace()) {
      eat_whitespace();
    } else if(is_comment()) {
      eat_comment();
    }
  }

/* FIXME probably memory inefficient */
//  ra_reset(hold_string);
  hold_string = ra_new();

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
    return TRUE;
  } else {
    print("unsupported char");
    print("line: " + ln + " char: " + ch + " " +String.fromCharCode(ch));
    throw "error";
  }

//  print("hold_string: "+ mk_js_string(ra_data_g(hold_string)));
  tok = ra_data_g(hold_string);
  return TRUE;
}

function skip(s) {
  if(match(tok, s)) {
    nt();
    return;
  }
  /* anything else is an error */
  error();
}

function emit(s, l) {
  ra_push32(l, s);
}

function declare_global(t) {
  dprint("declare_global: " +t);
  emit(mks(":GLOBAL_") , globals_list);
  emit(t, globals_list);
  emit(mks("\nNULL\n"), globals_list);
  skip(mks(";"));
}

function emit_out(s) {
  emit(s, output_list);
}

/* resizable arrays */
function ra_new() {
  var o;
  var t = {};
  o = calloc(ra_sizeof, 1);
  ra_capacity_s_(o, 4);
  t.obj = o;
  t.capacity = 4;
  t.len = 0;
  t.data_raw = calloc(t.capacity, 1);
  return t;
}

function ra_capacity_g(r) {
  return r.capacity;
}

function ra_capacity_s(r, v) {
  r.capacity = v;
}

function ra_capacity_g_(r) {
  return ri32(add(r,ra_capacity_o));
}

function ra_capacity_s_(r, v) {
  wi32(add(r,ra_capacity_o), v);
}

function ra_grow(r) {
  var sd;
  var dd;
  var c1;
  var c2;
  var o;
  c1 = ra_capacity_g(r);
  c2 = mul(c1, 2);
  dd = calloc(c2,1);
  sd = r.data_raw;
  o = 0;
  while(lt(o, c1)) {
    wi8(add(dd,o),ri8(add(sd,o)));
    o = add(o,1);
  }
  r.data_raw = dd;
  ra_capacity_s(r,c2);
  free(sd);
}

function ra_reset(r) {
  r.len = 0;
}

function ra_push32(r, v) {
  if(gt(add(r.len, 4), ra_capacity_g(r))) {
    ra_grow(r);
  }
  wi32(add(r.data_raw, r.len), v);
  r.len = add(r.len, 4);
}

function ra_push8(r, v) {
  if(gt(add(r.len, 4), ra_capacity_g(r))) {
    ra_grow(r);
  }
  wi32(add(r.data_raw, r.len), and(v, 255));
  r.len = add(r.len, 1);
}

function ra_get32(r, o) {
  return ri32(add(r.data_raw, mul(o,4)));
}

function ra_len32(r) {
  return div(r.len, 4);
}

function ra_data_g(r) {
  return r.data_raw;
}

function collect_arguments() {
  ra_reset(args);
  nt();
  while(eq(0, match(tok, mks(")")))) {
    if(eq(0, match(tok, mks(",")))) {
      ra_push32(args, tok);
    }
    nt();
  }
}


function to_hex_digit(a) {
  a = and(15, a);
  if(gt(a, 9)) {
    a = add(sub(a, 10), 65);
  } else {
    a = add(a, 48);
  }
  return a;
}

function to_hex_le(a) {
  var o;
  var i;
  var d1o;
  var d2o;
  i = 0;
  o = calloc(17, 1);
  while(lt(i, 4)) {
    d2o = mul(i, 2);
    d1o = add(d2o, 1);
    wi8(add(o, d1o), to_hex_digit(a));
    wi8(add(o, d2o), to_hex_digit(shr(a, 4)));
    a = shr(a, 8);
    i = add(i, 1);
  }
  return o;
}

function collect_local() {
  nt();
  ra_push32(locals,tok);
  indented_emit_out(mks("DEFINE LOCAL_"));
  emit_out(tok);
  emit_out(mks(" "));
/* FIXME clarify this calulation for local frame offset */
  emit_out(to_hex_le(sub(0,mul(add(1,add(add(ra_len32(args),ra_len32(locals)),frame_bias)),4))));
  emit_out(mks("\n"));
  indented_emit_out(mks("reserve_stack_slot\n"));
  nt();
  skip(mks(";"));
  dprint("locals: " +locals);
}

function variable_load(a, is_arg) {
  indented_emit_out(mks("local "));
  if(eq(is_arg, TRUE)) {
    emit_out(mks("ARG_"));
  } else {
    emit_out(mks("LOCAL_"));
  }
  emit_out(a);
  emit_out(mks(" "));
  if(neq(char0(), mkc('='))) {
    no_indent = 1;
    emit_out(mks("load "));
  }
}

function function_call(s) {
  var passed;
  passed = 0;
  dprint("function call");
  nt();

  indented_emit_out(mks("("));
  increase_indent();

  if(eq(0, match(tok, mks(")")))) {
    emit_out(mks(" ")); no_indent = 1;
    expression();
    indented_emit_out(mks("push_arg\n"));
    passed = 1;
    while(match(tok, mks(","))) {
      nt();
      expression();
      indented_emit_out(mks("push_arg\n"));
      passed = add(passed, 1);
    }
  } else {
    emit_out(mks(" ")); no_indent = 1;
  }
  skip(mks(")"));
  indented_emit_out(mks("do_call %FUNCTION_"));
  emit_out(s);
  emit_out(mks(" "));

  if(neq(passed, 0)) {
    emit_out(mks("cleanup_args_bytes %"));
    emit_out(int_str(mul(4, passed)));
    emit_out(mks("\n"));
  } else {
    no_indent = 1;
  }
  decrease_indent();
  indented_emit_out(mks(")\n"));
}

function primary_expr_variable() {
  var s;
  var i;
  s = tok;
  nt();
  i = 0;

  if(match(tok, mks("("))) {
    return function_call(s);
  }

  while(lt(i, ra_len32(locals))) {
    if(match(ra_get32(locals, i), s)) {
      variable_load(s, FALSE);
      return;
    }
    i = add(i, 1);
  }
  i = 0;
  while(lt(i, ra_len32(args))) {
    if(match(ra_get32(args, i), s)) {
      variable_load(s, TRUE);
      return;
    }
    i = add(i, 1);
  }


  /* otherwise assume is a global */
  indented_emit_out(mks("global &GLOBAL_"));
  emit_out(s);
  emit_out(mks(" "));
  if(match(tok, mks("="))) {
    return;
  }
  no_indent = 1;
  emit_out(mks("load "));
}

function primary_expr_number() {
  indented_emit_out(mks("constant %"));
  emit_out(tok);
  emit_out(mks(" ")); no_indent = 1;
  nt();
}

function escape_lookup(c) {
  /* fixme this should do the correct char lookup */
  var c0;
  var c1;
  c0 = ri8(add(c, 1));
  if(eq(c0, mkc('\\'))) {
    c1 = ri8(add(c, 2));
    if(eq(c1, 0)) {
      return 0;
    } else if(eq(c1, mkc('a'))) {
      return 7;
    } else if(eq(c1, mkc('b'))) {
      return 8;
    } else if(eq(c1, mkc('t'))) {
      return 9;
    } else if(eq(c1, mkc('n'))) {
      return 10;
    } else if(eq(c1, mkc('v'))) {
      return 11;
    } else if(eq(c1, mkc('f'))) {
      return 12;
    } else if(eq(c1, mkc('r'))) {
      return 13;
    } else if(eq(c1, mkc('e'))) {
      return 27;
    } else if(eq(c1, mkc('"'))) {
      return 34;
    } else if(eq(c1, mkc('\''))) {
      return 39;
    } else if(eq(c1, mkc('\\'))) {
      return 92;
    }
    error();
  }
  return c0;
}


function primary_expr_char() {
  emit_out(mks("constant %"));
  emit_out(int_str(escape_lookup(tok), 1));
  emit_out(mks(" "));
  nt();
}

function primary_expr_string() {
  var number_string;
  number_string = int_str(current_count);
  current_count = add(current_count, 1);

  emit(mks(":STRING_"), strings_list);
  uniqueID(number_string, strings_list);

  emit(tok, strings_list);
  emit(mks("\"\n"), strings_list);

  indented_emit_out(mks("constant &STRING_"));
  uniqueID_out(number_string);

  nt();
}

function expression() {
  dprint("expression");
  if(eq(char0(), mkc('('))) {
    nt();
    expression();
    skip(mks(")"));
  } else if(eq(tt, tt_identifier)) {
    primary_expr_variable();
  } else if(eq(tt, tt_number)) {
    primary_expr_number();
  } else if(eq(tt, tt_char)) {
    primary_expr_char();
  } else if(eq(tt, tt_string)) {
    primary_expr_string();
  } else {
    error();
  }

  if(eq(char0(), mkc('='))) {
    emit_out(mks("push_address\n"));
    nt();
    expression();
    indented_emit_out(mks("store\n"));
  }
}

function int_str(a) {
  var b;
  var d;
  var o;
  var t;
  if(typeof a != "number") {
    print("not number: "+a);
    error();
  }
  if(eq(a,0)) {
    return mks("0");
  }
  b = calloc(34,1);
  o = add(b,32);
  while(1) {
    t = mod(a,10);
    wi8(o,add(48,t));
    a = div(sub(a,t),10);
    if(eq(0,a)){
      break;
    }
    o = sub(o,1);
  }
  return o;
}

function cleanup_locals() {
  var c;
  c = ra_len32(locals);
  if(gt(c, 0)) {
    indented_emit_out(mks("cleanup_locals_bytes %"));
    emit_out(int_str(mul(4, c)));
    emit_out(mks(" "));
    no_indent = 1;
  }
}

function return_result() {
  nt();
  if(eq(0, match(tok, mks(";")))) {
    expression();
  }
  cleanup_locals();
  indented_emit_out(mks("ret\n"));
  nt();
}

function uniqueID(id, l) {
  emit(func, l);
  emit(mks("_"), l);
  emit(id, l);
  emit(mks("\n"), l);
}

function uniqueID_out(id) {
  uniqueID(id, output_list);
}

function process_while() {
  var number_string;
  var nested_break_num;
  nested_break_num = break_target_num;
  dprint("process_while");
  number_string = int_str(current_count);
  current_count = add(current_count, 1);
  break_target_num = number_string;
  emit_out(mks(":WHILE_"));
  uniqueID_out(number_string);
  nt();
  skip(mks("("));
  expression();
  indented_emit_out(mks("jump_false %END_WHILE_"));
  uniqueID_out(number_string);
  emit_out(mks("# THEN_while_"));
  uniqueID_out(number_string);
  nt(); /* skip ) */
  statement();
  indented_emit_out(mks("jump %WHILE_"));
  uniqueID_out(number_string);
  emit_out(mks(":END_WHILE_"));
  uniqueID_out(number_string);
  break_target_num = nested_break_num;
}

function process_if() {
  var number_string;
  dprint("process_if");
  number_string = int_str(current_count);
  current_count = add(current_count, 1);
  emit_out(mks("# IF_"));
  uniqueID_out(number_string);
  nt();
  skip(mks("("));
  expression();
  indented_emit_out(mks("jump_false %ELSE_"));
  uniqueID_out(number_string);
  skip(mks(")"));
  statement();
  indented_emit_out(mks("jump %_END_IF_"));
  uniqueID_out(number_string);
  emit_out(mks(":ELSE_"));
  uniqueID_out(number_string);
  if(match(tok, mks("else"))) {
    nt();
    statement();
  }
  emit_out(mks(":_END_IF_"));
  uniqueID_out(number_string);
}

function process_break() {
  nt();
  indented_emit_out(mks("jump %"));
  emit_out(break_target_prefix);
  emit_out(func);
  emit_out(mks("_"));
  emit_out(break_target_num);
  emit_out(mks("\n"));
  skip(mks(";"));
}

function process_asm() {
  nt();
  skip(mks("("));
  while(eq(char0(), mkc('"'))) {
    /* strip off leading quote */
    emit_out(add(tok, 1));
    emit_out(mks("\n"));
    nt();
  }
  skip(mks(")"));
  skip(mks(";"));
}

function statement() {
  if(match(tok, mks("{"))) {
    dprint("lcurly");
    nt();
    while(eq(0, match(tok, mks("}")))) {
      statement();
    }
    skip(mks("}"));
    dprint("rcurly");
  } else if(or(match(tok, mks("var")), match(tok, mks("int")))) {
    collect_local();
  } else if(match(tok, mks("if"))) {
    process_if();
  } else if(match(tok, mks("while"))) {
    process_while();
  } else if(match(tok, mks("asm"))) {
    process_asm();
  } else if(match(tok, mks("return"))) {
    return_result();
  } else if(match(tok, mks("break"))) {
    process_break();
  } else {
    expression();
    skip(mks(";"));
  }
}

function declare_function(t) {
  var i;
  ra_reset(locals);
  dprint("declare_function: " +t);
  current_count = 0;
  func = t;
  if(match(t, mks("main"))) {
    frame_bias = 1;
  } else {
    frame_bias = 0;
  }
  collect_arguments();
  nt();
  if(match(tok, mks(";"))) {
    dprint("function_prototype skip");
  } else if(match(tok, mks("{"))) {
    dprint("function_body");
    emit_out(mks(":FUNCTION_"));
    emit_out(func);
    increase_indent();
    emit_out(mks("\n"));
    i = sub(ra_len32(args), 1);
    while(gt(i, sub(0,1))) {
      indented_emit_out(mks("DEFINE ARG_"));
      emit_out(ra_get32(args, i));
      emit_out(mks(" "));
      /* FIXME explain this frame layout better */
      emit_out(to_hex_le(sub(0,mul(add(i,1),4))));
      emit_out(mks("\n"));
      i = sub(i, 1);
    }
    statement();
    if(eq(0, match(ra_get32(output_list, sub(ra_len32(output_list),1)), mks("ret\n")))) {
      cleanup_locals();
      indented_emit_out(mks("ret\n"));
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

  while(eq(0,eof)) {
    if(or(match(tok, mks("int")), or(match(tok, mks("var")),
          match(tok, mks("function"))))) {
      nt(); ltok = tok;
      nt();

      if(match(tok, mks(";"))) {
        declare_global(ltok);
      } else if(match(tok, mks("("))) {
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
  args = ra_new();
  locals = ra_new();
  globals_list = ra_new();
  strings_list = ra_new();
  output_list = ra_new();
  hold_string = ra_new();

  indent = 0;
  no_indent = 0;

  to=0;

  ln=1;

  tt_identifier = mks("identifier");
  tt_number = mks("number");
  tt_char = mks("char");
  tt_string = mks("string");
  tt_other = mks("other");

  break_target_prefix = mks("END_WHILE_");

  eof = FALSE;

  ra_capacity_o = 0;
  ra_len_o = 4;
  ra_data_o = 8;
  ra_sizeof = 12;
}

function join_list(l) {
  var o;
  var i;
  var len;
  o = [];
  i = 0;
  len = ra_len32(l);
  while (lt(i,len)) {
    o.push(mk_js_string(ra_get32(l, i)));
    i = add(i,1);
  }
  return o.join("");
}

function main() {
  init_globals();
  program();
  print("\n# Core program");
  print(join_list(output_list));
  print("# Program global variables");
  print(join_list(globals_list));
  print("# Program strings");
  print(join_list(strings_list));
  print(":ELF_end");
}
