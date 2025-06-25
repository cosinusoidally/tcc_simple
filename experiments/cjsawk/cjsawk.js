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

var indent = 0;
var no_indent = 0;

to=0;
ts=0;
te=0;
var ch;

ln=1;

var current_count;

var tt;

tt_identifier = mks_("identifier");
tt_number = mks_("number");
tt_char = mks_("char");
tt_string = mks_("string");
tt_other = mks_("other");

var tok;

var break_target_prefix;
var break_target_num;

/* needed since main has an additional number of elements initially on the
   stack */
var frame_bias;

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

function match_(a, b) {
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
      emit_out(mks_(" "));
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
  to = add(to, 1);
  if(eq(ch, 10)) {
    ln = add(ln, 1);
  }
}

function char0() {
  return ri8(tok_);
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

function f_str() {
  var i;
  var l;
  var b2;
  l = sub(te , ts);
  b2 = malloc(add(l, 1));
  i = ts;
  while(lt(i,te)) {
    wi8(add(b2, sub(i, ts)), f[i]);
    i = add(i, 1);
  }
  tok = mk_js_string(b2);
  tok_ = b2;
}

function get_id() {
  tt = tt_identifier;
  ts = sub(to, 1);
  while(or(is_num(), is_id())) {
    te = to;
    nc();
  }
  f_str();
}

function get_num() {
  tt = tt_number;
  ts = sub(to, 1);
  while(is_num()) {
    te = to;
    nc();
  }
  f_str();
}

function get_other() {
  tt = tt_other;
  ts = sub(to, 1);
  te = to;
  nc();
  f_str();
}

function get_char() {
  tt = tt_char;
  ts = sub(to, 1);
  nc();
  if(eq(ch, mkc('\\'))) {
    nc();
  }
  nc();
  te = to;
  nc();
  f_str();
}

function get_string() {
  tt = tt_string;
  ts = sub(to, 1);
  nc();
  while(neq(ch, mkc('"'))) {
    nc();
  }
  te = to;
  nc();
  f_str();
  /* FIXME hack to parse escape codes from JS, replace with cjsawk dialect
     version of this code */
  tok = '"' + JSON.parse("["+tok+"]")[0] + '"';
  tok_ = mks_(tok);
}

eof = FALSE;

function nt() {
  if(gte(to, f.length)) {
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
  if(match_(tok_, s)) {
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
  emit(mks_(":GLOBAL_") , globals_list);
  emit(t, globals_list);
  emit(mks_("\nNULL\n"), globals_list);
  skip(mks_(";"));
}

strings_list = [];

output_list = [];

function emit_out(s) {
  output_list.push(s);
}

/* resizable arrays */
function ra_new() {
  var t = {};
  t.capacity = 4;
  t.len = 4;
  t.data_raw = calloc(t.capacity, 1);
  return t;
}

function ra_capacity_g(r) {
  return r.capacity;
}

function ra_capacity_s(r, v) {
  r.capacity = v;
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


function args_reset() {
  ra_reset(args_);
}

function args_push32(v) {
  ra_push32(args_, v);
}

function args_get32(o) {
  return ri32(add(args_.data_raw, mul(o,4)));
}

function args_len32() {
  return div(args_.len, 4);
}

function collect_arguments() {
  args_reset();
  nt();
  while(eq(0, match_(tok_, mks_(")")))) {
    if(eq(0, match_(tok_, mks_(",")))) {
      args_push32(tok_);
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

function locals_push(s) {
  locals.push(s);
}

function collect_local() {
  nt();
  locals_push(tok_);
  indented_emit_out(mks_("DEFINE LOCAL_"));
  emit_out(tok_);
  emit_out(mks_(" "));
/* FIXME clarify this calulation for local frame offset */
  emit_out(to_hex_le(sub(0,mul(add(1,add(add(args_len32(),locals.length),frame_bias)),4))));
  emit_out(mks_("\n"));
  indented_emit_out(mks_("reserve_stack_slot\n"));
  nt();
  skip(mks_(";"));
  dprint("locals: " +locals);
}

function variable_load(a, is_arg) {
  indented_emit_out(mks_("local "));
  if(eq(is_arg, TRUE)) {
    emit_out(mks_("ARG_"));
  } else {
    emit_out(mks_("LOCAL_"));
  }
  emit_out(a);
  emit_out(mks_(" "));
  if(neq(char0(), mkc('='))) {
    no_indent = 1;
    emit_out(mks_("load "));
  }
}

function function_call(s) {
  var passed;
  passed = 0;
  dprint("function call");
  nt();

  indented_emit_out(mks_("("));
  increase_indent();

  if(eq(0, match_(tok_, mks_(")")))) {
    emit_out(mks_(" ")); no_indent = 1;
    expression();
    indented_emit_out(mks_("push_arg\n"));
    passed = 1;
    while(match_(tok_, mks_(","))) {
      nt();
      expression();
      indented_emit_out(mks_("push_arg\n"));
      passed = add(passed, 1);
    }
  } else {
    emit_out(mks_(" ")); no_indent = 1;
  }
  skip(mks_(")"));
  indented_emit_out(mks_("do_call %FUNCTION_"));
  emit_out(s);
  emit_out(mks_(" "));

  if(neq(passed, 0)) {
    emit_out(mks_("cleanup_args_bytes %"));
    emit_out(int_str(mul(4, passed)));
    emit_out(mks_("\n"));
  } else {
    no_indent = 1;
  }
  decrease_indent();
  indented_emit_out(mks_(")\n"));
}

function primary_expr_variable() {
  var s;
  var i;
  s = tok_;
  nt();
  i = 0;

  if(match_(tok_, mks_("("))) {
    return function_call(s);
  }

  while(lt(i, locals.length)) {
    if(match_(locals[i], s)) {
      variable_load(s, FALSE);
      return;
    }
    i = add(i, 1);
  }
  i = 0;
  while(lt(i, args_len32())) {
    if(match_(args_get32(i), s)) {
      variable_load(s, TRUE);
      return;
    }
    i = add(i, 1);
  }


  /* otherwise assume is a global */
  indented_emit_out(mks_("global &GLOBAL_"));
  emit_out(s);
  emit_out(mks_(" "));
  if(match_(tok_, mks_("="))) {
    return;
  }
  no_indent = 1;
  emit_out(mks_("load "));
}

function primary_expr_number() {
  indented_emit_out(mks_("constant %"));
  emit_out(tok_);
  emit_out(mks_(" ")); no_indent = 1;
  nt();
}

function escape_lookup(c) {
  /* fixme this should do the correct char lookup */
  var c0;
  var c1;
  c0 = mkc(c[1]);
  if(eq(c0, mkc('\\'))) {
    c1 = mkc(c[2]);
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
  emit_out(mks_("constant %"));
  emit_out(int_str(escape_lookup(tok), 1));
  emit_out(mks_(" "));
  nt();
}

function primary_expr_string() {
  var number_string;
  number_string = int_str(current_count);
  current_count = add(current_count, 1);
  emit(mks_(":STRING_"), strings_list);
  uniqueID(number_string, strings_list);

  emit(tok_, strings_list);
  emit(mks_("\n"), strings_list);

  indented_emit_out(mks_("constant &STRING_"));
  uniqueID_out(number_string);

  nt();
}

function expression() {
  dprint("expression");
  if(eq(char0(), mkc('('))) {
    nt();
    expression();
    skip(mks_(")"));
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
    emit_out(mks_("push_address\n"));
    nt();
    expression();
    indented_emit_out(mks_("store\n"));
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
    return mks_("0");
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
  c = locals.length;
  if(gt(c, 0)) {
    indented_emit_out(mks_("cleanup_locals_bytes %"));
    emit_out(int_str(mul(4, c)));
    emit_out(mks_(" "));
    no_indent = 1;
  }
}

function return_result() {
  nt();
  if(eq(0, match_(tok_, mks_(";")))) {
    expression();
  }
  cleanup_locals();
  indented_emit_out(mks_("ret\n"));
  nt();
}

function uniqueID(id, l) {
  emit(func, l);
  emit(mks_("_"), l);
  emit(id, l);
  emit(mks_("\n"), l);
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
  emit_out(mks_(":WHILE_"));
  uniqueID_out(number_string);
  nt();
  skip(mks_("("));
  expression();
  indented_emit_out(mks_("jump_false %END_WHILE_"));
  uniqueID_out(number_string);
  emit_out(mks_("# THEN_while_"));
  uniqueID_out(number_string);
  nt(); /* skip ) */
  statement();
  indented_emit_out(mks_("jump %WHILE_"));
  uniqueID_out(number_string);
  emit_out(mks_(":END_WHILE_"));
  uniqueID_out(number_string);
  break_target_num = nested_break_num;
}

function process_if() {
  var number_string;
  dprint("process_if");
  number_string = int_str(current_count);
  current_count = add(current_count, 1);
  emit_out(mks_("# IF_"));
  uniqueID_out(number_string);
  nt();
  skip(mks_("("));
  expression();
  indented_emit_out(mks_("jump_false %ELSE_"));
  uniqueID_out(number_string);
  skip(mks_(")"));
  statement();
  indented_emit_out(mks_("jump %_END_IF_"));
  uniqueID_out(number_string);
  emit_out(mks_(":ELSE_"));
  uniqueID_out(number_string);
  if(match_(tok_, mks_("else"))) {
    nt();
    statement();
  }
  emit_out(mks_(":_END_IF_"));
  uniqueID_out(number_string);
}

function process_break() {
  nt();
  indented_emit_out(mks_("jump %"));
  emit_out(break_target_prefix);
  emit_out(func);
  emit_out(mks_("_"));
  emit_out(break_target_num);
  emit_out(mks_("\n"));
  skip(mks_(";"));
}

function process_asm() {
  nt();
  skip(mks_("("));
  while(eq(char0(), mkc('"'))) {
/* FIXME strip off quotes in a non-js way */
    emit_out(mks_(tok.slice(1,-1)));
    emit_out(mks_("\n"));
    nt();
  }
  skip(mks_(")"));
  skip(mks_(";"));
}

function statement() {
  if(match_(tok_, mks_("{"))) {
    dprint("lcurly");
    nt();
    while(eq(0, match_(tok_, mks_("}")))) {
      statement();
    }
    skip(mks_("}"));
    dprint("rcurly");
  } else if(or(match_(tok_, mks_("var")), match_(tok_, mks_("int")))) {
    collect_local();
  } else if(match_(tok_, mks_("if"))) {
    process_if();
  } else if(match_(tok_, mks_("while"))) {
    process_while();
  } else if(match_(tok_, mks_("asm"))) {
    process_asm();
  } else if(match_(tok_, mks_("return"))) {
    return_result();
  } else if(match_(tok_, mks_("break"))) {
    process_break();
  } else {
    expression();
    skip(mks_(";"));
  }
}

function reset_locals(){
  locals = [];
}

function declare_function(t) {
  var i;
  reset_locals();
  dprint("declare_function: " +t);
  current_count = 0;
  func = t;
  if(match_(t, mks_("main"))) {
    frame_bias = 1;
  } else {
    frame_bias = 0;
  }
  collect_arguments();
  nt();
  if(match_(tok_, mks_(";"))) {
    dprint("function_prototype skip");
  } else if(match_(tok_, mks_("{"))) {
    dprint("function_body");
    emit_out(mks_(":FUNCTION_"));
    emit_out(func);
    increase_indent();
    emit_out(mks_("\n"));
    i = sub(args_len32(), 1);
    while(gt(i, sub(0,1))) {
      indented_emit_out(mks_("DEFINE ARG_"));
      emit_out(args_get32(i));
      emit_out(mks_(" "));
      /* FIXME explain this frame layout better */
      emit_out(to_hex_le(sub(0,mul(add(i,1),4))));
      emit_out(mks_("\n"));
      i = sub(i, 1);
    }
    statement();
    if(eq(0, match_(output_list[sub(output_list.length,1)], mks_("ret\n")))) {
      cleanup_locals();
      indented_emit_out(mks_("ret\n"));
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
    if(or(match_(tok_, mks_("int")), or(match_(tok_, mks_("var")),
          match_(tok_, mks_("function"))))) {
      nt(); ltok = tok_;
      nt();

      if(match_(tok_, mks_(";"))) {
        declare_global(ltok);
      } else if(match_(tok_, mks_("("))) {
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
  break_target_prefix = mks_("END_WHILE_");
  args_ = ra_new();
}

function join_list(l) {
  var o = [];
  var c;
  for(var i = 0; i < l.length; i++) {
    c = l[i];
    if(typeof c == "number") {
      c = mk_js_string(c);
    } else {
// DEBUG
      print("string: "+c);
    }
    o.push(c);
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
