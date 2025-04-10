#include "support.c"
#include "support_common.c"

#define function int
#define var int
#define bool int
#define FILE int

var O_WRONLY;
var O_CREAT;
var O_TRUNC;

/* decls */

function expect_tok_(expected_tok, file, line);
function get_tok();
function get_ident();
function parse_assignment_expression();
function parse_comma_expression();
function parse_call_params();
function parse_cast_expression();
function parse_compound_statement();
function parse_conditional_expression();
function parse_enum();
function parse_struct_or_union(struct_or_union_tok);
function parse_declarator(abstract_decl, parent_type);
function parse_declaration_specifiers(allow_typedef);
function parse_initializer_list();
function parse_initializer();
function generate_exe();
function mov_reg_imm(dst, imm);             // Move 32 bit immediate to register
function mov_reg_reg(dst, src);
function mov_mem8_reg(base, offset, src);
function mov_mem16_reg(base, offset, src);
function mov_mem32_reg(base, offset, src);
function mov_mem8_reg(base, offset, src);
function mov_reg_mem8(dst, base, offset);
function mov_reg_mem16(dst, base, offset);
function mov_reg_mem32(dst, base, offset);
function mov_reg_mem8_sign_ext(dst, base, offset);
function mov_reg_mem16_sign_ext(dst, base, offset);

function add_reg_imm(dst, imm);
function add_reg_lbl(dst, lbl);
function add_reg_reg(dst, src);
function or_reg_reg (dst, src);
function and_reg_reg(dst, src);
function sub_reg_reg(dst, src);
function xor_reg_reg(dst, src);
function imul_reg_reg(dst, src); // signed multiplication
function idiv_reg_reg(dst, src); // signed division
function irem_reg_reg(dst, src); // signed remainder
function mul_reg_reg(dst, src);  // unsigned multiplication
function div_reg_reg(dst, src);  // unsigned division
function rem_reg_reg(dst, src);  // unsigned remainder
function s_l_reg_reg(dst, src);  // signed/unsigned left shift
function sar_reg_reg(dst, src);  // signed right shift
function shr_reg_reg(dst, src);  // unsigned right shift
function mov_reg_lbl(reg, lbl);
function push_reg(src);
function pop_reg (dst);
function jump(lbl);
function jump_rel(offset);
function call(lbl);
function call_reg(reg);
function ret();
function debug_interrupt();



#define ast int
#define true 1
#define false 0
#define EOF (-1)

var line_number;
var column_number;
var last_tok_line_number;
var last_tok_column_number;

struct IncludeStack {
  int fp; // (FILE *)
  struct IncludeStack *next;
  int dirname;  // (char *) The base path of the file, used to resolve relative paths
  int filepath; // (char *) The path of the file, used to print error messages
  int line_number;
  int column_number;
};

struct IncludeStack *include_stack, *include_stack2;
var fp = 0; // Current file pointer that's being read
var fp_filepath = 0; // The path of the current file being read
var include_search_path = 0; // Search path for include files
var output_fd = 1; // Output file descriptor (1 = stdout)

// Tokens and AST nodes
enum {
  // Keywords
  AUTO_KW = 300,
  BREAK_KW,
  CASE_KW,
  CHAR_KW,
  CONST_KW,
  CONTINUE_KW,
  DEFAULT_KW,
  DO_KW,
  DOUBLE_KW,
  ELSE_KW,
  ENUM_KW,
  EXTERN_KW,
  FLOAT_KW,
  FOR_KW,
  GOTO_KW,
  IF_KW,
  INLINE_KW,
  INT_KW,
  LONG_KW,
  REGISTER_KW,
  RETURN_KW,
  SHORT_KW,
  SIGNED_KW,
  SIZEOF_KW,
  STATIC_KW,
  STRUCT_KW,
  SWITCH_KW,
  TYPEDEF_KW,
  UNION_KW,
  UNSIGNED_KW,
  VOID_KW,
  VOLATILE_KW,
  WHILE_KW,

  // Non-character operands
  INTEGER     = 401, // Integer written in decimal
  INTEGER_L   = 404,
  INTEGER_LL,
  INTEGER_U,
  INTEGER_UL,
  INTEGER_ULL,
  CHARACTER = 410, // Fixed value so the ifdef above don't change the value
  STRING    = 411,

  AMP_AMP   = 450,
  AMP_EQ,
  ARROW,
  BAR_BAR,
  BAR_EQ,
  CARET_EQ,
  EQ_EQ,
  GT_EQ,
  LSHIFT_EQ,
  LSHIFT,
  LT_EQ,
  MINUS_EQ,
  MINUS_MINUS,
  EXCL_EQ,
  PERCENT_EQ,
  PLUS_EQ,
  PLUS_PLUS,
  RSHIFT_EQ,
  RSHIFT,
  SLASH_EQ,
  STAR_EQ,
  HASH_HASH,
  PLUS_PLUS_PRE,
  MINUS_MINUS_PRE,
  PLUS_PLUS_POST,
  MINUS_MINUS_POST,
  ELLIPSIS,
  PARENS,
  INITIALIZER_LIST,
  DECL,
  DECLS,
  FUN_DECL,
  CAST,
  MACRO_ARG = 499,
  IDENTIFIER = 500, // 500 because it's easy to remember
  TYPE = 501,
  MACRO = 502,

  LIST = 600, // List object
};

function putstr(str) {
  while (ri8(str)) {
    putchar(ri8(str));
    str = add(str, 1);
  }
}

function putint_aux(n) {
  if (lte(n, sub(0,10))) {
    putint_aux(div_(n, 10));
  }
  putchar(sub(mkc('0'), mod(n, 10)));
}

function putint(n) {
  if (lt(n, 0)) {
    putchar(mkc('-'));
    putint_aux(n);
  } else {
    putint_aux(sub(0, n));
  }
}

function fatal_error(msg) {
  if (neq(include_stack, 0)) {
    putstr(include_stack->filepath); putchar(mkc(':'));
    putint(last_tok_line_number); putchar(mkc(':')); putint(last_tok_column_number);
    putstr(mks("  ")); putstr(msg); putchar(mkc('\n'));
  } else {
    putstr(msg); putchar(mkc('\n'));
  }
  exit(1);
}

function syntax_error(msg) {
  putstr(include_stack->filepath); putchar(mkc(':'));
  putint(last_tok_line_number); putchar(mkc(':')); putint(last_tok_column_number);
  putstr(mks("  syntax error: ")); putstr(msg); putchar(mkc('\n'));
  exit(1);
}

// tokenizer

var ch;
var tok;
var val;

var STRING_POOL_SIZE;
char *string_pool;
var string_pool_alloc;
var string_start;
var hash;

// These parameters give a perfect hashing of the C keywords
var HASH_PARAM;
var HASH_PRIME;
#define HEAP_SIZE 2000000
var heap[HEAP_SIZE];
var heap_alloc;

function r_heap(o) {
  return ri32(add(heap,mul(4,o)));
}

var alloc_result;

function alloc_obj(size) {
  alloc_result = heap_alloc;
  heap_alloc = add(heap_alloc, size);

  if (gt(heap_alloc, HEAP_SIZE)) {
    fatal_error(mks("heap overflow"));
  }

  return alloc_result;
}

function get_op(node) {
  return and(r_heap(node), 1023);
}

function get_nb_children(node) {
  return shr(r_heap(node), 10);
}

function get_val(node) {
  return r_heap(add(node,1));
}

function set_val(node, val) {
  heap[node+1] = val;
}

function get_child(node, i) {
  return r_heap(add(node, add(i,1)));
}

function set_child(node, i, child) {
  heap[node+i+1] = child;
}

function get_val_(expected_node, node) {
  return get_val(node);
}

function get_child_(expected_parent_node, node, i) {
  return get_child(node, i);
}

function get_child__(expected_parent_node, expected_node, node, i) {
  return get_child(node, i);
}

function get_child_opt_(expected_parent_node, expected_node, node, i) {
  return get_child(node, i);
}

var ast_result;

function new_ast0(op, val) {

  ast_result = alloc_obj(2);

  heap[ast_result] = op;
  set_val(ast_result, val);

  return ast_result;
}

function new_ast1(op, child0) {

  ast_result = alloc_obj(2);

  heap[ast_result] = add(op, 1024);
  set_child(ast_result, 0, child0);

  return ast_result;
}

function new_ast2(op, child0, child1) {

  ast_result = alloc_obj(3);

  heap[ast_result] = add(op, 2048);
  set_child(ast_result, 0, child0);
  set_child(ast_result, 1, child1);

  return ast_result;
}

function new_ast3(op, child0, child1, child2) {

  ast_result = alloc_obj(4);

  heap[ast_result] = add(op, 3072);
  set_child(ast_result, 0, child0);
  set_child(ast_result, 1, child1);
  set_child(ast_result, 2, child2);

  return ast_result;
}

function new_ast4(op, child0, child1, child2, child3) {

  ast_result = alloc_obj(5);

  heap[ast_result] = add(op, 4096);
  set_child(ast_result, 0, child0);
  set_child(ast_result, 1, child1);
  set_child(ast_result, 2, child2);
  set_child(ast_result, 3, child3);

  return ast_result;
}

function clone_ast(orig) {
  int nb_children;
  int i;

  nb_children = get_nb_children(orig);

  // Account for the value of ast nodes with no child
  if (eq(nb_children, 0)) {
    nb_children = 1;
  }

  ast_result = alloc_obj(add(nb_children, 1));

  heap[ast_result] = r_heap(orig); // copy operator and nb of children
  i = 0;
  while(lt(i, nb_children)) {
    set_child(ast_result, i, get_child(orig, i));
    i = add(i,1);
  }

  return ast_result;
}

// TODO: Use macro to avoid indirection?
// Functions used to create and access lists.
function cons(child0, child1) {
  return new_ast2(LIST, child0, child1);
}
function car(pair) {
  return get_child_(LIST, pair, 0);
}
function car_(expected_op, pair) {
  return get_child__(LIST, expected_op, pair, 0);
}
function cdr(pair) {
  return get_child_(LIST, pair, 1);
}
function cdr_(expected_op, pair) {
  return get_child_opt_(LIST, expected_op, pair, 1);
}
function set_cdr(pair, value) {
  set_child(pair, 1, value);
}
function tail(x) {
  return cdr_(LIST, x);
}

// Simple accessor to get the string from the string pool
function STRING_BUF(string_val) {
  return add(string_pool, r_heap(add(string_val,1)));
}

function STRING_LEN(string_val) {
  return r_heap(add(string_val,4));
}

function begin_string() {
  string_start = string_pool_alloc;
  hash = 0;
}

// Append the current character (ch) to the string under construction in the pool
function accum_string() {
  hash = mod(add(ch, xor(hash, HASH_PARAM)), HASH_PRIME);
  wi8(add(string_pool, string_pool_alloc), ch);
  string_pool_alloc = add(string_pool_alloc, 1);
  if (gte(string_pool_alloc, STRING_POOL_SIZE)) {
    fatal_error(mks("string pool overflow"));
  }
}

// Append a character to the current string under construction in the pool
// FIXME LJW should be a char?
function accum_string_char(c) {
  hash = (c + (hash ^ HASH_PARAM)) % HASH_PRIME;
  string_pool[string_pool_alloc] = c;
  string_pool_alloc += 1;
  if (string_pool_alloc >= STRING_POOL_SIZE) {
    fatal_error("string pool overflow");
  }
}

// Append a string from the string_pool to the string under construction
function accum_string_string(string_probe) {
  var string_start;
  var string_end;

  string_start = STRING_BUF(string_probe);
  string_end = add(string_start, STRING_LEN(string_probe));
  while (lt(string_start, string_end)) {
    accum_string_char(ri8(string_start));
    string_start = add(string_start, 1);
  }
}

// Similar to accum_string_string, but writes an integer to the string pool
// Note that this function only supports small integers, represented as positive number.
function accum_string_integer(n) {
  if (lt(n, 0)) {
    accum_string_char(mkc('-'));
    accum_string_integer(sub(0, n));
  } else {
    if (gt(n, 9)) {
      accum_string_integer(div_(n, 10));
    }
    accum_string_char(add(mkc('0'), mod(n, 10)));
  }
}

var probe;
var probe_start;
var c1;
var c2;
var end_ident_i;

function end_ident() {
  string_pool[string_pool_alloc] = 0; // terminate string
  string_pool_alloc = add(string_pool_alloc, 1); // account for terminator

  probe = r_heap(hash);

  while (neq(probe, 0)) {
    probe_start = r_heap(add(probe,1));
    end_ident_i = 0;
    c1 = ri8(add(string_pool, add(string_start, end_ident_i)));
    c2 = ri8(add(string_pool, add(probe_start, end_ident_i)));
    while (eq(c1, c2)) {
      if (eq(c1, 0)) {
        string_pool_alloc = string_start; // undo string allocation
        return probe;
      }
      end_ident_i = add(end_ident_i, 1);
      c1 = ri8(add(string_pool, add(string_start, end_ident_i)));
      c2 = ri8(add(string_pool, add(probe_start, end_ident_i)));
    }
    hash = probe; // remember previous ident
    probe = r_heap(probe);
  }

  // a new ident has been found

  probe = alloc_obj(5);

  heap[hash] = probe; // add new ident at end of chain

  heap[probe] = 0; // no next ident
  heap[add(probe, 1)] = string_start;
  heap[add(probe, 2)] = IDENTIFIER;
  heap[add(probe, 3)] = 0; // Token tag
  heap[add(probe, 4)] = sub(sub(string_pool_alloc, string_start), 1); // string length (excluding terminator)

  return probe;
}

// Like end_ident, but for strings instead of identifiers
// We want to deduplicate strings to reuse memory if possible.
function end_string() {
  return end_ident();
}

function expect_tok(expected_tok) {
  expect_tok_(expected_tok, __FILE__, __LINE__);
}

#define IFDEF_DEPTH_MAX 20
var if_macro_stack[IFDEF_DEPTH_MAX]; // Stack of if macro states
var if_macro_stack_ix = 0;
var if_macro_mask = true;      // Indicates if the current if/elif block is being executed
var if_macro_executed = false; // If any of the previous if/elif conditions were true

function r_if_macro_stack(o) {
  return ri32(add(if_macro_stack,mul(4,o)));
}

// get_tok parameters:
// Whether to expand macros or not.
// Useful to parse macro definitions containing other macros without expanding them.
var expand_macro = true;
// Don't expand macro arguments. Used for stringification and token pasting.
var expand_macro_arg = true;
// Don't produce newline tokens. Used when reading the tokens of a macro definition.
var skip_newlines = true;

#define MACRO_RECURSION_MAX 180 // Supports up to 60 (180 / 3) nested macro expansions.
var macro_stack[MACRO_RECURSION_MAX];
var macro_stack_ix = 0;

function r_macro_stack(o) {
  return ri32(add(macro_stack,mul(4,o)));
}

var macro_tok_lst = 0;  // Current list of tokens to replay for the macro being expanded
var macro_args = 0;     // Current list of arguments for the macro being expanded
var macro_ident = 0;    // The identifier of the macro being expanded (if any)
var macro_args_count;   // Number of arguments for the current macro being expanded
var paste_last_token = false; // Whether the last token was a ## or not

function prev_macro_mask() {
  if(eq(if_macro_stack_ix, 0)) {
    return 0;
  }
  return r_if_macro_stack(sub(if_macro_stack_ix, 2));
}

function push_if_macro_mask(new_mask) {
  if (gte(if_macro_stack_ix, IFDEF_DEPTH_MAX)) {
    fatal_error(mks("Too many nested #ifdef/#ifndef directives. Maximum supported is 20."));
  }
  // Save current mask on the stack because it's about to be overwritten
  if_macro_stack[if_macro_stack_ix] = if_macro_mask;
  if_macro_stack[add(if_macro_stack_ix, 1)] = if_macro_executed;
  if_macro_stack_ix = add(if_macro_stack_ix, 2);

  // If the current block is masked off, then the new mask is the logical AND of the current mask and the new mask
  new_mask = and(if_macro_mask, new_mask);

  // Then set the new mask value and reset the executed flag
  if_macro_mask = new_mask;
  if_macro_executed = new_mask;
}

function pop_if_macro_mask() {
  if (eq(if_macro_stack_ix, 0)) {
    fatal_error(mks("Unbalanced #ifdef/#ifndef/#else/#endif directives."));
  }
  if_macro_stack_ix = sub(if_macro_stack_ix, 2);
  if_macro_mask = r_if_macro_stack(if_macro_stack_ix);
  if_macro_executed = r_if_macro_stack(if_macro_stack_ix, 1);
}

function get_ch() {
  ch = fgetc(fp);

  if (eq(ch, EOF)) {
    // If it's not the last file on the stack, EOF means that we need to switch to the next file
    if (neq(include_stack->next, 0)) {
      fclose(include_stack->fp);
      include_stack2 = include_stack;
      include_stack = include_stack->next;
      fp = include_stack->fp;
      fp_filepath = include_stack->filepath;
      line_number = include_stack->line_number;
      column_number = include_stack->column_number;
      // Not freeing include_stack2->filepath because it may not be dynamically allocated
      free(include_stack2->dirname);
      free(include_stack2);
      // EOF is treated as a newline so that files without a newline at the end are still parsed correctly
      // On the next get_ch call, the first character of the next file will be read
      ch = mkc('\n');
    }
  }
  else if (eq(ch, mkc('\n'))) {
    line_number = add(line_number, 1);
    column_number = 0;
  } else {
    column_number = add(column_number, 1);
  }
}

// TODO: It would be nice to not have to duplicate this code
function strlen(str) {
  var i;
  i = 0;
  while (neq(ri8(add(str, i)), 0)) {
    i = add(i, 1);
  }
  return i;
}

function memcpy(dest, src, n) {
  var i;
  i = 0;
  while(lt(i, n)) {
    wi8(add(dest, i), ri8(add(src, i)));
    i = add(i, 1);
  }
}

function substr(str, start, end) {
  var len;
  var temp;
  len = sub(end, start);
  temp = malloc(add(len, 1));
  memcpy(temp, add(str, start), len);
  wi8(add(temp, len), 0);
  return temp;
}

function str_concat(s1, s2) {
  var s1_len;
  var s2_len;
  var temp;
  s1_len = strlen(s1);
  s2_len = strlen(s2);
  temp = malloc(add(add(s1_len, s2_len), 1));
  memcpy(temp, s1, s1_len);
  memcpy(add(temp, s1_len), s2, s2_len);
  wi8(add(temp, add(s1_len, s2_len)), 0);
  return temp;
}

// Removes the last component of the path, keeping the trailing slash if any.
// For example, /a/b/c.txt -> /a/b/
// If the path does not contain a slash, it returns "".
function file_parent_directory(path) {
  var i;
  var last_slash;
  i = 0;
  last_slash = sub(0, 1);
  while (neq(ri8(add(path, i)), 0)) {
    if (eq(ri8(add(path, i)), mkc('/'))) {
      last_slash = i;
    }
    i = add(i, 1);
  }
  if (eq(last_slash, sub(i, 1))) {
    path = malloc(1);
    wi8(path, 0);
  } else {
    path = substr(path, 0, add(last_slash, 1));
  }
  return path;
}

function fopen_source_file(file_name, relative_to) {
  var fp;
  fp_filepath = file_name;
  if (relative_to) {
    fp_filepath = str_concat(relative_to, fp_filepath);
  }
  fp = fopen(fp_filepath, mks("r"));
  if (eq(fp, 0)) {
    putstr(fp_filepath); putchar(mkc('\n'));
    fatal_error(mks("Could not open file"));
  }
  return fp;
}

function include_file(file_name, relative_to) {
  fp = fopen_source_file(file_name, relative_to);
  include_stack2 = malloc(sizeof(struct IncludeStack));
  include_stack2->next = include_stack;
  include_stack2->fp = fp;
  include_stack2->dirname = file_parent_directory(fp_filepath);
  include_stack2->filepath = fp_filepath;
  include_stack2->line_number = 1;
  include_stack2->column_number = 0;
  // Save the current file position so we can return to it after the included file is done
  if (neq(include_stack, 0)) {
    include_stack->line_number = line_number;
    include_stack->column_number = column_number;
  }
  line_number = 1;
  column_number = 1;
  include_stack = include_stack2;
}

function DIGIT_BYTE(val) {
  return (mod(sub(0, val), 256));
}

function INIT_ACCUM_DIGIT() {
  return 0;
}

function accum_digit(base) {
  int digit = 99;
  if (and(lte(mkc('0'), ch), lte(ch, mkc('9')))) {
    digit = sub(ch, mkc('0'));
  } else if (and(lte(mkc('A'), ch), lte(ch, mkc('Z')))) {
    digit = add(sub(ch, mkc('A')), 10);
  } else if (and(lte(mkc('a'), ch), lte(ch, mkc('z')))) {
    digit = ch - 'a' + 10;
  }
  if (gte(digit, base)) {
    return 0; // character is not a digit in that base
  } else {
    // TODO: Put overflow check back
    // if ((val < limit) || ((val == limit) && (digit > limit * base - MININT))) {
    //   fatal_error("literal integer overflow");
    // }

    val = sub(mul(val, base), digit);
    get_ch();
    return 1;
  }
}

function get_string_char() {

  val = ch;
  get_ch();

  if (eq(val, mkc('\\'))) {
    if (and(lte(mkc('0'), ch), lte(ch, mkc('7')))) {
      // Parse octal character, up to 3 digits.
      // Note that \1111 is parsed as '\111' followed by '1'
      // See https://en.wikipedia.org/wiki/Escape_sequences_in_C#Notes
      val = INIT_ACCUM_DIGIT();
      accum_digit(8);
      accum_digit(8);
      accum_digit(8);
      val = DIGIT_BYTE(val); // keep low 8 bits, without overflowing
    } else if (or(eq(ch, mkc('x')), eq(ch, mkc('X')))) {
      get_ch();
      val = INIT_ACCUM_DIGIT();
      // Allow 1 or 2 hex digits.
      if (accum_digit(16)) {
        accum_digit(16);
      } else {
        syntax_error("invalid hex escape -- it must have at least one digit");
      }
      val = DIGIT_BYTE(val); // keep low 8 bits, without overflowing
    } else {
      if (eq(ch, mkc('a'))) {
        val = 7;
      } else if (eq(ch, mkc('b'))) {
        val = 8;
      } else if (eq(ch, mkc('f'))) {
        val = 12;
      } else if (eq(ch, mkc('n'))) {
        val = 10;
      } else if (eq(ch, mkc('r'))) {
        val = 13;
      } else if (eq(ch, mkc('t'))) {
        val = 9;
      } else if (eq(ch, mkc('v'))) {
        val = 11;
      } else if (or(or(eq(ch, mkc('\\')),eq(ch,mkc('\''))),eq(ch,mkc('\"')))) {
        val = ch;
      } else {
        syntax_error(mks("unimplemented string character escape"));
      }
      get_ch();
    }
  }
}

function accum_string_until(end) {
  while (and(neq(ch, end), neq(ch, EOF))) {
    get_string_char();
    tok = ch;
    ch = val;
    accum_string();
    ch = tok;
  }

  if (neq(ch, end)) {
    syntax_error(mks("unterminated string literal"));
  }

  get_ch();
}

// We add the preprocessor keywords to the ident table so they can be easily
// recognized by the preprocessor. Because these are not C keywords, their kind
// is still IDENTIFIER so the parser (which runs after the preprocessor) can
// treat them as such.
var IFDEF_ID;
var IFNDEF_ID;
var ELIF_ID;
var ENDIF_ID;
var DEFINE_ID;
var UNDEF_ID;
var INCLUDE_ID;
var DEFINED_ID;
var WARNING_ID;
var ERROR_ID;
var INCLUDE_SHELL_ID;

var NOT_SUPPORTED_ID;

// We want to recognize certain identifers without having to do expensive string comparisons
var ARGV__ID;
var ARGV_ID;
var IFS_ID;
var MAIN_ID;

var PUTCHAR_ID;
var GETCHAR_ID;
var EXIT_ID;
var MALLOC_ID;
var FREE_ID;
var PRINTF_ID;
var FOPEN_ID;
var FCLOSE_ID;
var FGETC_ID;
var PUTSTR_ID;
var PUTS_ID;
var READ_ID;
var WRITE_ID;
var OPEN_ID;
var CLOSE_ID;

// Macros that are defined by the preprocessor
var FILE__ID;
var LINE__ID;

// When we parse a macro, we generally want the tokens as they are, without expanding them.
function get_tok_macro() {
  bool prev_expand_macro = expand_macro;
  bool prev_macro_mask = if_macro_mask;
  bool skip_newlines_prev = skip_newlines;

  expand_macro = false;
  if_macro_mask = true;
  skip_newlines = false;
  get_tok();
  expand_macro = prev_expand_macro;
  if_macro_mask = prev_macro_mask;
  skip_newlines = skip_newlines_prev;
}

// Like get_tok_macro, but skips newline
// This is useful when we want to read the arguments of a macro expansion.
function get_tok_macro_expand() {
  bool prev_expand_macro = expand_macro;
  bool prev_macro_mask = if_macro_mask;

  expand_macro = false;
  if_macro_mask = true;
  get_tok();
  expand_macro = prev_expand_macro;
  if_macro_mask = prev_macro_mask;
}

function lookup_macro_token(args, tok, val) {
  int ix;
  ix = 0;

  if (lt(tok, IDENTIFIER)) {
    return cons(tok, val); // Not an identifier
  }

  while (neq(args, 0)) {
    if (eq(car(args), val)) {
      break; // Found!
    }
    args = cdr(args);
    ix = add(ix, 1);
  }

  if (eq(args, 0)) { // Identifier is not a macro argument
    return cons(tok, val);
  } else {
    return cons(MACRO_ARG, ix);
  }
}

function read_macro_tokens(args) {
  int toks; // List of token to replay
  int tail;
  toks = 0;

  // Accumulate tokens so they can be replayed when the macro is used
  if (and(neq(tok, mkc('\n')), neq(tok, EOF))) {
    // Append the token/value pair to the replay list
    toks = cons(lookup_macro_token(args, tok, val), 0);
    tail = toks;
    get_tok_macro();
    while (and(neq(tok, mkc('\n')), neq(tok, EOF))) {
      set_cdr(tail, cons(lookup_macro_token(args, tok, val), 0));
      tail = cdr(tail); // Advance tail
      get_tok_macro();
    }

    // Check that there are no leading or trailing ##
    if (or(eq(car(car(toks)), HASH_HASH), eq(car(car(tail)), HASH_HASH))) {
      syntax_error(mks("'##' cannot appear at either end of a macro expansion"));
    }
  }

  return toks;
}

// A few things that are different from the standard:
// - We allow sequence of commas in the argument list
// - Function-like macros with 0 arguments can be called either without parenthesis or with ().
// - No support for variadic macros. Tcc only uses them in tests so it should be ok.
function handle_define() {
  var macro;    // The identifier that is being defined as a macro
  var args; // List of arguments for a function-like macro
  var args_count; // Number of arguments for a function-like macro. -1 means it's an object-like macro

  args = 0;
  args_count = sub(0, 1);

  if (and(and(neq(tok,IDENTIFIER),neq(tok,MACRO)),or(lt(tok,AUTO_KW),lt(tok,WHILE_KW)))) {
    putstr(mks("tok=")); putint(tok); putchar(mkc('\n'));
    syntax_error(mks("#define directive can only be followed by a identifier"));
  }

  heap[add(val, 2)] = MACRO; // Mark the identifier as a macro
  macro = val;
  if (eq(ch, mkc('('))) { // Function-like macro
    args_count = 0;
    get_tok_macro(); // Skip macro name
    get_tok_macro(); // Skip '('
    while (and(neq(tok, mkc('\n')), neq(tok, EOF))) {
      if (eq(tok, mkc(','))) {
        // Allow sequence of commas, this is more lenient than the standard
        get_tok_macro();
        continue;
      } else if (eq(tok, mkc(')'))) {
        get_tok_macro();
        break;
      }
      get_tok_macro();
      // Accumulate parameters in reverse order. That's ok because the arguments
      // to the macro will also be in reverse order.
      args = cons(val, args);
      args_count = add(args_count, 1);
    }
  } else {
    get_tok_macro(); // Skip macro name
  }

  // Accumulate tokens so they can be replayed when the macro is used
  heap[add(macro, 3)] = cons(read_macro_tokens(args), args_count);

}

function eval_constant(expr, if_macro) {
  var op;
  var op1;
  var op2;
  var child0;
  var child1;

  op = get_op(expr);

  if (gte(get_nb_children(expr), 1)) {
    child0 = get_child(expr, 0);
  }
  if (gte(get_nb_children(expr), 2)) {
    child1 = get_child(expr, 1);
  }

  switch (op) {
    case PARENS:      return eval_constant(child0, if_macro);
    case INTEGER:
    case INTEGER_L:
    case INTEGER_LL:
    case INTEGER_U:
    case INTEGER_UL:
    case INTEGER_ULL:
      return sub(0, get_val(expr));
    case CHARACTER:   return get_val_(CHARACTER, expr);
    case '~':         return not(eval_constant(child0, if_macro));
    case '!':         return eq(0, eval_constant(child0, if_macro));
    case '-':
    case '+':
      op1 = eval_constant(child0, if_macro);
      if (eq(get_nb_children(expr), 1)) {
        if(eq(op, mkc('-'))) {
          return sub(0, op1);
        }
        return op1;
      } else {
        op2 = eval_constant(child1, if_macro);
        if(eq(op,mkc('-'))) {
          return sub(op1, op2);
        }
        return add(op1, op2);
      }

    case '?':
      op1 = eval_constant(child0, if_macro);
      if (op1) {
        return eval_constant(child1, if_macro);
      } else {
        return eval_constant(get_child(expr, 2), if_macro);
      }

    case '*':
    case '/':
    case '%':
    case '&':
    case '|':
    case '^':
    case LSHIFT:
    case RSHIFT:
    case EQ_EQ:
    case EXCL_EQ:
    case LT_EQ:
    case GT_EQ:
    case '<':
    case '>':
      op1 = eval_constant(child0, if_macro);
      op2 = eval_constant(child1, if_macro);
      switch (op) {
        case '*':     return mul(op1, op2);
        case '/':     return div_(op1, op2);
        case '%':     return mod(op1, op2);
        case '&':     return and(op1, op2);
        case '|':     return or(op1, op2);
        case '^':     return xor(op1, op2);
        case LSHIFT:  return shl(op1, op2);
        case RSHIFT:  return shr(op1, op2);
        case EQ_EQ:   return eq(op1, op2);
        case EXCL_EQ: return neq(op1, op2);
        case LT_EQ:   return lte(op1, op2);
        case GT_EQ:   return gte(op1, op2);
        case '<':     return lt(op1, op2);
        case '>':     return gt(op1, op2);
      }
      return 0; // Should never reach here

    case AMP_AMP:
      op1 = eval_constant(child0, if_macro);
      if (eq(0,op1)) {
        return 0;
      } else {
        return eval_constant(child1, if_macro);
      }

    case BAR_BAR:
      op1 = eval_constant(child0, if_macro);
      if (op1) {
        return 1;
      } else {
        return eval_constant(child1, if_macro);
      }

    case '(': // defined operators are represented as fun calls
      if (and(if_macro, or(get_val_(IDENTIFIER, child0),DEFINED_ID))) {
        return eq(child1, MACRO);
      } else {
        fatal_error(mks("unknown function call in constant expressions"));
        return 0;
      }

    case IDENTIFIER:
      if (if_macro) {
        // Undefined identifiers are 0
        // At this point, macros have already been expanded so we can't have a macro identifier
        return 0;
      } else {
        // TODO: Enums when outside of if_macro
        fatal_error(mks("identifiers are not allowed in constant expression"));
        return 0;
      }

    default:
      putstr(mks("op=")); putint(op); putchar(mkc('\n'));
      fatal_error(mks("unsupported operator in constant expression"));
      return 0;
  }
}


function evaluate_if_condition() {
  var prev_skip_newlines;
  var previous_mask;
  var expr;

  prev_skip_newlines = skip_newlines;
  previous_mask = if_macro_mask;
  // Temporarily set to true so that we can read the condition even if it's inside an ifdef false block
  // Unlike in other directives using get_tok_macro, we want to expand macros in the condition
  if_macro_mask = true;
  skip_newlines = false; // We want to stop when we reach the first newline
  get_tok(); // Skip the #if keyword
  expr = parse_assignment_expression();

  // Restore the previous value
  if_macro_mask = previous_mask;
  skip_newlines = prev_skip_newlines;
  return eval_constant(expr, true);
}

function handle_include() {
  if (eq(tok, STRING)) {
    include_file(STRING_BUF(val), include_stack->dirname);
    get_tok_macro(); // Skip the string
  } else if (eq(tok, mkc('<'))) {
    accum_string_until(mkc('>'));
    val = end_string();
    // #include <file> directives only take effect if the search path is provided
    // TODO: Issue a warning to stderr when skipping the directive
    if (neq(include_search_path, 0)) {
      include_file(STRING_BUF(val), include_search_path);
    }
    get_tok_macro(); // Skip the string
  } else {
    putstr(mks("tok=")); putint(tok); putchar(mkc('\n'));
    syntax_error(mks("expected string to #include directive"));
  }
}

function handle_preprocessor_directive() {
  var temp;
  get_tok_macro(); // Get the # token
  get_tok_macro(); // Get the directive

  if (and(eq(tok,IDENTIFIER),or(eq(val,IFDEF_ID),eq(val,IFNDEF_ID)))) {
    temp = val;
    get_tok_macro(); // Get the macro name
    if(eq(temp,IFDEF_ID)) {
      push_if_macro_mask(eq(tok,MACRO));
    } else {
      push_if_macro_mask(neq(tok,MACRO));
    }
    get_tok_macro(); // Skip the macro name
  } else if (eq(tok, IF_KW)) {
    temp = neq(evaluate_if_condition(), 0);
    push_if_macro_mask(temp);
  } else if (and(eq(tok, IDENTIFIER), eq(val, ELIF_ID))) {
    temp = neq(evaluate_if_condition(), 0);
    if (and(prev_macro_mask(), eq(0, if_macro_executed))) {
      if_macro_executed = or(if_macro_executed, temp);
      if_macro_mask = temp;
    } else {
      if_macro_mask = false;
    }
  } else if (eq(tok, ELSE_KW)) {
    if (prev_macro_mask()) { // If the parent block mask is true
      if_macro_mask = eq(0, if_macro_executed);
      if_macro_executed = true;
    } else {
      if_macro_mask = false;
    }
    get_tok_macro(); // Skip the else keyword
  } else if (and(eq(tok, IDENTIFIER), eq(val, ENDIF_ID))) {
    pop_if_macro_mask();
    get_tok_macro(); // Skip the else keyword
  } else if (if_macro_mask) {
    if (and(eq(tok, IDENTIFIER), eq(val, INCLUDE_ID))) {
      get_tok_macro(); // Get the STRING token
      handle_include();
    }
    else if (and(eq(tok, IDENTIFIER), eq(val, UNDEF_ID))) {
      get_tok_macro(); // Get the macro name
      if (or(eq(tok, IDENTIFIER), eq(tok, MACRO))) {
        // TODO: Doesn't play nice with typedefs, because they are not marked as macros
        heap[add(val, 2)] = IDENTIFIER; // Unmark the macro identifier
        get_tok_macro(); // Skip the macro name
      } else {
        putstr(mks("tok=")); putint(tok); putchar(mkc('\n'));
        syntax_error(mks("#undef directive can only be followed by a identifier"));
      }
    } else if (and(eq(tok, IDENTIFIER), eq(val, DEFINE_ID))) {
      get_tok_macro(); // Get the macro name
      handle_define();
    } else if (and(eq(tok,IDENTIFIER),or(eq(val,WARNING_ID),eq(val,ERROR_ID)))) {
      temp = val;
      if(eq(temp, WARNING_ID)) {
        putstr(mks("warning:"));
      } else {
        putstr(mks("error:"));
      }
      // Print the rest of the line, it does not support \ at the end of the line but that's ok
      while (and(neq(ch, mkc('\n')), neq(ch, EOF))) {
        putchar(ch); get_ch();
      }
      putchar(mkc('\n'));
      tok = mkc('\n');
      if (eq(temp, ERROR_ID)) {
        exit(1);
      }
    } else {
      putstr(mks("tok=")); putint(tok); putstr(mks(": ")); putstr(STRING_BUF(val)); putchar(mkc('\n'));
      syntax_error(mks("unsupported preprocessor directive"));
    }
  } else {
    // Skip the rest of the directive
    while (and(neq(tok, mkc('\n')), neq(tok, EOF))) {
      get_tok_macro();
    }
  }

  if (and(neq(tok, mkc('\n')), neq(tok, EOF))) {
    putstr(mks("tok=")); putint(tok); putchar(mkc('\n'));
    putstr(mks("directive=")); putint(tok); putchar(mkc('\n'));
    if (or(eq(tok, IDENTIFIER), eq(tok, MACRO))) {
      putstr(mks("string = "));
      putstr(STRING_BUF(val));
      putchar(mkc('\n'));
    }
    syntax_error(mks("preprocessor expected end of line"));
  }

  // Because handle_preprocessor_directive is called from get_tok, and it loops
  // after the call to handle_preprocessor_directive, we don't need to call
  // get_tok before returning.
}

function get_ident() {

  begin_string();

  while (or(or(and(lte(mkc('A'), ch), lte(ch, mkc('Z'))),
               and(lte(mkc('a'), ch), lte(ch, mkc('z')))),
            or(and(lte(mkc('0'), ch), lte(ch, mkc('9'))),
               eq(ch, mkc('_'))))) {
    accum_string();
    get_ch();
  }

  val = end_ident();
  tok = r_heap(add(val, 2));
}

function intern_str(name) {
  var i;
  var prev_ch;

  i = 0;
  prev_ch = ch; // The character may be important to the calling function, saving it

  begin_string();

  while (neq(ri8(add(name, i)), 0)) {
    ch = ri8(add(name, i));
    accum_string();
    i = add(i, 1);
  }

  i = end_string();

  ch = prev_ch;

  return i;
}

function init_ident(tok, name) {
  var i;
  i = intern_str(name);
  heap[add(i, 2)] = tok;
  return i;
}

function init_ident_table() {

  var i;
  i = 0;

  while (lt(i, HASH_PRIME)) {
    heap[i] = 0;
    i = add(i, 1);
  }

  init_ident(AUTO_KW,     mks("auto"));
  init_ident(BREAK_KW,    mks("break"));
  init_ident(CASE_KW,     mks("case"));
  init_ident(CHAR_KW,     mks("char"));
  init_ident(CONST_KW,    mks("const"));
  init_ident(CONTINUE_KW, mks("continue"));
  init_ident(DEFAULT_KW,  mks("default"));
  init_ident(DO_KW,       mks("do"));
  init_ident(DOUBLE_KW,   mks("double"));
  init_ident(ELSE_KW,     mks("else"));
  init_ident(ENUM_KW,     mks("enum"));
  init_ident(EXTERN_KW,   mks("extern"));
  init_ident(FLOAT_KW,    mks("float"));
  init_ident(FOR_KW,      mks("for"));
  init_ident(GOTO_KW,     mks("goto"));
  init_ident(IF_KW,       mks("if"));
  init_ident(INLINE_KW,   mks("inline"));
  init_ident(INT_KW,      mks("int"));
  init_ident(LONG_KW,     mks("long"));
  init_ident(REGISTER_KW, mks("register"));
  init_ident(RETURN_KW,   mks("return"));
  init_ident(SHORT_KW,    mks("short"));
  init_ident(SIGNED_KW,   mks("signed"));
  init_ident(SIZEOF_KW,   mks("sizeof"));
  init_ident(STATIC_KW,   mks("static"));
  init_ident(STRUCT_KW,   mks("struct"));
  init_ident(SWITCH_KW,   mks("switch"));
  init_ident(TYPEDEF_KW,  mks("typedef"));
  init_ident(UNION_KW,    mks("union"));
  init_ident(UNSIGNED_KW, mks("unsigned"));
  init_ident(VOID_KW,     mks("void"));
  init_ident(VOLATILE_KW, mks("volatile"));
  init_ident(WHILE_KW,    mks("while"));

  // Preprocessor keywords. These are not tagged as keyword since they can be
  // used as identifiers after the preprocessor stage.
  IFDEF_ID   = init_ident(IDENTIFIER, mks("ifdef"));
  IFNDEF_ID  = init_ident(IDENTIFIER, mks("ifndef"));
  ELIF_ID    = init_ident(IDENTIFIER, mks("elif"));
  ENDIF_ID   = init_ident(IDENTIFIER, mks("endif"));
  DEFINE_ID  = init_ident(IDENTIFIER, mks("define"));
  WARNING_ID = init_ident(IDENTIFIER, mks("warning"));
  ERROR_ID   = init_ident(IDENTIFIER, mks("error"));
  UNDEF_ID   = init_ident(IDENTIFIER, mks("undef"));
  INCLUDE_ID = init_ident(IDENTIFIER, mks("include"));
  DEFINED_ID = init_ident(IDENTIFIER, mks("defined"));
  INCLUDE_SHELL_ID = init_ident(IDENTIFIER, mks("include_shell"));

  ARGV_ID = init_ident(IDENTIFIER, mks("argv"));
  ARGV__ID = init_ident(IDENTIFIER, mks("argv_"));
  IFS_ID  = init_ident(IDENTIFIER, mks("IFS"));
  MAIN_ID = init_ident(IDENTIFIER, mks("main"));

  PUTCHAR_ID = init_ident(IDENTIFIER, mks("putchar"));
  GETCHAR_ID = init_ident(IDENTIFIER, mks("getchar"));
  EXIT_ID    = init_ident(IDENTIFIER, mks("exit"));
  MALLOC_ID  = init_ident(IDENTIFIER, mks("malloc"));
  FREE_ID    = init_ident(IDENTIFIER, mks("free"));
  PRINTF_ID  = init_ident(IDENTIFIER, mks("printf"));
  FOPEN_ID   = init_ident(IDENTIFIER, mks("fopen"));
  FCLOSE_ID  = init_ident(IDENTIFIER, mks("fclose"));
  FGETC_ID   = init_ident(IDENTIFIER, mks("fgetc"));
  PUTSTR_ID  = init_ident(IDENTIFIER, mks("putstr"));
  PUTS_ID    = init_ident(IDENTIFIER, mks("puts"));
  READ_ID    = init_ident(IDENTIFIER, mks("read"));
  WRITE_ID   = init_ident(IDENTIFIER, mks("write"));
  OPEN_ID    = init_ident(IDENTIFIER, mks("open"));
  CLOSE_ID   = init_ident(IDENTIFIER, mks("close"));

  // Stringizing is recognized by the macro expander, but it returns a hardcoded
  // string instead of the actual value. This may be enough to compile TCC.
  NOT_SUPPORTED_ID = init_ident(IDENTIFIER, mks("NOT_SUPPORTED"));
}

function init_builtin_string_macro(macro_str, value) {
  var macro_id;
  macro_id = init_ident(MACRO, macro_str);
  // Macro object shape: ([(tok, val)], arity). -1 arity means it's an object-like macro
  heap[add(macro_id, 3)] = cons(cons(cons(STRING, intern_str(value)), 0), sub(0,1));
  return macro_id;
}

function init_builtin_int_macro(macro_str, value) {
  int macro_id = init_ident(MACRO, macro_str);
  heap[add(macro_id, 3)] = cons(cons(cons(INTEGER, sub(0,value)), 0), sub(0,1));
  return macro_id;
}

function init_builtin_empty_macro(macro_str) {
  int macro_id = init_ident(MACRO, macro_str);
  heap[add(macro_id, 3)] = cons(0, sub(0,1)); // -1 means it's an object-like macro, 0 means no tokens
  return macro_id;
}

function init_pnut_macros() {
  init_builtin_int_macro(mks("PNUT_CC"), 1);

  init_builtin_string_macro(mks("__DATE__"), mks("Jan  1 1970"));
  init_builtin_string_macro(mks("__TIME__"), mks("00:00:00"));
  init_builtin_string_macro(mks("__TIMESTAMP__"), mks("Jan  1 1970 00:00:00"));
  FILE__ID = init_builtin_string_macro(mks("__FILE__"), mks("<unknown>"));
  LINE__ID = init_builtin_int_macro(mks("__LINE__"), 0);

  init_builtin_int_macro(mks("PNUT_EXE"), 1);
  init_builtin_int_macro(mks("PNUT_EXE_32"), 1);
  init_builtin_int_macro(mks("PNUT_I386"), 1);
  init_builtin_int_macro(mks("PNUT_I386_LINUX"), 1);
  init_builtin_int_macro(mks("__linux__"), 1);
  init_builtin_int_macro(mks("__i386__"), 1);
}

// A macro argument is represented using a list of tokens.
// Macro arguments are split by commas, but commas can also appear in function
// calls and as operators. To distinguish between the two, we need to keep track
// of the parenthesis depth.
function macro_parse_argument() {
  var arg_tokens;
  var parens_depth;
  var tail;

  arg_tokens = 0;
  parens_depth = 0;

  while(and(or(gt(parens_depth,0),and(neq(tok,mkc(',')),neq(tok,mkc(')')))),neq(tok,EOF))) {
    if (eq(tok,mkc('('))) {
      parens_depth = add(parens_depth, 1); // Enter parenthesis
    }
    if (eq(tok,mkc(')'))) {
      parens_depth = sub(parens_depth, 1); // End of parenthesis
    }

    if (eq(arg_tokens, 0)) {
      arg_tokens = cons(cons(tok, val), 0);
      tail = arg_tokens;
    } else {
      set_cdr(tail, cons(cons(tok, val), 0));
      tail = cdr(tail);
    }
    get_tok_macro_expand();
  }

  return arg_tokens;
}

function check_macro_arity(macro_args_count, macro) {
  var expected_argc;
  expected_argc = cdr(r_heap(add(macro,3)));
  if (neq(macro_args_count, expected_argc)) {
    putstr(mks("expected_argc=")); putint(expected_argc);
    putstr(mks(" != macro_args_count=")); putint(macro_args_count);
    putchar(mkc('\n'));
    putstr(mks("macro=")); putstr(STRING_BUF(macro)); putchar(mkc('\n'));
    syntax_error(mks("macro argument count mismatch"));
  }
}

// Reads the arguments of a macro call, where the arguments are split by commas.
// Note that args are accumulated in reverse order, as the macro arguments refer
// to the tokens in reverse order.
function get_macro_args_toks(macro) {
  var args;
  var macro_args_count;
  var prev_is_comma;

  args = 0;
  macro_args_count = 0;
  prev_is_comma = eq(tok, mkc(','));

  get_tok_macro_expand(); // Skip '('

  while (and(neq(tok, mkc(')')), neq(tok, EOF))) {
    if (eq(tok, mkc(','))) {
      get_tok_macro_expand(); // Skip comma
      if (prev_is_comma) { // Push empty arg
        args = cons(0, args);
        macro_args_count = add(macro_args_count, 1);
      }
      prev_is_comma = true;
      continue;
    } else {
      prev_is_comma = false;
    }

    args = cons(macro_parse_argument(), args);
    macro_args_count = add(macro_args_count, 1);
  }

  if (neq(tok, mkc(')'))) {
    syntax_error(mks("unterminated macro argument list"));
  }

  if (prev_is_comma) {
    args = cons(0, args); // Push empty arg
    macro_args_count = add(macro_args_count, 1);
  }

  check_macro_arity(macro_args_count, macro);

  return args;
}

function get_macro_arg(ix) {
  var arg;
  arg = macro_args;
  while (gt(ix, 0)) {
    if (eq(arg, 0)) {
      syntax_error(mks("too few arguments to macro"));
    }
    arg = cdr(arg);
    ix = sub(ix, 1);
  }
  return car(arg);
}

// "Pops" the current macro expansion and restores the previous macro expansion context.
// This is done when the current macro expansion is done.
function return_to_parent_macro() {
  if (eq(macro_stack_ix, 0)) {
    fatal_error(mks("return_to_parent_macro: no parent macro"));
  }

  macro_stack_ix = sub(macro_stack_ix, 3);
  macro_tok_lst   = r_macro_stack(macro_stack_ix);
  macro_args      = r_macro_stack(add(macro_stack_ix, 1));
  macro_ident     = r_macro_stack(add(macro_stack_ix, 2));
}

// Begins a new macro expansion context, saving the current context onn the macro stack.
// Takes as argument the name of the macro, the tokens to be expanded and the arguments.
function begin_macro_expansion(ident, tokens, args) {
  if (gte(add(macro_stack_ix, 3), MACRO_RECURSION_MAX)) {
    fatal_error(mks("Macro recursion depth exceeded."));
  }

  macro_stack[macro_stack_ix]     = macro_tok_lst;
  macro_stack[add(macro_stack_ix, 1)] = macro_args;
  macro_stack[add(macro_stack_ix, 2)] = macro_ident;
  macro_stack_ix = add(macro_stack_ix, 3);

  macro_ident   = ident;
  macro_tok_lst = tokens;
  macro_args    = args;
}

// Search the macro stack to see if the macro is already expanding.
function macro_is_already_expanding(ident) {
  var i;
  i = macro_stack_ix;
  if (or(eq(ident, 0), eq(macro_ident, 0))) {
    return false; // Unnamed macro or no macro is expanding
  }
  if (eq(ident, macro_ident)) {
    return true;  // The same macro is already expanding
  }

  // Traverse the stack to see if the macro is already expanding
  while (gt(i, 0)) {
    i = sub(i, 3);
    if (eq(r_macro_stack(add(i, 2)), ident)) {
      return true;
    }
  }
  return false;
}

// Undoes the effect of get_tok by replacing the current token with the previous
// token and saving the current token to be returned by the next call to get_tok.
function undo_token(prev_tok, prev_val) {
  begin_macro_expansion(0, cons(cons(tok, val), 0), 0); // Push the current token back
  tok = prev_tok;
  val = prev_val;
}

// Try to expand a macro and returns if the macro was expanded.
// A macro is not expanded if it is already expanding or if it's a function-like
// macro that is not called with parenthesis. In that case, the macro identifier
// is returned as a normal identifier.
// If the wrong number of arguments is passed to a function-like macro, a fatal error is raised.
function attempt_macro_expansion(macro) {
  // We must save the tokens because the macro may be redefined while reading the arguments
  var tokens;
  tokens = car(r_heap(add(macro, 3)));

  if (macro_is_already_expanding(macro)) { // Self referencing macro
    tok = IDENTIFIER;
    val = macro;
    return false;
  } else if (eq(cdr(r_heap(add(macro,3))),sub(0,1))) { // Object-like macro
    // Note: Redefining __{FILE,LINE}__ macros, either with the #define or #line directives is not supported.
    if (eq(macro, FILE__ID)) {
      tokens = cons(cons(STRING, intern_str(fp_filepath)), 0);
    }
    else if (eq(macro, LINE__ID)) {
      tokens = cons(cons(INTEGER, sub(0, line_number)), 0);
    }
    begin_macro_expansion(macro, tokens, 0);
    return true;
  } else { // Function-like macro
    expect_tok(MACRO); // Skip macro identifier
    if (eq(tok, mkc('('))) {
      begin_macro_expansion(macro, tokens, get_macro_args_toks(macro));
      return true;
    } else {
      undo_token(IDENTIFIER, macro);
      return false;
    }
  }
}

// https://gcc.gnu.org/onlinedocs/cpp/Stringizing.html
function stringify() {
  var arg;
  expand_macro_arg = false;
  get_tok_macro();
  expand_macro_arg = true;
  if (neq(tok, MACRO_ARG)) {
    putstr(mks("tok=")); putint(tok); putchar(mkc('\n'));
    syntax_error(mks("expected macro argument after #"));
  }
  arg = get_macro_arg(val);
  tok = STRING;
  // Support the case where the argument is a single identifier/macro/keyword token
  if (and(or(or(eq(car(car(arg)),IDENTIFIER),eq(car(car(arg)),MACRO)),and(lte(AUTO_KW,car(car(arg))),lte(car(car(arg)),WHILE_KW))),eq(cdr(arg),0))) {
    val = cdr(car(arg)); // Use the identifier probe
  } else {
    val = NOT_SUPPORTED_ID; // Return string "NOT_SUPPORTED"
  }
}

// Concatenates two non-negative integers into a single integer
// Note that this function only supports small integers, represented as positive integers.
function paste_integers(left_val, right_val) {
  int result;
  int right_digits;
  result = left_val;
  right_digits = right_val;

  while (gt(right_digits, 0)) {
    result = mul(result, 10);
    right_digits = div_(right_digits, 10);
  }
  return add(result, right_val);
}

// Support token pasting between identifiers and non-negative integers
function paste_tokens(left_tok, left_val) {
  var right_tok;
  var right_val;
  expand_macro_arg = false;
  get_tok_macro();
  expand_macro_arg = true;
  // We need to handle the case where the right-hand side is a macro argument that expands to empty
  // In that case, the left-hand side is returned as is.
  if (eq(tok, MACRO_ARG)) {
    if (eq(get_macro_arg(val), 0)) {
      tok = left_tok;
      val = left_val;
      return;
    } else {
      begin_macro_expansion(0, get_macro_arg(val), 0); // Play the tokens of the macro argument
      get_tok_macro();
    }
  }
  right_tok = tok;
  right_val = val;
  if (or(or(eq(left_tok,IDENTIFIER),eq(left_tok,TYPE)),or(eq(left_tok,MACRO),lte(left_tok,WHILE_KW)))) {
    // Something that starts with an identifier can only be an identifier
    begin_string();
    accum_string_string(left_val);

    if (or(or(eq(right_tok,IDENTIFIER),eq(right_tok,TYPE)),or(eq(right_tok, MACRO),lte(right_tok,WHILE_KW)))) {
      accum_string_string(right_val);
    } else if (or(or(or(eq(right_tok,INTEGER),
               eq(right_tok,INTEGER_L)),or(eq(right_tok,INTEGER_LL),eq(right_tok,INTEGER_U))),or(eq(right_tok,INTEGER_UL),eq(right_tok,INTEGER_ULL))
              )) {
      accum_string_integer(sub(0,right_val));
    } else {
      putstr(mks("left_tok=")); putint(left_tok); putstr(mks(", right_tok=")); putint(right_tok); putchar(mkc('\n'));
      // show identifier/macro string
      putstr(mks("left=")); putstr(STRING_BUF(left_val)); putchar(mkc('\n'));
      syntax_error(mks("cannot paste an identifier with a non-identifier or non-negative integer"));
    }

    val = end_ident();
    tok = r_heap(add(val, 2)); // The kind of the identifier
  } else if (or(or(eq(left_tok,INTEGER),
             eq(left_tok,INTEGER_L)),or(or(eq(left_tok,INTEGER_LL),eq(left_tok,INTEGER_U)),or(eq(left_tok,INTEGER_UL),eq(left_tok,INTEGER_ULL))))
            ) {
    if (or(or(or(eq(right_tok,INTEGER),
        eq(right_tok,INTEGER_L)),or(eq(right_tok,INTEGER_LL),eq(right_tok,INTEGER_U))),or(eq(right_tok,INTEGER_UL),eq(right_tok,INTEGER_ULL)))
       ) {
      val = sub(0,paste_integers(sub(0,left_val), sub(0,right_val)));
    } else if (or(or(eq(right_tok,IDENTIFIER),eq(right_tok,MACRO)),lte(right_tok,WHILE_KW))) {
      begin_string();
      accum_string_integer(sub(0, left_val));
      accum_string_string(right_val);

      val = end_ident();
      tok = r_heap(add(val, 2)); // The kind of the identifier
    } else {
      putstr(mks("left_tok=")); putint(left_tok); putstr(mks(", right_tok=")); putint(right_tok); putchar(mkc('\n'));
      syntax_error(mks("cannot paste an integer with a non-integer"));
    }
  } else {
    putstr(mks("left_tok=")); putint(left_tok); putstr(mks(", right_tok=")); putint(right_tok); putchar(mkc('\n'));
    syntax_error(mks("cannot paste a non-identifier or non-integer"));
  }
}

function get_tok() {
  var prev_tok_line_number;
  var prev_tok_column_number;

  prev_tok_line_number = line_number;
  prev_tok_column_number = column_number;

  // This outer loop is used to skip over tokens removed by #ifdef/#ifndef/#else
  while(1) {
    while (1) {
      // Check if there are any tokens to replay. Macros are just identifiers that
      // have been marked as macros. In terms of how we get into that state, a
      // macro token is first returned by the get_ident call a few lines below.
      if (neq(macro_tok_lst, 0)) {
        tok = car(car(macro_tok_lst));
        val = cdr(car(macro_tok_lst));
        macro_tok_lst = cdr(macro_tok_lst);
        // Tokens that are identifiers and up are tokens whose kind can change
        // between the moment the macro is defined and where it is used.
        // So we reload the kind from the ident table.
        if (gte(tok, IDENTIFIER)) {
          tok = r_heap(add(val, 2));
        }

        // Check if the next token is ## for token pasting
        if (and(neq(macro_tok_lst,0),eq(car(car(macro_tok_lst)), HASH_HASH))) {
          if (or(eq(tok,MACRO),eq(tok,MACRO_ARG))) {
            // If the token is a macro or macro arg, it must be expanded before pasting
            macro_tok_lst = cdr(macro_tok_lst); // We consume the ## token
            paste_last_token = true;
          } else {
            // macro_tok_lst is not empty because read_macro_tokens checked for trailing ##
            macro_tok_lst = cdr(macro_tok_lst); // Skip the ##
            paste_tokens(tok, val);
          }
        } else if (and(eq(macro_tok_lst,0),paste_last_token)) { // We finished expanding the left-hand side of ##
          if (eq(macro_stack_ix, 0)) {
            // If we are not in a macro expansion, we can't paste the last token
            // This should not happen if the macro is well-formed, which is
            // checked by read_macro_tokens.
            syntax_error(mks("## cannot appear at the end of a macro expansion"));
          }
          return_to_parent_macro();
          paste_last_token = false; // We are done pasting
          paste_tokens(tok, val);
        }

        if (eq(tok, MACRO)) { // Nested macro expansion!
          if (attempt_macro_expansion(val)) {
            continue;
          }
          break;
        } else if (and(eq(tok, MACRO_ARG), expand_macro_arg)) {
          begin_macro_expansion(0, get_macro_arg(val), 0); // Play the tokens of the macro argument
          continue;
        } else if (eq(tok, mkc('#'))) { // Stringizing!
          stringify();
          break;
        }
        break;
      } else if (neq(macro_stack_ix, 0)) {
        return_to_parent_macro();
        continue;
      } else if (lte(ch, mkc(' '))) {

        if (eq(ch, EOF)) {
          tok = EOF;
          break;
        }

        // skip whitespace, detecting when it is at start of line.
        // When skip_newlines is false, produces a '\n' token whenever it
        // encounters whitespace containing at least a newline.
        // This condenses multiple newlines into a single '\n' token and serves
        // to end the current preprocessor directive.

        tok = 0; // Reset the token
        while (and(lte(0, ch), lte(ch, mkc(' ')))) {
          if (eq(ch, mkc('\n'))) {
            tok = ch;
          }
          get_ch();
        }

        if (and(eq(tok, mkc('\n')), eq(0, skip_newlines))) {
          // If the newline is followed by a #, the preprocessor directive is
          // handled in the next iteration of the loop.
          break;
        }

        // will continue while (1) loop
      }

      // detect '#' at start of line, possibly preceded by whitespace
      else if (and(eq(tok, mkc('\n')), eq(ch, mkc('#')))) {
        tok = 0; // Consume the newline so handle_preprocessor_directive's get_tok doesn't re-enter this case
        handle_preprocessor_directive();
        // will continue while (1) loop
      }

      else if (or(and(lte(mkc('a'), ch), lte(ch, mkc('z'))),
                  or(and(lte(mkc('A'), ch), lte(ch, mkc('Z'))),
                     eq(ch, mkc('_'))))) {

        get_ident();

        if (eq(tok, MACRO)) {
          // We only expand in ifdef true blocks and if the expander is enabled.
          // Since this is the "base case" of the macro expansion, we don't need
          // to disable the other places where macro expansion is done.
          if (if_macro_mask && expand_macro) {
            if (attempt_macro_expansion(val)) {
              continue;
            }
            break;
          }
        }
        break;
      } else if (and(lte(mkc('0'), ch), lte(ch, mkc('9')))) {

        val = INIT_ACCUM_DIGIT();

        tok = INTEGER;
        if (eq(ch, mkc('0'))) { // val == 0 <=> ch == '0'
          get_ch();
          if (or(eq(ch, mkc('x')), eq(ch, mkc('X')))) {
            get_ch();
            if (accum_digit(16)) {
              while (accum_digit(16));
            } else {
              syntax_error(mks("invalid hex integer -- it must have at least one digit"));
            }
          } else {
            while (accum_digit(8));
          }
        } else {
          while (accum_digit(10));
        }

        // If this is enabled with PARSE_NUMERIC_LITERAL_WITH_BASE, using a
        // suffix replaces INTEGER_OCT and INTEGER_HEX with base 10 INTEGER.
        if (or(eq(ch, mkc('u')), eq(ch, mkc('U')))) {
          // Note: allows suffixes with mixed case, such as lL for simplicity
          tok = INTEGER_U;
          get_ch();
          if (or(eq(ch, mkc('l')), eq(ch, mkc('L')))) {
            tok = INTEGER_UL;
            get_ch();
            if (or(eq(ch, mkc('l')), eq(ch, mkc('L')))) {
              tok = INTEGER_ULL;
              get_ch();
            }
          }
        } else if (or(eq(ch, mkc('l')), eq(ch, mkc('L')))) {
          tok = INTEGER_L;
          get_ch();
          if (or(eq(ch, mkc('l')), eq(ch, mkc('L')))) {
            tok = INTEGER_LL;
            get_ch();
          }
          if (or(eq(ch, mkc('u')), eq(ch, mkc('U')))) {
            if(eq(tok, INTEGER_LL)) {
              tok = INTEGER_ULL;
            } else {
              tok = INTEGER_UL;
            }
            get_ch();
          }
        } else if (or(eq(ch, mkc('f')), eq(ch, mkc('.')))) {
          get_ch();
          tok = INTEGER;
          while (accum_digit(10)); // Skip the fractional part
          val = 0; // Force the value to be 0 for now. TODO: Convert to float
        }

        break;

      } else if (eq(ch, mkc('\''))) {

        get_ch();
        get_string_char();

        if (neq(ch, mkc('\''))) {
          syntax_error(mks("unterminated character literal"));
        }

        get_ch();

        tok = CHARACTER;

        break;

      } else if (eq(ch, mkc('\"'))) {

        get_ch();

        begin_string();
        accum_string_until(mkc('\"'));

        val = end_string();
        tok = STRING;

        break;

      } else {

        tok = ch; // fallback for single char tokens

        if (eq(ch, mkc('/'))) {

          get_ch();
          if (eq(ch, mkc('*'))) {
            get_ch();
            tok = ch; // remember previous char, except first one
            while (and(or(neq(tok,mkc('*')),neq(ch,mkc('/'))),neq(ch,EOF))) {
              tok = ch;
              get_ch();
            }
            if (eq(ch, EOF)) {
              syntax_error(mks("unterminated comment"));
            }
            get_ch();
            // will continue while (1) loop
          } else if (eq(ch, mkc('/'))) {
            while (and(neq(ch, mkc('\n')), neq(ch, EOF))) {
              get_ch();
            }
            // will continue while (1) loop
          } else {
            if (eq(ch, mkc('='))) {
              get_ch();
              tok = SLASH_EQ;
            }
            break;
          }

        } else if (eq(ch, mkc('&'))) {

          get_ch();
          if (eq(ch, mkc('&'))) {
            get_ch();
            tok = AMP_AMP;
          } else if (eq(ch, mkc('='))) {
            get_ch();
            tok = AMP_EQ;
          }

          break;

        } else if (eq(ch, mkc('|'))) {

          get_ch();
          if (eq(ch, mkc('|'))) {
            get_ch();
            tok = BAR_BAR;
          } else if (eq(ch, mkc('='))) {
            get_ch();
            tok = BAR_EQ;
          }

          break;

        } else if (eq(ch, mkc('<'))) {

          get_ch();
          if (eq(ch, mkc('='))) {
            get_ch();
            tok = LT_EQ;
          } else if (eq(ch, mkc('<'))) {
            get_ch();
            if (eq(ch, mkc('='))) {
              get_ch();
              tok = LSHIFT_EQ;
            } else {
              tok = LSHIFT;
            }
          }

          break;

        } else if (eq(ch, mkc('>'))) {

          get_ch();
          if (eq(ch, mkc('='))) {
            get_ch();
            tok = GT_EQ;
          } else if (eq(ch, mkc('>'))) {
            get_ch();
            if (eq(ch, mkc('='))) {
              get_ch();
              tok = RSHIFT_EQ;
            } else {
              tok = RSHIFT;
            }
          }

          break;

        } else if (eq(ch, mkc('='))) {

          get_ch();
          if (eq(ch, mkc('='))) {
            get_ch();
            tok = EQ_EQ;
          }

          break;

        } else if (eq(ch, mkc('!'))) {

          get_ch();
          if (eq(ch, mkc('='))) {
            get_ch();
            tok = EXCL_EQ;
          }

          break;

        } else if (ch == '+') {

          get_ch();
          if (ch == '=') {
            get_ch();
            tok = PLUS_EQ;
          } else if (ch == '+') {
            get_ch();
            tok = PLUS_PLUS;
          }

          break;

        } else if (eq(ch, mkc('-'))) {

          get_ch();
          if (eq(ch, mkc('='))) {
            get_ch();
            tok = MINUS_EQ;
          } else if (eq(ch, mkc('>'))) {
            get_ch();
            tok = ARROW;
          } else if (eq(ch, mkc('-'))) {
            get_ch();
            tok = MINUS_MINUS;
          }

          break;

        } else if (eq(ch, mkc('*'))) {

          get_ch();
          if (eq(ch, mkc('='))) {
            get_ch();
            tok = STAR_EQ;
          }

          break;

        } else if (eq(ch, mkc('%'))) {

          get_ch();
          if (eq(ch, mkc('='))) {
            get_ch();
            tok = PERCENT_EQ;
          }

          break;

        } else if (eq(ch, mkc('^'))) {

          get_ch();
          if (eq(ch, mkc('='))) {
            get_ch();
            tok = CARET_EQ;
          }

          break;

        } else if (eq(ch, mkc('#'))) {

          get_ch();
          if (eq(ch, mkc('#'))) {
            get_ch();
            tok = HASH_HASH;
          }

          break;

        } else if (eq(ch, mkc('.'))) {
          get_ch();
          if (eq(ch, mkc('.'))) {
            get_ch();
            if (eq(ch, mkc('.'))) {
              get_ch();
              tok = ELLIPSIS;
            } else {
              syntax_error(mks("invalid token"));
            }
          } else {
            tok = mkc('.');
          }
          break;
        } else if (or(or(or(eq(ch,mkc('~')),eq(ch,mkc('.'))),or(or(eq(ch,mkc('?')),eq(ch,mkc(','))),or(eq(ch,mkc(':')),eq(ch,mkc(';'))))),or(or(or(eq(ch,mkc('(')),eq(ch,mkc(')'))),or(eq(ch,mkc('[')),eq(ch,mkc(']')))),or(eq(ch,mkc('{')),eq(ch,mkc('}')))))) {

          tok = ch;

          get_ch();

          break;

        } else if (eq(ch, mkc('\\'))) {
          get_ch();

          if (eq(ch, mkc('\n'))) { // Continues with next token
            get_ch();
          } else {
            putstr(mks("ch=")); putint(ch); putchar(mkc('\n'));
            syntax_error(mks("unexpected character after backslash"));
          }
        } else {
          putstr(mks("ch=")); putint(ch); putchar(mkc('\n'));
          syntax_error(mks("invalid token"));
        }
      }
    }
    if(if_macro_mask) {
      break;
    }
  }

  last_tok_line_number = prev_tok_line_number;
  last_tok_column_number = prev_tok_column_number;
}

function parse_error_internal(msg, token, file, line) {
  putstr(msg);
  putstr(mks("\n"));

  exit(1);
}

function parse_error(msg, token) {
  parse_error_internal(msg, token, __FILE__, __LINE__);
}

function expect_tok_(expected_tok, file, line) {
  if (neq(tok, expected_tok)) {
    putstr(mks("expected tok=")); putint(expected_tok);
    putstr(mks("\ncurrent tok=")); putint(tok); putchar(mkc('\n'));
    parse_error_internal(mks("unexpected token"), tok, file, line);
  }
  get_tok();
}

// The storage class specifier and type qualifier tokens are all between 300 (AUTO_KW) and 326 (VOLATILE_KW) so we store them as bits in an int.
function MK_TYPE_SPECIFIER(tok) {
  return shl(1, (sub(tok, AUTO_KW)));
}

function TEST_TYPE_SPECIFIER(specifier, tok) {
  return and(specifier, shl(1, sub(tok, AUTO_KW)));
}

function get_type_specifier(type_or_decl) {
  var td;
  while (1) {
    td = get_op(type_or_decl);
    if(eq(td, DECL)) {
      type_or_decl = get_child_(DECL, type_or_decl, 1);
    } else if (eq(td, mkc('['))) {
      type_or_decl = get_child_(mkc('['), type_or_decl, 0);
    } else if (eq(td, mkc('*'))) {
      type_or_decl = get_child_(mkc('*'), type_or_decl, 0);
    } else {
      return type_or_decl;
    }
  }
}

function pointer_type(parent_type, is_const) {
  var t;
  if(is_const) {
    t = MK_TYPE_SPECIFIER(CONST_KW);
  } else {
    t = 0;
  }
  return new_ast2(mkc('*'), t, parent_type);
}

function function_type(parent_type, params) {
  return new_ast3(mkc('('), parent_type, params, false);
}

function function_type1(parent_type, param1) {
  return new_ast3(mkc('('), parent_type, cons(param1, 0), 0);
}

function function_type2(parent_type, param1, param2) {
  return new_ast3(mkc('('), parent_type, cons(param1, cons(param2, 0)), 0);
}

function function_type3(parent_type, param1, param2, param3) {
  return new_ast3(mkc('('), parent_type, cons(param1, cons(param2, cons(param3, 0))), 0);
}

function make_variadic_func(func_type) {
  set_child(func_type, 2, true); // Set the variadic flag
  return func_type;
}

// Type and declaration parser
function is_type_starter(tok) {
  // Numeric types
  if(or(or(eq(tok,INT_KW),eq(tok,CHAR_KW)),or(eq(tok,SHORT_KW),eq(tok,LONG_KW)))) {
    return true;
  }
  // Void and floating point types
  if(or(eq(tok,VOID_KW),or(eq(tok,FLOAT_KW),eq(tok,DOUBLE_KW)))) {
    return true;
  }
  // Signedness
  if(or(eq(tok,SIGNED_KW),eq(tok,UNSIGNED_KW))) {
    return true;
  }
  // User defined types
  if(eq(tok,TYPE)) {
    return true;
  }
  // Type attributes
  if(or(eq(tok,CONST_KW),eq(tok,VOLATILE_KW))) {
    return true;
  }
  // Enum, struct, union
  if(or(eq(tok,ENUM_KW),or(eq(tok,STRUCT_KW),eq(tok,UNION_KW)))) {
    return true;
  }
  // Storage class specifiers are not always valid type starters in all
  // contexts, but we allow them here
  if(or(eq(tok,TYPEDEF_KW),or(eq(tok,STATIC_KW),or(eq(tok,AUTO_KW),or(eq(tok,REGISTER_KW),eq(tok,EXTERN_KW)))))) {
    return true;
  }
  if(eq(tok,INLINE_KW)) {
    return true;
  }
  return false;
}

function parse_enum() {
  var name;
  var ident;
  var result;
  var tail;
  var value;
  var next_value;
  var last_literal_type;

  result = 0;
  value = 0;
  next_value = 0;
  last_literal_type = INTEGER; // Default to decimal integer for enum values

  expect_tok(ENUM_KW);

  if (or(eq(tok, IDENTIFIER), eq(tok, TYPE))) {
    // When the enum keyword is used with an identifier that's typedefed, the typedef is ignored.
    name = new_ast0(IDENTIFIER, val);
    get_tok();
  } else {
    name = 0;
  }

  // Note: The parser doesn't distinguish between a reference to an enum type and a declaration.
  // If child#2 is 0, it's either a reference to a type or a forward declaration.
  if (eq(tok, mkc('{'))) {
    get_tok();

    while (neq(tok, mkc('}'))) {
      if (neq(tok, IDENTIFIER)) {
        parse_error(mks("identifier expected"), tok);
      }
      ident = new_ast0(IDENTIFIER, val);
      get_tok();

      if (eq(tok, mkc('='))) {
        get_tok();
        value = parse_assignment_expression();
        if (eq(value, 0)) {
          parse_error(mks("Enum value must be a constant expression"), tok);
        }

        if (and(neq(get_op(value), INTEGER)
        , and(neq(get_op(value),INTEGER_U), and(neq(get_op(value),INTEGER_UL),and(neq(get_op(value),INTEGER_ULL),
        and(neq(get_op(value),INTEGER_L), neq(get_op(value),INTEGER_LL))))))
           ) {
        value = new_ast0(last_literal_type, -eval_constant(value, false)); // negative value to indicate it's a small integer
        }
        next_value = sub(get_val(value), 1); // Next value is the current value + 1, but val is negative
      } else {
        value = new_ast0(last_literal_type, next_value);
        next_value = sub(next_value, 1);
      }

      if (eq(result, 0)) {
        result = cons(new_ast2(mkc('='), ident, value), 0);
        tail = result;
      } else {
        set_child(tail, 1, cons(new_ast2(mkc('='), ident, value), 0));
        tail = get_child_(LIST, tail, 1);
      }

      if (eq(tok, mkc(','))) {
        get_tok();
      } else {
        break;
      }
    }

    expect_tok(mkc('}'));

  }

  return new_ast3(ENUM_KW, 0, name, result); // child#0 is the storage-class specifiers and type qualifiers
}

function parse_struct_or_union(struct_or_union_tok) {
  var name;
  var type_specifier;
  var decl;
  var result;
  var tail;
  var ends_in_flex_array;

  result = 0;
  ends_in_flex_array = false;

  expect_tok(struct_or_union_tok);

  if (or(eq(tok, IDENTIFIER), eq(tok, TYPE))) {
    // When the struct/union keyword is used with an identifier that's typedefed, the typedef is ignored.
    name = new_ast0(IDENTIFIER, val);
    get_tok();
  } else {
    name = 0; // Unnamed struct
  }

  // Note: The parser doesn't distinguish between a reference to a struct/union type and a declaration.
  // If child#2 is 0, it's either a reference to a type or a forward declaration.
  if (eq(tok, mkc('{'))) {
    get_tok();

    while (neq(tok, mkc('}'))) {
      if (eq(0,is_type_starter(tok))) {
        parse_error(mks("type expected in struct declaration"), tok);
      }
      if (ends_in_flex_array) {
        parse_error(mks("flexible array member must be last"), tok);
      }
      type_specifier = parse_declaration_specifiers(false);

      // If the decl has no name, it's an anonymous struct/union member
      // and there can only be 1 declarator so not looping.
      if (eq(tok, mkc(';'))) {
        if (and(neq(get_op(type_specifier),ENUM_KW), and(neq(get_op(type_specifier),STRUCT_KW),neq(get_op(type_specifier),UNION_KW)))) {
          parse_error(mks("Anonymous struct/union member must be a struct or union type"), tok);
        }
        decl = new_ast3(DECL, 0, type_specifier, 0);

        if (eq(result, 0)) {
          result = cons(decl, 0);
          tail = result;
        } else {
          set_child(tail, 1, cons(decl, 0));
          tail = get_child_(LIST, tail, 1);
        }
      } else {
        while (1) {
          decl = parse_declarator(false, type_specifier);
          if (eq(result, 0)) {
            tail = result = cons(decl, 0);
          } else {
            set_child(tail, 1, cons(decl, 0));
            tail = get_child_(LIST, tail, 1);
          }

          if (eq(get_child_(DECL, decl, 1), VOID_KW)) {
            parse_error(mks("member with void type not allowed in struct/union"), tok);
          }
          if (and(eq(get_child_(DECL, decl, 1), mkc('[')), eq(get_child_(mkc('['), get_child_(DECL, decl, 1), 1), 0))) {
            // Set ends_in_flex_array if the type is an array with no size
            ends_in_flex_array = true;
            break;
          }
          if (eq(tok, mkc(','))) {
            get_tok();
          } else {
            break;
          }
        }
      }

      expect_tok(mkc(';'));
    }

    expect_tok(mkc('}'));
  }

  return new_ast3(struct_or_union_tok, 0, name, result); // child#0 is the storage-class specifiers and type qualifiers
}

function parse_type_specifier() {
  var type_specifier;
  type_specifier = 0;
  switch (tok) {
    case CHAR_KW:
    case INT_KW:
    case VOID_KW:
    case FLOAT_KW:
    case DOUBLE_KW:
      type_specifier = new_ast0(tok, 0);
      get_tok();
      return type_specifier;

    case SHORT_KW:
      get_tok();
      if (eq(tok, INT_KW)) {
        get_tok(); // Just "short" is equivalent to "short int"
      }
      return new_ast0(SHORT_KW, 0);

    case SIGNED_KW:
      get_tok();
      type_specifier = parse_type_specifier();
      // Just "signed" is equivalent to "signed int"
      if (eq(type_specifier, 0)) {
        type_specifier = new_ast0(INT_KW, 0);
      }
      return type_specifier;

    case UNSIGNED_KW:
      get_tok();
      type_specifier = parse_type_specifier();
      // Just "unsigned" is equivalent to "unsigned int"
      if (eq(type_specifier, 0)) {
        type_specifier = new_ast0(INT_KW, MK_TYPE_SPECIFIER(UNSIGNED_KW));
      }
      // Set the unsigned flag
      else {
        set_val(type_specifier, or(get_val(type_specifier),MK_TYPE_SPECIFIER(UNSIGNED_KW)));
      }
      return type_specifier;

    case LONG_KW:
      get_tok();
      if (eq(tok, DOUBLE_KW)) {
        get_tok();
        return new_ast0(DOUBLE_KW, 0);
      } else {
        if (eq(tok, LONG_KW)) {
          get_tok();
          if (eq(tok, INT_KW)) {
            get_tok(); // Just "long long" is equivalent to "long long int"
          }
          return new_ast0(LONG_KW, 0);
        } else if (eq(tok, INT_KW)) {
          get_tok(); // Just "long" is equivalent to "long int", which we treat as "int"
          return new_ast0(INT_KW, 0);
        } else {
          return new_ast0(INT_KW, 0);
        }
      }

    default:
      return 0;
  }
}

// A declaration is split in 2 parts:
//    1. specifiers and qualifiers
//    2. declarators and initializers
// This function parses the first part
// Storage class specifiers affect declarations instead of types, so it's easier to extract it from the type

var glo_specifier_storage_class = 0;

function parse_declaration_specifiers(allow_typedef) {
  var type_specifier;
  var type_qualifier;
  var loop;
  var specifier_storage_class;

  type_specifier = 0;
  type_qualifier = 0;
  loop = true;
  specifier_storage_class = 0;

  while (loop) {
    switch (tok) {
      case AUTO_KW:
      case REGISTER_KW:
      case STATIC_KW:
      case EXTERN_KW:
      case TYPEDEF_KW:
        if (neq(specifier_storage_class, 0)) {
          fatal_error(mks("Multiple storage classes not supported"));
        }
        if (and(eq(tok, TYPEDEF_KW), eq(0,allow_typedef))) {
          parse_error(mks("Unexpected typedef"), tok);
        }
        specifier_storage_class = tok;
        get_tok();
        break;

      case INLINE_KW:
        get_tok(); // Ignore inline
        break;

      case CONST_KW:
      case VOLATILE_KW:
        type_qualifier = or(type_qualifier, MK_TYPE_SPECIFIER(tok));
        get_tok();
        break;

      case CHAR_KW:
      case INT_KW:
      case VOID_KW:
      case SHORT_KW:
      case SIGNED_KW:
      case UNSIGNED_KW:
      case LONG_KW:
      case FLOAT_KW:
      case DOUBLE_KW:
        if (neq(type_specifier, 0)) {
          parse_error(mks("Unexpected C type specifier"), tok);
        }
        type_specifier = parse_type_specifier();
        if (eq(type_specifier, 0)) {
          parse_error(mks("Failed to parse type specifier"), tok);
        }
        break;

      case STRUCT_KW:
      case UNION_KW:
        if (neq(type_specifier, 0)) {
          parse_error(mks("Multiple types not supported"), tok);
        }
        type_specifier = parse_struct_or_union(tok);
        break;

      case ENUM_KW:
        if (neq(type_specifier, 0)) {
          parse_error(mks("Multiple types not supported"), tok);
        }
        type_specifier = parse_enum();
        break;

      case TYPE:
        if (neq(type_specifier, 0)) {
          parse_error(mks("Multiple types not supported"), tok);
        }
        // Lookup type in the types table. It is stored in the tag of the
        // interned string object. The type is cloned so it can be modified.
        type_specifier = clone_ast(heap[val + 3]);
        get_tok();
        break;

      default:
        loop = false; // Break out of loop
        break;
    }
  }

  // Note: Remove to support K&R C syntax
  if (eq(type_specifier, 0)) {
    parse_error(mks("Type expected"), tok);
  }

  if (neq(type_qualifier, 0)) {
    // This can only happen if an array/function type is typedef'ed
    if (or(eq(get_op(type_specifier),mkc('[')),eq(get_op(type_specifier),mkc('(')))) {
      parse_error(mks("Type qualifiers not allowed on typedef'ed array or function type"), tok);
    }

    // Set the type qualifier, keeping the storage class specifier from the typedef if it exists
    set_child(type_specifier, 0, or(get_child(type_specifier, 0),type_qualifier));
  }
  glo_specifier_storage_class = specifier_storage_class;

  return type_specifier;
}

var parse_param_list_is_variadic = false;

function parse_param_list() {
  var result;
  var tail;
  var decl;

  result = 0;
  parse_param_list_is_variadic = false;

  expect_tok(mkc('('));

  while (and(neq(tok, mkc(')')), neq(tok, EOF))) {
    if (is_type_starter(tok)) {
      decl = parse_declarator(true, parse_declaration_specifiers(false));
      if (eq(get_op(get_child_(DECL, decl, 1)), VOID_KW)) {
        if (or(neq(tok,mkc(')')), neq(result, 0))) {
          parse_error("void must be the only parameter", tok);
        }
        break;
      }
    } else if (eq(tok, IDENTIFIER)) {
      // Support K&R param syntax in function definition
      decl = new_ast3(DECL, new_ast0(IDENTIFIER, val), new_ast0(INT_KW, 0), 0);
      get_tok();
    } else if (eq(tok, ELLIPSIS)) {
      // ignore ELLIPSIS nodes for now, but it should be the last parameter
      if (eq(result, 0)) {
        parse_error(mks("Function must have a named parameter before ellipsis parameter"), tok);
      }
      get_tok();
      parse_param_list_is_variadic = true;
      break;
    } else {
      parse_error(mks("Parameter declaration expected"), tok);
    }

    if (eq(tok, mkc(','))) {
      get_tok();
    }

    if (eq(result, 0)) {
      result = cons(decl, 0);
      tail = result;
    } else {
      set_child(tail, 1, cons(decl, 0));
      tail = get_child_(LIST, tail, 1);
    }
  }

  expect_tok(mkc(')'));

  return result;
}

function get_inner_type(type) {
  var t;
  t = get_op(type);
  if(or(eq(t, DECL), eq(t, mkc('*')))) {
    return get_child(type, 1);
  } else if(or(eq(t, mkc('[')), eq(t, mkc('(')))) {
    return get_child(type, 0);
  } else {
    fatal_error(mks("Invalid type"));
    return 0;
  }
}

function update_inner_type(parent_type, inner_type) {
  var t;
  t = get_op(parent_type);
  if(or(eq(t, DECL), eq(t, mkc('*')))) {
    set_child(parent_type, 1, inner_type);
  } else if(or(eq(t, mkc('[')), eq(t, mkc('(')))) {
    set_child(parent_type, 0, inner_type);
  } else {
    return 0;
  }
}

// Parse a declarator. In C, declarators are written as they are used, meaning
// that the identifier appears inside the declarator, and is surrounded by the
// operators that are used to access the declared object.
//
// When manipulating declarator and type objects, it's much more convenient to
// have the identifier as the outermost node, and the order of the operators
// reversed, ending with the type specifier (base type).
// For example, `int *a[10]` is parsed as `(decl a (array 10 (pointer int)))`
// even if the parser parses `int`, `*`, identifier and `[10]` in that order.
//
// To achieve this, parse_declarator takes the inner type as an argument, and
// the inner type is extended as the declarator is parsed. The parent_type is
// then used in the declarator base case, the identifier, and which
// creates the DECL node.
//
// There's a small twist to this however, caused by array and function
// declarators appearing postfixed to the declarator. Because tokens are only
// read once, we can't skip ahead to expand the inner type with array/function
// declarator and then recursively call parse_declarator with the extended type.
// Instead, parse_declarator keeps track of the node that wraps the inner type
// and returns it in `parse_declarator_parent_type_parent`. Using the reference
// to the node containing the inner type, it is then possible to insert the
// array/function declarator in the right location, that is around the inner
// type.
//
// Parameters: abstract_decl: true if the declarator may omit the identifier
var parse_declarator_parent_type_parent;

function parse_declarator(abstract_decl, parent_type) {
  var first_tok;
  var result;
  var decl;
  var arr_size_expr;
  var parent_type_parent;

  first_tok = tok; // Indicates if the declarator is a noptr-declarator
  result = 0;

  if(eq(tok, IDENTIFIER)) {
    result = new_ast3(DECL, new_ast0(IDENTIFIER, val), parent_type, 0); // child#2 is the initializer
    parent_type_parent = result;
    get_tok();
  } else if(eq(tok, mkc('*'))) {
    get_tok();
    // Pointers may be const-qualified
    parent_type_parent = pointer_type(parent_type, eq(tok, CONST_KW));
    if (eq(tok, CONST_KW)) {
      get_tok();
    }
    result = parse_declarator(abstract_decl, parent_type_parent);
  } else if(eq(tok, mkc('('))) {
    // Parenthesis delimit the specifier-and-qualifier part of the declaration from the declarator
    get_tok();
    result = parse_declarator(abstract_decl, parent_type);
    parent_type_parent = parse_declarator_parent_type_parent;
    expect_tok(mkc(')'));
  } else {
    // Abstract declarators don't need names, and so in the base declarator,
    // we don't require an identifier. This is useful for function pointers.
    // In that case, we create a DECL node with no identifier.
    if (abstract_decl) {
      result = new_ast3(DECL, 0, parent_type, 0); // child#0 is the identifier, child#2 is the initializer
      parent_type_parent = result;
    } else {
      parse_error(mks("Invalid declarator, expected an identifier but declarator doesn't have one"), tok);
    }
  }

  // At this point, the only non-recursive declarator is an identifier
  // so we know that get_op(result) == DECL.
  // Because we want the DECL to stay as the outermost node, we temporarily
  // unwrap the DECL parent_type.
  decl = result;
  result = get_child_(DECL, decl, 1);

  while (neq(first_tok, mkc('*'))) {
    // noptr-declarator may be followed by [ constant-expression ] to declare an
    // array or by ( parameter-type-list ) to declare a function. We loop since
    // both may be present.
    if (eq(tok, mkc('['))) {
      // Check if not a void array
      if (eq(get_op(result), VOID_KW)) {
        parse_error(mks("void array not allowed"), tok);
      }
      get_tok();
      if (eq(tok, mkc(']'))) {
        val = 0;
      } else {
        arr_size_expr = parse_assignment_expression();
        if (eq(arr_size_expr, 0)) {
          parse_error(mks("Array size must be an integer constant"), tok);
        }
        val = eval_constant(arr_size_expr, false);
      }
      result = new_ast2(mkc('['), get_inner_type(parent_type_parent), val);
      update_inner_type(parent_type_parent, result);
      parent_type_parent = result;
      expect_tok(mkc(']'));
    } else if (eq(tok, mkc('('))) {
      result = new_ast3(mkc('('), get_inner_type(parent_type_parent), parse_param_list(), false);
      if (parse_param_list_is_variadic) {
        result = make_variadic_func(result);
      }
      update_inner_type(parent_type_parent, result);
      parent_type_parent = result;
    } else {
      break;
    }
  }

  parse_declarator_parent_type_parent = parent_type_parent;
  return decl;
}

function parse_initializer_list() {
  var result;
  var tail;

  result = 0;
  tail = 0;

  expect_tok(mkc('{'));

  while (and(neq(tok, mkc('}')), neq(tok, EOF))) {
    if (result == 0) {
      result = cons(parse_initializer(), 0);
      tail = result;
    } else {
      set_child(tail, 1, cons(parse_initializer(), 0));
      tail = get_child_(LIST, tail, 1);
    }
    if (eq(tok, mkc(','))) {
      get_tok();
    } else {
      break;
    }
  }

  expect_tok(mkc('}'));

  return new_ast1(INITIALIZER_LIST, result);
}

function parse_initializer() {
  if (eq(tok, mkc('{'))) {
    return parse_initializer_list();
  } else {
    return parse_assignment_expression();
  }
}

function parse_declarator_and_initializer(is_for_typedef, type_specifier) {
  var declarator;
  declarator = parse_declarator(false, type_specifier);

  if (eq(is_for_typedef, 0)) {
    if (eq(tok, mkc('='))) {
      get_tok();
      // parse_declarator returns a DECL node where the initializer is child#2
      set_child(declarator, 2, parse_initializer());
    }
  }

  return declarator;
}

function parse_declarators(is_for_typedef, type_specifier, first_declarator) {
  var declarators;
  var tail;

  declarators = cons(first_declarator, 0); // Wrap the declarators in a list
  tail = declarators;

  // Otherwise, this is a variable or declaration
  while (neq(tok, mkc(';'))) {
    if (eq(tok, mkc(','))) {
      get_tok();
      set_child(tail, 1, cons(parse_declarator_and_initializer(is_for_typedef, type_specifier), 0));
      tail = get_child__(LIST, LIST, tail, 1);
    } else {
      parse_error(mks("';' or ',' expected"), tok);
    }
  }

  return declarators;
}

function add_typedef(declarator) {
  var decl_ident;
  var decl_type;

  decl_ident = get_val_(IDENTIFIER, get_child__(DECL, IDENTIFIER, declarator, 0));
  decl_type = get_child_(DECL, declarator, 1); // child#1 is the type

  heap[add(decl_ident, 2)] = TYPE;
  heap[add(decl_ident, 3)] = decl_type;
}

function parse_fun_def(declarator) {
  var fun_type;
  var params;

  fun_type = get_child__(DECL, '(', declarator, 1);
  params = get_child_('(', fun_type, 1);

  // Check that the parameters are all named since declarator may be abstract
  while (neq(params, 0)) {
    if (eq(get_child_(DECL, get_child__(LIST, DECL, params, 0), 0), 0)) {
      parse_error("Parameter name expected", tok);
    }
    params = get_child_(LIST, params, 1);
  }
  if (neq(get_child_(DECL, declarator, 2), 0)) {
    parse_error(mks("Initializer not allowed in function definition"), tok);
  }
  return new_ast2(FUN_DECL, declarator, parse_compound_statement());
}

function parse_declaration(local) {
  var result;
  var declarator;
  var declarators;
  var type_specifier;

  // First we parse the specifiers:
  type_specifier = parse_declaration_specifiers(true);

  // From cppreference:
  // > The enum, struct, and union declarations may omit declarators, in which
  // > case they only introduce the enumeration constants and/or tags.
  if (eq(tok, mkc(';'))) {
    if(and(neq(get_op(type_specifier),ENUM_KW),and(neq(get_op(type_specifier),STRUCT_KW),neq(get_op(type_specifier),UNION_KW)))) {
      parse_error(mks("enum/struct/union declaration expected"), tok);
    }
    // If the specifier is a typedef, we add the typedef'ed type to the type table
    // Note: Should this return a DECL node instead of a ENUM, STRUCT, or UNION node?
    // It doesn't have a name so maybe it makes more sense to have a separate node type?
    if (eq(glo_specifier_storage_class, TYPEDEF_KW)) {
      add_typedef(new_ast3(DECL, 0, type_specifier, 0));
    }
    result = type_specifier;
  } else if (eq(glo_specifier_storage_class, TYPEDEF_KW)) {
    // The type_specifier contained a typedef, it can't be a function or a
    // variable declaration, and the declarators cannot be initialized.
    // The typedef'ed types will be added to the type table.
    declarator = parse_declarator_and_initializer(true, type_specifier); // First declarator
    declarators = parse_declarators(true, type_specifier, declarator);
    type_specifier = declarators; // Save declarators in type_specifier
    while (neq(declarators, 0)) {
      add_typedef(get_child__(LIST, DECL, declarators, 0));
      declarators = get_child_opt_(LIST, LIST, declarators, 1);
    }
    result = new_ast1(TYPEDEF_KW, type_specifier);
  } else {
    // Then we parse the declarators and initializers
    declarator = parse_declarator_and_initializer(false, type_specifier);

    // The declarator may be a function definition, in which case we parse the function body
    if(and(eq(get_op(get_child_(DECL,declarator,1)),mkc('(')),eq(tok,mkc('{')))) {
      if (local) {
        parse_error(mks("Function definition not allowed in local scope"), tok);
      }
      return parse_fun_def(declarator);
    }

    declarators = parse_declarators(false, type_specifier, declarator);
    result = new_ast2(DECLS, declarators, glo_specifier_storage_class); // child#1 is the storage class specifier
  }

  expect_tok(mkc(';'));
  return result;
}

function parse_parenthesized_expression() {

  var result;

  expect_tok(mkc('('));

  result = parse_comma_expression();

  expect_tok(mkc(')'));

  return new_ast1(PARENS, result);
}

function parse_primary_expression() {
  var result;
  var tail;

  result = 0;

  if (or(eq(tok,IDENTIFIER),or(eq(tok,CHARACTER),or(eq(tok,INTEGER),
     or(eq(tok,INTEGER_L),or(eq(tok,INTEGER_LL),or(eq(tok,INTEGER_U),or(eq(tok,INTEGER_UL),eq(tok,INTEGER_ULL)
     )))))))) {

    result = new_ast0(tok, val);
    get_tok();

  } else if (eq(tok, STRING)) {
    result = new_ast0(STRING, val);
    get_tok();

    if (eq(tok, STRING)) { // Contiguous strings
      result = cons(get_val_(STRING, result), 0); // Result is now a list of string values
      tail = result;
      while (eq(tok, STRING)) {
        set_cdr(tail, cons(val, 0));
        tail = cdr(tail);
        get_tok();
      }

      // Unpack the list of strings into a single string
      begin_string();

      while (neq(result, 0)) {
        accum_string_string(car(result));
        result = cdr(result);
      }

      result = new_ast0(STRING, end_string());
    }

  } else if (eq(tok, mkc('('))) {

    result = parse_parenthesized_expression();

  } else {
    parse_error(mks("identifier, literal, or '(' expected"), tok);
  }

  return result;
}

function parse_postfix_expression() {
  var result;
  var child;

  result = parse_primary_expression();

  while (1) {
    if (eq(tok, mkc('['))) {

      get_tok();
      child = parse_comma_expression();
      result = new_ast2(mkc('['), result, child);
      expect_tok(mkc(']'));

    } else if (tok == '(') {

      get_tok();
      if (eq(tok, mkc(')'))) {
        child = 0;
      } else {
        child = parse_call_params();
      }
      result = new_ast2(mkc('('), result, child);
      expect_tok(mkc(')'));

    } else if (eq(tok, mkc('.'))) {

      get_tok();
      if (neq(tok, IDENTIFIER)) {
        parse_error(mks("identifier expected"), tok);
      }
      result = new_ast2(mkc('.'), result, new_ast0(IDENTIFIER, val));
      get_tok();

    } else if (eq(tok, ARROW)) {

      get_tok();
      if (neq(tok, IDENTIFIER)) {
        parse_error(mks("identifier expected"), tok);
      }
      result = new_ast2(ARROW, result, new_ast0(IDENTIFIER, val));
      get_tok();

    } else if (eq(tok, PLUS_PLUS)) {

      get_tok();
      result = new_ast1(PLUS_PLUS_POST, result);

    } else if (eq(tok, MINUS_MINUS)) {

      get_tok();
      result = new_ast1(MINUS_MINUS_POST, result);

    } else {
      break;
    }
  }

  return result;
}

function parse_unary_expression() {
  var result;
  var op;

  if (eq(tok, PLUS_PLUS)) {

    get_tok();
    result = parse_unary_expression();
    result = new_ast1(PLUS_PLUS_PRE, result);

  } else if (eq(tok, MINUS_MINUS)) {

    get_tok();
    result = parse_unary_expression();
    result = new_ast1(MINUS_MINUS_PRE, result);

  } else if(or(eq(tok,mkc('&')),or(eq(tok,mkc('*')),or(eq(tok,mkc('+')),or(eq(tok,mkc('-')),or(eq(tok,mkc('~')),eq(tok,mkc('!')))))))) {

    op = tok;
    get_tok();
    result = parse_cast_expression();
    result = new_ast1(op, result);

  } else if (and(skip_newlines, eq(tok, SIZEOF_KW))) { // only parse sizeof if we're not in a #if expression

    get_tok();
    if (eq(tok, mkc('('))) {
      get_tok();
      // May be a type or an expression
      if (is_type_starter(tok)) {
      result = parse_declarator(true, parse_declaration_specifiers(false));
      expect_tok(mkc(')'));
      } else {
        // We need to put the current token and '(' back on the token stream.
        // Otherwise, sizeof (cast_expression) fails to parse.
        undo_token(mkc('('), 0);
        result = parse_unary_expression();
      }
    } else {
      result = parse_unary_expression();
    }
    result = new_ast1(SIZEOF_KW, result);

  } else if(and(eq(0,skip_newlines),and(eq(tok,IDENTIFIER),eq(val,DEFINED_ID)))) { // Parsing a macro

    get_tok_macro();
    if (eq(tok, mkc('('))) {
      get_tok_macro();
      result = new_ast2(mkc('('), new_ast0(IDENTIFIER, DEFINED_ID), tok);
      get_tok_macro();
      expect_tok(mkc(')'));
    } else if (tok == IDENTIFIER || tok == MACRO) {
      result = new_ast2(mkc('('), new_ast0(IDENTIFIER, DEFINED_ID), tok);
      get_tok_macro();
    } else {
      parse_error(mks("identifier or '(' expected"), tok);
      return 0;
    }

  } else {
    result = parse_postfix_expression();
  }

  return result;
}

function parse_cast_expression() {
  var result;
  var type;

  if (eq(tok, mkc('('))) {
    // Ideally, we'd parse as many ( as needed, but then we would have to
    // backtrack when the first parenthesis is for a parenthesized expression
    // and not a cast.
    // I think we could write a version of parse_parenthesized_expression that
    // already has the first parenthesis consumed. It would be called when
    // after parsing the cast and cast expression, there are still parenthesis
    // to close, but I'm not sure how we could create the AST since it's all
    // very top down and that would flip the order of the AST creation.

    // Concretely, this means we can't parse cast expressions where the type
    // is wrapped in parenthesis, like in the following example:
    // (((char *)) var)
    // But that should be ok for TCC.
    get_tok();

    if (is_type_starter(tok)) {
      type = parse_declarator(true, parse_declaration_specifiers(false));

      expect_tok(mkc(')'));
      result = new_ast2(CAST, type, parse_cast_expression());
      return result;
    } else {
      // We need to put the current token and '(' back on the token stream.
      undo_token(mkc('('), 0);
    }
  }

  return parse_unary_expression();
}

function parse_multiplicative_expression() {
  var result;
  var child;
  var op;

  result = parse_cast_expression();

  while(or(eq(tok,mkc('*')),or(eq(tok,mkc('/')),eq(tok,mkc('%'))))) {

    op = tok;
    get_tok();
    child = parse_cast_expression();
    result = new_ast2(op, result, child);

  }

  return result;
}

function parse_additive_expression() {
  var result;
  var child;
  var op;

  result = parse_multiplicative_expression();

  while (or(eq(tok,mkc('+')),eq(tok,mkc('-')))) {

    op = tok;
    get_tok();
    child = parse_multiplicative_expression();
    result = new_ast2(op, result, child);

  }

  return result;
}

function parse_shift_expression() {
  var result;
  var child;
  var op;

  result = parse_additive_expression();

  while (or(eq(tok, LSHIFT), eq(tok, RSHIFT))) {

    op = tok;
    get_tok();
    child = parse_additive_expression();
    result = new_ast2(op, result, child);

  }

  return result;
}

function parse_relational_expression() {
  var result;
  var child;
  var op;

  result = parse_shift_expression();

  while(or(eq(tok,mkc('<')),or(eq(tok,mkc('>')),or(eq(tok,LT_EQ),eq(tok,GT_EQ))))) {

    op = tok;
    get_tok();
    child = parse_shift_expression();
    result = new_ast2(op, result, child);

  }

  return result;
}

function parse_equality_expression() {
  var result;
  var child;
  var op;

  result = parse_relational_expression();

  while (or(eq(tok,EQ_EQ), eq(tok, EXCL_EQ))) {

    op = tok;
    get_tok();
    child = parse_relational_expression();
    result = new_ast2(op, result, child);

  }

  return result;
}

function parse_AND_expression() {
  var result;
  var child;

  result = parse_equality_expression();

  while (eq(tok, mkc('&'))) {

    get_tok();
    child = parse_equality_expression();
    result = new_ast2(mkc('&'), result, child);

  }

  return result;
}

function parse_exclusive_OR_expression() {
  var result;
  var child;

  result = parse_AND_expression();

  while (eq(tok, mkc('^'))) {

    get_tok();
    child = parse_AND_expression();
    result = new_ast2(mkc('^'), result, child);

  }

  return result;
}

function parse_inclusive_OR_expression() {
  var result;
  var child;

  result = parse_exclusive_OR_expression();

  while (eq(tok, mkc('|'))) {

    get_tok();
    child = parse_exclusive_OR_expression();
    result = new_ast2('|', result, child);

  }

  return result;
}

function parse_logical_AND_expression() {
  var result;
  var child;

  result = parse_inclusive_OR_expression();

  while (eq(tok, AMP_AMP)) {

    get_tok();
    child = parse_inclusive_OR_expression();
    result = new_ast2(AMP_AMP, result, child);

  }

  return result;
}

function parse_logical_OR_expression() {
  var result;
  var child;

  result = parse_logical_AND_expression();

  while (eq(tok, BAR_BAR)) {

    get_tok();
    child = parse_logical_AND_expression();
    result = new_ast2(BAR_BAR, result, child);

  }

  return result;
}

function parse_conditional_expression() {
  var result;
  var child1;
  var child2;

  result = parse_logical_OR_expression();

  if (eq(tok, mkc('?'))) {

    get_tok();
    child1 = parse_comma_expression();
    expect_tok(mkc(':'));
    child2 = parse_conditional_expression();
    result = new_ast3(mkc('?'), result, child1, child2);

  }

  return result;
}

function parse_assignment_expression() {
  var result;
  var child;
  var op;

  result = parse_conditional_expression();

  if (or(eq(tok, mkc('=')), or(eq(tok, PLUS_EQ), or(eq(tok, MINUS_EQ),
         or(eq(tok, STAR_EQ), or(eq(tok, SLASH_EQ), or(eq(tok, PERCENT_EQ),
         or(eq(tok, LSHIFT_EQ), or(eq(tok, RSHIFT_EQ), or(eq(tok, AMP_EQ),
         or(eq(tok, CARET_EQ), eq(tok, BAR_EQ)))))))))))) {

    op = tok;
    get_tok();
    child = parse_assignment_expression();
    result = new_ast2(op, result, child);

  }

  return result;
}

function parse_comma_expression() {
  var result;

  result = parse_assignment_expression();

  if (eq(tok, mkc(','))) { // "comma expressions" without , don't need to be wrapped in a comma node
    get_tok();
    result = new_ast2(mkc(','), result, 0);
    set_child(result, 1, parse_comma_expression());
  }

  return result;
}

function parse_call_params() {
  var result;

  result = parse_assignment_expression();
  result = new_ast2(LIST, result, 0);

  if (eq(tok, mkc(','))) {
    get_tok();
    set_child(result, 1, parse_call_params());
  }

  return result;
}

function parse_comma_expression_opt() {
  var result;

  if (or(eq(tok, mkc(':')), or(eq(tok, mkc(';')), eq(tok, mkc(')'))))) {
    result = 0;
  } else {
    result = parse_comma_expression();
  }

  return result;
}

function parse_expression() {
  return parse_comma_expression();
}

function parse_constant_expression() {
  return parse_expression();
}

function parse_statement() {
  var result;
  var child1;
  var child2;
  var child3;
  var start_tok;

  if (eq(tok, IF_KW)) {

    get_tok();
    result = parse_parenthesized_expression();
    child1 = parse_statement();

    if (eq(tok, ELSE_KW)) {
      get_tok();
      child2 = parse_statement();
    } else {
      child2 = 0;
    }

    result = new_ast3(IF_KW, result, child1, child2);

  } else if (eq(tok, SWITCH_KW)) {

    get_tok();
    result = parse_parenthesized_expression();
    child1 = parse_statement();

    result = new_ast2(SWITCH_KW, result, child1);

  } else if (eq(tok, CASE_KW)) {

    get_tok();
    result = parse_constant_expression();
    expect_tok(mkc(':'));
    child1 = parse_statement();

    result = new_ast2(CASE_KW, result, child1);

  } else if (eq(tok, DEFAULT_KW)) {

    get_tok();
    expect_tok(mkc(':'));
    result = parse_statement();

    result = new_ast1(DEFAULT_KW, result);

  } else if (eq(tok, WHILE_KW)) {

    get_tok();
    result = parse_parenthesized_expression();
    child1 = parse_statement();

    result = new_ast2(WHILE_KW, result, child1);

  } else if (eq(tok, DO_KW)) {

    get_tok();
    result = parse_statement();
    expect_tok(WHILE_KW);
    child1 = parse_parenthesized_expression();
    expect_tok(mkc(';'));

    result = new_ast2(DO_KW, result, child1);

  } else if (eq(tok, FOR_KW)) {

    get_tok();
    expect_tok(mkc('('));
    result = parse_comma_expression_opt();
    expect_tok(mkc(';'));
    child1 = parse_comma_expression_opt();
    expect_tok(mkc(';'));
    child2 = parse_comma_expression_opt();
    expect_tok(mkc(')'));
    child3 = parse_statement();

    result = new_ast4(FOR_KW, result, child1, child2, child3);

  } else if (eq(tok, GOTO_KW)) {

    get_tok();
    expect_tok(IDENTIFIER);
    result = new_ast1(GOTO_KW, new_ast0(IDENTIFIER, val));
    expect_tok(mkc(';'));

  } else if (eq(tok, CONTINUE_KW)) {

    get_tok();
    expect_tok(mkc(';'));

    result = new_ast0(CONTINUE_KW, 0);

  } else if (eq(tok, BREAK_KW)) {

    get_tok();
    expect_tok(mkc(';'));

    result = new_ast0(BREAK_KW, 0);

  } else if (eq(tok, RETURN_KW)) {

    get_tok();
    result = parse_comma_expression_opt();
    expect_tok(mkc(';'));

    result = new_ast1(RETURN_KW, result);

  } else if (eq(tok, mkc('{'))) {

    result = parse_compound_statement();

  } else {

    start_tok = tok;

    result = parse_comma_expression_opt();

    if(and(eq(tok,mkc(':')),and(neq(start_tok,mkc('(')),eq(get_op(result),IDENTIFIER)))) {

      get_tok(); // Skip :

      child1 = parse_statement();

      result = new_ast2(mkc(':'), result, child1);

    } else {

      expect_tok(mkc(';'));

    }
  }

  return result;
}

function parse_compound_statement() {
  var result;
  var child1;
  var tail;

  result = 0;

  expect_tok(mkc('{'));

  // TODO: Simplify this
  if (and(neq(tok, mkc('}')), neq(tok, EOF))) {
    if (is_type_starter(tok)) {
      child1 = parse_declaration(true);
    } else {
      child1 = parse_statement();
    }
    result = new_ast2(mkc('{'), child1, 0);
    tail = result;
    while (and(neq(tok, mkc('}')), neq(tok, EOF))) {
      if (is_type_starter(tok)) {
        child1 = parse_declaration(true);
      } else {
        child1 = parse_statement();
      }
      child1 = new_ast2(mkc('{'), child1, 0);
      set_child(tail, 1, child1);
      tail = child1;
    }
  }

  expect_tok(mkc('}'));

  return result;
}

//-----------------------------------------------------------------------------

// Select code generator

// start x86.c
var WORD_SIZE = 4;

// x86 codegen
// start exe.c


// 1MB heap
var RT_HEAP_SIZE = 104857600;

#define MAX_CODE_SIZE 5000000
var code[MAX_CODE_SIZE];
var code_alloc = 0;

function r_code(o) {
  return ri32(add(code,mul(4,o)));
}

function emit_i8(a) {
  if (gte(code_alloc, MAX_CODE_SIZE)) {
    fatal_error(mks("code buffer overflow"));
  }
  code[code_alloc] = and(a, 0xFF);
  code_alloc = add(code_alloc, 1);
}

function emit_2_i8(a, b) {
  emit_i8(a);
  emit_i8(b);
}

function emit_4_i8(a, b, c, d) {
  emit_2_i8(a, b);
  emit_2_i8(c, d);
}

function emit_i32_le(n) {
  emit_4_i8(n, shr(n, 8), shr(n, 16), shr(n, 24));
}

char write_buf[1];

function write_i8(n) {
  wi8(write_buf, and(n, 255));
  write(output_fd, write_buf, 1);
}

function write_2_i8(a, b) {
  write_i8(a);
  write_i8(b);
}

function write_4_i8(a, b, c, d) {
  write_2_i8(a, b);
  write_2_i8(c, d);
}

function write_i32_le(n) {
  write_4_i8(n, shr(n, 8), shr(n, 16), shr(n, 24));
}

// If the main function returns a value
var main_returns = false;

// Environment tracking
var cgc_fs = 0;
// Function bindings that follows lexical scoping rules
var cgc_locals = 0;
// Like cgc_locals, but with 1 scope for the entire function
var cgc_locals_fun = 0;
// Global bindings
var cgc_globals = 0;
// Bump allocator used to allocate static objects
var cgc_global_alloc = 0;

enum BINDING {
  // Because function params, local and global variables all share the same
  // namespace and we want to find the first one of them, we need to keep
  // BINDING_PARAM_LOCAL, BINDING_VAR_LOCAL and BINDING_VAR_GLOBAL
  // in consecutive order.
  BINDING_PARAM_LOCAL,
  BINDING_VAR_LOCAL,
  BINDING_VAR_GLOBAL,
  BINDING_ENUM_CST,
  BINDING_LOOP,
  BINDING_SWITCH,
  BINDING_FUN,
  BINDING_GOTO_LABEL,
  BINDING_TYPE_STRUCT,
  BINDING_TYPE_UNION,
  BINDING_TYPE_ENUM,
};

// Some small accessor for the bindings
// All bindings have a next pointer and a kind.
// Most have an identifier, but not all
function binding_next(binding) {
  return r_heap(binding);
}

function binding_kind(binding) {
  return r_heap(add(binding, 1));
}

function binding_ident(binding) {
  return r_heap(add(binding, 2));
}

function cgc_lookup_binding_ident(binding_type, ident, binding) {
  while (neq(binding, 0)) {
    if(and(eq(binding_kind(binding),binding_type),eq(binding_ident(binding),ident))) {
      break;
    }
    binding = binding_next(binding);
  }
  return binding;
}

function cgc_lookup_last_binding(binding_type, binding) {
  while (neq(binding, 0)) {
    if (eq(binding_kind(binding), binding_type)) {
      break;
    }
    binding = binding_next(binding);
  }
  return binding;
}

function cgc_lookup_var(ident, binding) {
  while (neq(binding, 0)) {
    if (and(lte(binding_kind(binding),BINDING_VAR_GLOBAL),eq(binding_ident(binding),ident))) {
      break;
    }
    binding = binding_next(binding);
  }
  return binding;
}

function cgc_lookup_fun(ident, env) {
  return cgc_lookup_binding_ident(BINDING_FUN, ident, env);
}

function cgc_lookup_enclosing_loop(env) {
  return cgc_lookup_last_binding(BINDING_LOOP, env);
}

function cgc_lookup_enclosing_switch(env) {
  return cgc_lookup_last_binding(BINDING_SWITCH, env);
}

function cgc_lookup_enclosing_loop_or_switch(binding) {
  while (neq(binding, 0)) {
    if(or(eq(binding_kind(binding),BINDING_LOOP),eq(binding_kind(binding),BINDING_SWITCH))) {
      break;
    }
    binding = binding_next(binding);
  }
  return binding;
}

function cgc_lookup_goto_label(ident, env) {
  return cgc_lookup_binding_ident(BINDING_GOTO_LABEL, ident, env);
}

function cgc_lookup_struct(ident, env) {
  return cgc_lookup_binding_ident(BINDING_TYPE_STRUCT, ident, env);
}

function cgc_lookup_union(ident, env) {
  return cgc_lookup_binding_ident(BINDING_TYPE_UNION, ident, env);
}

function cgc_lookup_enum(ident, env) {
  return cgc_lookup_binding_ident(BINDING_TYPE_ENUM, ident, env);
}

function cgc_lookup_enum_value(ident, env) {
  return cgc_lookup_binding_ident(BINDING_ENUM_CST, ident, env);
}

function cgc_add_local(binding_type, ident, type, env) {
  var binding;
  binding = alloc_obj(5);
  heap[add(binding, 0)] = env;
  heap[add(binding, 1)] = binding_type;
  heap[add(binding, 2)] = ident;
  heap[add(binding, 3)] = cgc_fs;
  heap[add(binding, 4)] = type;
  return binding;
}

function cgc_add_local_param(ident, width, type) {
  cgc_locals = cgc_add_local(BINDING_PARAM_LOCAL, ident, type, cgc_locals);
  cgc_fs = sub(cgc_fs, width);
}

function cgc_add_local_var(ident, width, type) {
  cgc_fs = add(cgc_fs, width);
  cgc_locals = cgc_add_local(BINDING_VAR_LOCAL, ident, type, cgc_locals);
}

function cgc_add_enclosing_loop(loop_fs, break_lbl, continue_lbl) {
  var binding;
  binding = alloc_obj(5);
  heap[add(binding, 0)] = cgc_locals;
  heap[add(binding, 1)] = BINDING_LOOP;
  heap[add(binding, 2)] = loop_fs;
  heap[add(binding, 3)] = break_lbl;
  heap[add(binding, 4)] = continue_lbl;
  cgc_locals = binding;
}

function cgc_add_enclosing_switch(loop_fs, break_lbl, next_case_lbl) {
  var binding;
  binding = alloc_obj(6);
  heap[add(binding, 0)] = cgc_locals;
  heap[add(binding, 1)] = BINDING_SWITCH;
  heap[add(binding, 2)] = loop_fs;
  heap[add(binding, 3)] = break_lbl;
  heap[add(binding, 4)] = next_case_lbl;
  heap[add(binding, 5)] = 0; // Default label
  cgc_locals = binding;
}

function cgc_add_global(ident, width, type, is_static_local) {
  var binding;
  binding = alloc_obj(5);
  if(is_static_local) {
    heap[add(binding, 0)] = cgc_locals;
  } else {
    heap[add(binding, 0)] = cgc_globals;
  }
  heap[add(binding, 1)] = BINDING_VAR_GLOBAL;
  heap[add(binding, 2)] = ident;
  heap[add(binding, 3)] = cgc_global_alloc;
  heap[add(binding, 4)] = type;
  cgc_global_alloc = add(cgc_global_alloc, width);
  if (is_static_local) {
    cgc_locals = binding;
  } else {
    cgc_globals = binding;
  }
}

function cgc_add_global_fun(ident, label, type) {
  var binding;
  binding = alloc_obj(6);
  heap[add(binding, 0)] = cgc_globals;
  heap[add(binding, 1)] = BINDING_FUN;
  heap[add(binding, 2)] = ident;
  heap[add(binding, 3)] = 0;
  heap[add(binding, 4)] = label;
  heap[add(binding, 5)] = type;
  cgc_globals = binding;
}

function cgc_add_enum(ident, value) {
  var binding;
  binding = alloc_obj(4);
  heap[add(binding, 0)] = cgc_globals;
  heap[add(binding, 1)] = BINDING_ENUM_CST;
  heap[add(binding, 2)] = ident;
  heap[add(binding, 3)] = value;
  cgc_globals = binding;
}

function cgc_add_goto_label(ident, lbl) {
  var binding;
  binding = alloc_obj(5);
  heap[add(binding, 0)] = cgc_locals_fun;
  heap[add(binding, 1)] = BINDING_GOTO_LABEL;
  heap[add(binding, 2)] = ident;
  heap[add(binding, 3)] = lbl;
  cgc_locals_fun = binding;
}

function cgc_add_typedef(ident, struct_or_union_or_enum, type) {
  var binding;
  binding = alloc_obj(4);
  heap[add(binding, 0)] = cgc_globals;
  heap[add(binding, 1)] = struct_or_union_or_enum;
  heap[add(binding, 2)] = ident;
  heap[add(binding, 3)] = type;
  cgc_globals = binding;
}

function grow_fs(words) {
  cgc_fs = add(cgc_fs, words);
}

const var reg_X;
const var reg_Y;
const var reg_Z;
const var reg_SP;
const var reg_glo;

#define mov_mem_reg(base, offset, src) mov_mem32_reg(base, offset, src)
#define mov_reg_mem(dst, base, offset) mov_reg_mem32(dst, base, offset)

function load_mem_location(int dst, int base, int offset, int width, bool is_signed) {
  if (is_signed) {
    switch (width) {
      case 1: mov_reg_mem8_sign_ext(dst, base, offset);  break;
      case 2: mov_reg_mem16_sign_ext(dst, base, offset); break;
      case 4: mov_reg_mem32(dst, base, offset); break;
      default: fatal_error("load_mem_location: unknown width");
    }
  } else {
    switch (width) {
      case 1: mov_reg_mem8(dst, base, offset);  break;
      case 2: mov_reg_mem16(dst, base, offset); break;
      case 4: mov_reg_mem32(dst, base, offset); break;
      default: fatal_error("load_mem_location: unknown width");
    }
  }
}

// Write a value from a register to a memory location
function write_mem_location(int base, int offset, int src, int width) {
  if (width > WORD_SIZE) {
    fatal_error("write_mem_location: width > WORD_SIZE");
  }

  switch (width) {
    case 1: mov_mem8_reg(base, offset, src); break;
    case 2: mov_mem16_reg(base, offset, src); break;
    case 4: mov_mem32_reg(base, offset, src); break;
    default: fatal_error("write_mem_location: unknown width");
  }
}

function copy_obj(int dst_base, int dst_offset, int src_base, int src_offset, int width) {
  int i;
  // move the words
  for (i = 0; i < width / WORD_SIZE; i += 1) {
    mov_reg_mem(reg_Z, src_base, src_offset + i * WORD_SIZE);
    mov_mem_reg(dst_base, dst_offset + i * WORD_SIZE, reg_Z);
  }

  // then move the remaining bytes
  for (i = width - width % WORD_SIZE; i < width; i += 1) {
    mov_reg_mem8(reg_Z, src_base, src_offset + i);
    mov_mem8_reg(dst_base, dst_offset + i, reg_Z);
  }
}

// Initialize a memory location with a value
function initialize_memory(int val, int base, int offset, int width) {
  int i;
  mov_reg_imm(reg_Z, val);
  for (i = 0; i < width / WORD_SIZE; i += 1) {
    mov_mem_reg(base, offset + i * WORD_SIZE, reg_Z);
  }
  for (i = width - width % WORD_SIZE; i < width; i += 1) {
    mov_mem8_reg(base, offset + i, reg_Z);
  }
}

function is_power_of_2(int n) {
  return n != 0 && (n & (n - 1)) == 0;
}

function power_of_2_log(int n) {
  int i = 0;
  while (n > 1) {
    n /= 2;
    i += 1;
  }
  return i;
}

function mul_for_pointer_arith(int reg, int width) {
  int other_reg = reg == reg_X ? reg_Y : reg_X;

  if (width == 1) return;

  if (is_power_of_2(width)) {
    while (width > 1) {
      width /= 2;
      add_reg_reg(reg, reg);
    }
  } else {
    push_reg(other_reg);
    mov_reg_imm(other_reg, width);
    mul_reg_reg(reg, other_reg);
    pop_reg(other_reg);
  }
}

function div_for_pointer_arith(int reg, int width) {
  int reg_start = reg;

  if (width == 1) return;

  if (is_power_of_2(width)) {
    // sar_reg_reg does not work with reg_Y, so we need to shift the value to reg_X
    if (reg_start != reg_X) {
      push_reg(reg_X);                // Save reg_X
      mov_reg_reg(reg_X, reg_start);  // Move the value to reg_X
      reg = reg_X;
    } else {
      push_reg(reg_Y);                // Otherwise we still clobber reg_Y so save it
    }

    // At this point, reg is always reg_X, and reg_Y is free
    mov_reg_imm(reg_Y, power_of_2_log(width));
    sar_reg_reg(reg_X, reg_Y);

    // Now reg_X contains the result, and we move it back in reg_start if needed
    if (reg_start != reg_X) {
      mov_reg_reg(reg_start, reg_X);
      pop_reg(reg_X);
    } else {
      pop_reg(reg_Y); // Otherwise, we still need to restore reg_Y
    }
  } else {
    // div_reg_reg only works with reg_X on certain architectures, so we need to save it
    if (reg_start != reg_X) {
      push_reg(reg_X);
      reg = reg_X;
    } else {
      push_reg(reg_Y);
    }

    mov_reg_imm(reg_Y, width);
    div_reg_reg(reg_X, reg_Y);

    if (reg_start != reg_X) {
      mov_reg_reg(reg_start, reg_X);
      pop_reg(reg_X);
    } else {
      pop_reg(reg_Y);
    }
  }
}

const int EQ; // x == y
const int NE; // x != y
const int LT; // x < y
const int LT_U; // x < y  (unsigned)
const int GE; // x >= y
const int GE_U; // x >= y (unsigned)
const int LE; // x <= y
const int LE_U; // x <= y (unsigned)
const int GT; // x > y
const int GT_U; // x > y  (unsigned)

function jump_cond_reg_reg(int cond, int lbl, int reg1, int reg2);

void os_exit();
void os_allocate_memory(int size);
void os_read();
void os_write();
void os_open();
void os_close();
void os_seek();
void os_unlink();
void os_mkdir();
void os_chmod();
void os_access();

void rt_putchar();
void rt_debug(char* msg);
void rt_crash(char* msg);

void setup_proc_args(int global_vars_size);

#define cgc int

int setup_lbl;
int init_start_lbl;
int init_next_lbl;
int main_lbl;
int exit_lbl;
int getchar_lbl;
int putchar_lbl;
int fopen_lbl;
int fclose_lbl;
int fgetc_lbl;
int malloc_lbl;
int free_lbl;
int printf_lbl; // Stub

int read_lbl;
int write_lbl;
int open_lbl;
int close_lbl;
int seek_lbl;
int unlink_lbl;
int mkdir_lbl;
int chmod_lbl;
int access_lbl;

function word_size_align(int n) {
  return (n + WORD_SIZE - 1) / WORD_SIZE * WORD_SIZE;
}

function align_to(int mul, int n) {
  return (n + mul - 1) / mul * mul;
}

function grow_stack(int words) {
  add_reg_imm(reg_SP, -words * WORD_SIZE);
}

// Like grow_stack, but takes bytes instead of words.
// To maintain alignment, the stack is grown by a multiple of WORD_SIZE (rounded
// up from the number of bytes).
function grow_stack_bytes(int bytes) {
  add_reg_imm(reg_SP, -word_size_align(bytes));
}

void rt_debug(char* msg);
void rt_crash(char* msg);

// Label definition

enum {
  GENERIC_LABEL,
  GOTO_LABEL,
};

#define assert_all_labels_defined() // No-op
#define add_label(lbl) // No-op
#define alloc_label(name) alloc_label_()

function alloc_label_() {
  int lbl = alloc_obj(2);
  heap[lbl] = GENERIC_LABEL;
  heap[lbl + 1] = 0; // Address of label
  add_label(lbl);
  return lbl;
}

function alloc_goto_label() {
  int lbl = alloc_obj(3);
  heap[lbl] = GOTO_LABEL;
  heap[lbl + 1] = 0; // Address of label
  heap[lbl + 2] = 0; // cgc-fs of label
  add_label(lbl);
  return lbl;
}

function use_label(int lbl) {

  int addr = heap[lbl + 1];

  if (heap[lbl] != GENERIC_LABEL) fatal_error("use_label expects generic label");

  if (addr < 0) {
    // label address is currently known
    addr = -addr - (code_alloc + 4); // compute relative address
    emit_i32_le(addr);
  } else {
    // label address is not yet known
    emit_i32_le(0); // 32 bit placeholder for distance
    code[code_alloc-1] = addr; // chain with previous patch address
    heap[lbl + 1] = code_alloc;
  }
}

function def_label(int lbl) {

  int addr = heap[lbl + 1];
  int label_addr = code_alloc;
  int next;

  if (heap[lbl] != GENERIC_LABEL) fatal_error("def_label expects generic label");

  heap[lbl + 1] = -label_addr; // define label's address
  while (addr != 0) {
    next = code[addr-1]; // get pointer to next patch address
    code_alloc = addr;
    addr = label_addr - addr; // compute relative address
    code_alloc -= 4;
    emit_i32_le(addr);
    addr = next;
  }
  code_alloc = label_addr;
}

// Similar to use_label, but for gotos.
// The main difference is that it adjusts the stack and jumps, as opposed to
// simply emitting the address.
function jump_to_goto_label(int lbl) {

  int addr = heap[lbl + 1];
  int lbl_fs = heap[lbl + 2];
  int start_code_alloc = code_alloc;

  if (heap[lbl] != GOTO_LABEL) fatal_error("jump_to_goto_label expects goto label");

  if (addr < 0) {
    // label address is currently known
    grow_stack(lbl_fs - cgc_fs);
    start_code_alloc = code_alloc;
    jump_rel(0); // Generate dummy jump instruction to get instruction length
    addr = -addr - code_alloc; // compute relative address
    code_alloc = start_code_alloc;
    jump_rel(addr);
  } else {
    // label address is not yet known
    // placeholders for when we know the destination address and frame size
    grow_stack(0);
    jump_rel(0);
    code[code_alloc-1] = addr; // chain with previous patch address
    code[code_alloc-2] = cgc_fs; // save current frame size
    code[code_alloc-3] = start_code_alloc; // track initial code alloc so we can come back
    heap[lbl + 1] = code_alloc;
  }
}

function def_goto_label(int lbl) {

  int addr = heap[lbl + 1];
  int label_addr = code_alloc;
  int next;
  int goto_fs;
  int start_code_alloc;

  if (heap[lbl] != GOTO_LABEL) fatal_error("def_goto_label expects goto label");

  if (addr < 0) {
    fatal_error("goto label defined more than once");
  } else {
    heap[lbl + 1] = -label_addr; // define label's address
    heap[lbl + 2] = cgc_fs;      // define label's frame size
    while (addr != 0) {
      next = code[addr-1]; // get pointer to next patch address
      goto_fs = code[addr-2]; // get frame size at goto instruction
      code_alloc = code[addr-3]; // reset code pointer to start of jump_to_goto_label instruction
      grow_stack(cgc_fs - goto_fs); // adjust stack
      start_code_alloc = code_alloc;
      jump_rel(0); // Generate dummy jump instruction to get instruction length
      addr = label_addr - code_alloc; // compute relative address
      code_alloc = start_code_alloc;
      jump_rel(addr);
      addr = next;
    }
    code_alloc = label_addr;
  }
}

ast int_type;
ast uint_type;
ast char_type;
ast string_type;
ast void_type;
ast void_star_type;

function dereference_type(ast type) {
  switch (get_op(type)) {
    case '[': // Array type
      return get_child_('[', type, 0);
    case '*': // Pointer type
      return get_child_('*', type, 1);
    default:
      putstr("type="); putint(get_op(type)); putchar('\n');
      fatal_error("dereference_type: non pointer is being dereferenced with *");
      return -1;
  }
}

// Type, structure and union handling
int struct_union_size(ast struct_type);

// A pointer type is either an array type or a type with at least one star
function is_pointer_type(ast type) {
  bool op = get_op(type);
  return op == '[' || op == '*';
}

function is_function_type(ast type) {
  int op = get_op(type);
  if (op == '*') {
    if (get_op(get_child_('*', type, 1)) == '(') {
      return true;
    }
  }
  return op == '(';
}

bool is_struct_or_union_type(ast type) {
  int op = get_op(type);
  return op == STRUCT_KW || op == UNION_KW;
}

// An aggregate type is either an array type or a struct/union type (that's not a reference)
bool is_aggregate_type(ast type) {
  int op = get_op(type);
  return op == '[' || op == STRUCT_KW || op == UNION_KW;
}

bool is_not_pointer_type(ast type) {
  return !is_pointer_type(type);
}

bool is_numeric_type(ast type) {
  switch (get_op(type)) {
    case CHAR_KW:
    case INT_KW:
    case FLOAT_KW:
    case DOUBLE_KW:
    case SHORT_KW:
    case LONG_KW:
    case ENUM_KW: // Enums are considered numeric types
      return true;
    default: // Struct/union/pointer/array
      return false;
  }
}

bool is_signed_numeric_type(ast type) {
  switch (get_op(type)) {
    case CHAR_KW:
    case INT_KW:
    case FLOAT_KW:
    case DOUBLE_KW:
    case SHORT_KW:
    case LONG_KW:
      return !TEST_TYPE_SPECIFIER(get_val(type), UNSIGNED_KW);
    default:
      return true; // Not a numeric type => it's a struct/union/pointer/array and we consider it signed
  }
}

// Size an object of the given type would occupy in memory (in bytes).
// If array_value is true, the size of the array is returned, otherwise the
// size of the pointer is returned.
// If word_align is true, the size is rounded up to the word size.
int type_width(ast type, bool array_value, bool word_align) {
  int width = 1;
  // Basic type kw
  switch (get_op(type)) {
    case '[':
      // In certain contexts, we want to know the static size of the array (i.e.
      // sizeof, in struct definitions, etc.) while in other contexts we care
      // about the pointer (i.e. when passing an array to a function, etc.)
      if (array_value) {
        width = get_child_('[', type, 1) * type_width(get_child_('[', type, 0), true, false);
      } else {
        width = WORD_SIZE; // Array is a pointer to the first element
      }
      break;
    case '*':      width = WORD_SIZE; break;
    case VOID_KW:  width = 1;         break; // Default to 1 byte for void so pointer arithmetic and void casts work
    case CHAR_KW:  width = 1;         break;
    case SHORT_KW: width = 2;         break;
    case INT_KW:   width = 4;         break;
    case LONG_KW:
      width = 4;
      break;
    case STRUCT_KW:
    case UNION_KW:
      width = struct_union_size(type);
      break;
    default:       width = WORD_SIZE; break;
  }

  if (word_align) width = word_size_align(width);
  return width;
}

// Width of an object pointed to by a reference type.
ast ref_type_width(ast type) {
  return type_width(dereference_type(type), false, false);
}

// Structs, enums and unions types come in 2 variants:
//  - definition: the type contains the members of the struct/enum/union
//  - reference: the type reference an already declared struct/enum/union and doesn't contain the members.
//
// We mostly want to work with type definitions, and not type references so
// this function returns the type definition when passed a type reference.
ast canonicalize_type(ast type) {
  ast res = type;
  int binding;

  if (get_op(type) == STRUCT_KW && get_child_opt_(STRUCT_KW, LIST, type, 2) == 0) { // struct with empty def => reference
    binding = cgc_lookup_struct(get_val_(IDENTIFIER, get_child__(STRUCT_KW, IDENTIFIER, type, 1)), cgc_globals);
  } else if (get_op(type) == UNION_KW && get_child_opt_(UNION_KW, LIST, type, 2) == 0) { // union with empty def => reference
    binding = cgc_lookup_union(get_val_(IDENTIFIER, get_child__(UNION_KW, IDENTIFIER, type, 1)), cgc_globals);
  } else if (get_op(type) == ENUM_KW && get_child_opt_(ENUM_KW, LIST, type, 2) == 0) { // enum with empty def => reference
    binding = cgc_lookup_enum(get_val_(IDENTIFIER, get_child__(ENUM_KW, IDENTIFIER, type, 1)), cgc_globals);
  } else {
    return res;
  }

  if (binding == 0) {
    putstr("type="); putstr(STRING_BUF(get_val_(IDENTIFIER, get_child(type, 1)))); putchar('\n');
    fatal_error("canonicalize_type: type is not defined");
  }
  res = heap[binding+3];

  return res;
}

// Size of the largest member of a struct or union, used for alignment
int struct_union_size_largest_member = 0;

int type_largest_member(ast type) {
  switch (get_op(type)) {
    case STRUCT_KW:
    case UNION_KW:
      struct_union_size_largest_member = 0;
      struct_union_size(type); // Compute struct_union_size_largest_member global
      return struct_union_size_largest_member;
    case '[':
      return type_largest_member(get_child_('[', type, 0));
    default:
      return type_width(type, true, false);
  }
}

// Size of a struct or union type
int struct_union_size(ast type) {
  ast members;
  ast member_type;
  int member_size, largest_submember_size;
  int sum_size = 0, max_size = 0, largest_member_size = 0;

  type = canonicalize_type(type);
  members = get_child(type, 2);

  while (members != 0) {
    member_type = get_child_(DECL, car_(DECL, members), 1);
    members = tail(members);
    member_size = type_width(member_type, true, false);
    largest_submember_size = type_largest_member(member_type);
    if (member_size != 0) sum_size = align_to(largest_submember_size, sum_size); // Align the member to the word size
    sum_size += member_size;                                          // Struct size is the sum of its members
    if (member_size > max_size) max_size = member_size;               // Union size is the max of its members
    if (largest_member_size < largest_submember_size) largest_member_size = largest_submember_size;
  }

  sum_size = align_to(largest_member_size, sum_size); // The final struct size is a multiple of its widest member
  max_size = align_to(largest_member_size, max_size); // The final union size is a multiple of its widest member

  // Set the struct_union_size_largest_member global to "return" it
  struct_union_size_largest_member = largest_member_size;
  return get_op(type) == STRUCT_KW ? sum_size : max_size;
}

// Find offset of struct member
int struct_member_offset_go(ast struct_type, ast member_ident) {
  ast members = get_child(canonicalize_type(struct_type), 2);
  int offset = 0;
  int member_size, sub_offset;
  ast decl, ident;

  while (members != 0) {
    decl = car_(DECL, members);
    ident = get_child_opt_(DECL, IDENTIFIER, decl, 0);
    if (ident == 0) { // Anonymous struct member, search that struct
      sub_offset = struct_member_offset_go(get_child_(DECL, decl, 1), member_ident);
      if (sub_offset != -1) return offset + sub_offset;
    } else if (get_val_(IDENTIFIER, member_ident) == get_val_(IDENTIFIER, ident)) {
      return offset;
    }

    if (get_op(struct_type) == STRUCT_KW) {
      // For unions, fields are always at offset 0. We must still iterate
      // because the field may be in an anonymous struct, in which case the
      // final offset is not 0.
      member_size = type_width(get_child_(DECL, decl, 1), true, false);
      if (member_size != 0) offset = align_to(type_largest_member(get_child_(DECL, decl, 1)), offset);
      offset += member_size;
    }
    members = tail(members);
  }

  return -1;
}

int struct_member_offset(ast struct_type, ast member_ident) {
  int offset = struct_member_offset_go(struct_type, member_ident);
  if (offset == -1) fatal_error("struct_member_offset: member not found");
  return offset;
}

// Find a struct member
ast struct_member_go(ast struct_type, ast member_ident) {
  ast members = get_child(canonicalize_type(struct_type), 2);
  ast decl, ident;

  while (members != 0) {
    decl = car_(DECL, members);
    ident = get_child_opt_(DECL, IDENTIFIER, decl, 0);
    if (ident == 0) { // Anonymous struct member, search that struct
      ident = struct_member_go(get_child_(DECL, decl, 1), member_ident);
      if (ident != -1) return ident; // Found member in the anonymous struct
    } else if (get_val_(IDENTIFIER, member_ident) == get_val_(IDENTIFIER, ident)) {
      return decl;
    }
    members = tail(members);
  }

  return -1;
}

ast struct_member(ast struct_type, ast member_ident) {
  ast member = struct_member_go(struct_type, member_ident);
  if (member == -1) fatal_error("struct_member: member not found");
  return member;
}

int resolve_identifier(int ident_probe) {
  int binding = cgc_lookup_var(ident_probe, cgc_locals);
  if (binding != 0) return binding;

  binding = cgc_lookup_var(ident_probe, cgc_globals);
  if (binding != 0) return binding;

  binding = cgc_lookup_fun(ident_probe, cgc_globals);
  if (binding != 0) return binding;

  binding = cgc_lookup_enum_value(ident_probe, cgc_globals);
  if (binding != 0) return binding;

  putstr("ident = "); putstr(STRING_BUF(ident_probe)); putchar('\n');
  fatal_error("identifier not found");
  return 0;
}

// Compute the type of an expression
ast value_type(ast node) {
  int op = get_op(node);
  int nb_children = get_nb_children(node);
  int binding;
  ast left_type, right_type;
  ast child0, child1;

  if (nb_children >= 1) child0 = get_child(node, 0);
  if (nb_children >= 2) child1 = get_child(node, 1);

  if (nb_children == 0) {
    if (op == INTEGER || op == INTEGER_L || op == INTEGER_LL) {
      return int_type;
    } else if (op == INTEGER_U || op == INTEGER_UL || op == INTEGER_ULL) {
      return uint_type;
    }
    else if (op == CHARACTER) {
      return char_type;
    } else if (op == STRING) {
      return string_type;
    } else if (op == IDENTIFIER) {
      binding = resolve_identifier(get_val_(IDENTIFIER, node));
      switch (binding_kind(binding)) {
        case BINDING_PARAM_LOCAL:
        case BINDING_VAR_LOCAL:
          return heap[binding+4];
        case BINDING_VAR_GLOBAL:
          return heap[binding+4];
        case BINDING_ENUM_CST:
          return int_type;
        case BINDING_FUN:
          return heap[binding+5];
        default:
          putstr("ident = ");
          putstr(STRING_BUF(get_val_(IDENTIFIER, node)));
          putchar('\n');
          fatal_error("value_type: unknown identifier");
          return -1;
      }

    } else {
      putstr("op="); putint(op); putchar('\n');
      fatal_error("value_type: unknown expression with nb_children == 0");
      return -1;
    }

  } else if (nb_children == 1) {

    if (op == '*') {
      left_type = value_type(child0);
      if (is_function_type(left_type)) {
        return left_type;
      } else {
        return dereference_type(left_type);
      }
    } else if (op == '&') {
      left_type = value_type(child0);
      return pointer_type(left_type, false);
    } else if (op == '!') {
      return int_type; // Logical not always returns an integer
    } else if (op == '+' || op == '-' || op == '~' || op == MINUS_MINUS || op == PLUS_PLUS || op == MINUS_MINUS_POST || op == PLUS_PLUS_POST || op == PLUS_PLUS_PRE || op == MINUS_MINUS_PRE || op == PARENS) {
      // Unary operation don't change the type
      return value_type(child0);
    } else if (op == SIZEOF_KW) {
      return int_type; // sizeof always returns an integer
    } else {
      putstr("op="); putint(op); putchar('\n');
      fatal_error("value_type: unexpected operator");
      return -1;
    }

  } else if (nb_children == 2) {

    if (op == '+' || op == '-' || op == '*' || op == '/' || op == '%' || op == '&' || op == '|' || op == '^'
     || op == LSHIFT || op == RSHIFT) {
      left_type = value_type(child0);
      right_type = value_type(child1);
      if (is_pointer_type(left_type) && is_pointer_type(right_type) && op == '-') {
        return int_type; // Pointer - Pointer = Integer
      } else if (is_pointer_type(left_type)) {
        // if left is an array or a pointer, the type is also a pointer
        return left_type;
      } else {
        // if left is not a pointer, the type is the type of the right operand
        return right_type;
      }
    } else if (op == '<' || op == '>' || op == EQ_EQ || op == EXCL_EQ || op == LT_EQ || op == GT_EQ) {
      return int_type; // Comparison always returns an integer
    } else if (op == ',') {
      return value_type(child1); // The type of the right operand
    } else if (op == '[') {
      left_type = value_type(child0);
      right_type = value_type(child1);

      if (get_op(left_type) == '[' || get_op(left_type) == '*') {
        return dereference_type(left_type);
      } else if (get_op(right_type) == '[' || get_op(right_type) == '*') {
        return dereference_type(right_type);
      } else {
        putstr("left_type="); putint(get_op(left_type)); putchar('\n');
        putstr("right_type="); putint(get_op(right_type)); putchar('\n');
        fatal_error("value_type: non pointer is being dereferenced as array");
        return -1;
      }
    } else if (op == '=' || op == AMP_EQ || op == BAR_EQ || op == CARET_EQ || op == LSHIFT_EQ || op == MINUS_EQ || op == PERCENT_EQ || op == PLUS_EQ || op == RSHIFT_EQ || op == SLASH_EQ || op == STAR_EQ) {
      return value_type(child0); // Only the left side is relevant here
    } else if (op == AMP_AMP || op == BAR_BAR) {
      // TODO: Check that the operands have compatible types?
      return value_type(child0);
    } else if (op == '(') {
      binding = cgc_lookup_fun(get_val_(IDENTIFIER, child0), cgc_globals);
      if (binding != 0) {
        // heap[binding+5] is the '(' type
        return get_child_('(', heap[binding+5], 0);
      } else {
        putstr("ident = ");
        putstr(STRING_BUF(get_val_(IDENTIFIER, child0)));
        putchar('\n');
        fatal_error("value_type: function not found");
        return -1;
      }
    } else if (op == '.') {
      left_type = value_type(child0);
      if (is_struct_or_union_type(left_type)) {
        return get_child_(DECL, struct_member(left_type, child1), 1); // child 1 of member is the type
      } else {
        fatal_error("value_type: . operator on non-struct pointer type");
        return -1;
      }
    } else if (op == ARROW) {
      // Same as '.', but left_type must be a pointer
      left_type = value_type(child0);
      if (get_op(left_type) == '*' && is_struct_or_union_type(get_child_('*', left_type, 1))) {
        return get_child_(DECL, struct_member(get_child_('*', left_type, 1), child1), 1); // child 1 of member is the type
      } else {
        fatal_error("value_type: -> operator on non-struct pointer type");
        return -1;
      }
    } else if (op == CAST) {
      return get_child_(DECL, child0, 1);
    } else {
      fatal_error("value_type: unknown expression with 2 children");
      return -1;
    }

  } else if (nb_children == 3) {

    if (op == '?') {
      // We assume that the 2 cases have the same type.
      return value_type(child1);
    } else {
      putstr("op="); putint(op); putchar('\n');
      fatal_error("value_type: unknown expression with 3 children");
      return -1;
    }

  } else {
    putstr("op="); putint(op); putchar('\n');
    fatal_error("value_type: unknown expression with >4 children");
    return -1;
  }
}

void codegen_binop(int op, ast lhs, ast rhs) {

  int lbl1;
  int lbl2;
  int cond = -1;
  ast left_type = value_type(lhs);
  ast right_type = value_type(rhs);
  bool left_is_numeric = is_numeric_type(left_type);
  bool right_is_numeric = is_numeric_type(right_type);
  int width;
  // If any of the operands is unsigned, the result is unsigned
  bool is_signed = false;
  if (is_signed_numeric_type(left_type) && is_signed_numeric_type(right_type)) is_signed = true;

  pop_reg(reg_Y); // rhs operand
  pop_reg(reg_X); // lhs operand

  if      (op == '<')     cond = is_signed ? LT : LT_U;
  else if (op == '>')     cond = is_signed ? GT : GT_U;
  else if (op == LT_EQ)   cond = is_signed ? LE : LE_U;
  else if (op == GT_EQ)   cond = is_signed ? GE : GE_U;
  else if (op == EQ_EQ)   cond = EQ;
  else if (op == EXCL_EQ) cond = NE;

  if (cond != -1) {

    lbl1 = alloc_label(0);
    lbl2 = alloc_label(0);
    jump_cond_reg_reg(cond, lbl1, reg_X, reg_Y);
    xor_reg_reg(reg_X, reg_X);
    jump(lbl2);
    def_label(lbl1);
    mov_reg_imm(reg_X, 1);
    def_label(lbl2);

  } else if (op == '+' || op == PLUS_EQ || op == PLUS_PLUS_PRE || op == PLUS_PLUS_POST) {
    // Check if one of the operands is a pointer
    // If so, multiply the other operand by the width of the pointer target object.

    if (is_pointer_type(left_type) && is_not_pointer_type(right_type)) {
      mul_for_pointer_arith(reg_Y, ref_type_width(left_type));
    } else if (is_pointer_type(right_type) && is_not_pointer_type(left_type)) {
      mul_for_pointer_arith(reg_X, ref_type_width(right_type));
    }

    add_reg_reg(reg_X, reg_Y);
  }
  else if (op == '-' || op == MINUS_EQ || op == MINUS_MINUS_PRE || op == MINUS_MINUS_POST) {
    // Pointer subtraction is only valid if one of the operands is a pointer
    // When both operands are pointers, the result is the difference between the two pointers divided by the width of the target object.
    // When one operand is a pointer and the other is an integer, the result is the pointer minus the integer times the width of the target object.

    if (is_pointer_type(left_type) && is_pointer_type(right_type)) {
      sub_reg_reg(reg_X, reg_Y);
      div_for_pointer_arith(reg_X, ref_type_width(left_type));
    } else if (is_pointer_type(left_type)) {
      mul_for_pointer_arith(reg_Y, ref_type_width(left_type));
      sub_reg_reg(reg_X, reg_Y);
    } else if (is_pointer_type(right_type)) {
      mul_for_pointer_arith(reg_X, ref_type_width(right_type));
      sub_reg_reg(reg_X, reg_Y);
    } else {
      sub_reg_reg(reg_X, reg_Y);
    }
  }
  else if (op == '*' || op == STAR_EQ) {
    if (!left_is_numeric || !right_is_numeric) fatal_error("invalid operands to *");
    if (is_signed) imul_reg_reg(reg_X, reg_Y);
    else mul_reg_reg(reg_X, reg_Y);
  }
  else if (op == '/' || op == SLASH_EQ) {
    if (!left_is_numeric || !right_is_numeric) fatal_error("invalid operands to /");
    if (is_signed) idiv_reg_reg(reg_X, reg_Y);
    else div_reg_reg(reg_X, reg_Y);
  }
  else if (op == '%' || op == PERCENT_EQ) {
    if (!left_is_numeric || !right_is_numeric) fatal_error("invalid operands to %");
    if (is_signed) irem_reg_reg(reg_X, reg_Y);
    else rem_reg_reg(reg_X, reg_Y);
  }
  else if (op == RSHIFT || op == RSHIFT_EQ) {
    if (!left_is_numeric || !right_is_numeric) fatal_error("invalid operands to >>");
    if (is_signed) sar_reg_reg(reg_X, reg_Y);
    else shr_reg_reg(reg_X, reg_Y);
  }
  else if (op == LSHIFT || op == LSHIFT_EQ) {
    if (!left_is_numeric || !right_is_numeric) fatal_error("invalid operands to <<");
    s_l_reg_reg(reg_X, reg_Y); // Shift left, independent of sign
  }
  else if (op == '&' || op == AMP_EQ) {
    if (!left_is_numeric || !right_is_numeric) fatal_error("invalid operands to &");
    and_reg_reg(reg_X, reg_Y);
  }
  else if (op == '|' || op == BAR_EQ) {
    if (!left_is_numeric || !right_is_numeric) fatal_error("invalid operands to |");
    or_reg_reg(reg_X, reg_Y);
  }
  else if (op == '^' || op == CARET_EQ) {
    if (!left_is_numeric || !right_is_numeric) fatal_error("invalid operands to ^");
    xor_reg_reg(reg_X, reg_Y);
  }
  else if (op == ',')                       mov_reg_reg(reg_X, reg_Y); // Ignore lhs and keep rhs
  else if (op == '[') {
    // Same as pointer addition for address calculation
    if (is_pointer_type(left_type) && is_not_pointer_type(right_type)) {
      mul_for_pointer_arith(reg_Y, ref_type_width(left_type));
      width = ref_type_width(left_type);
      is_signed = is_signed_numeric_type(dereference_type(left_type));
    } else if (is_pointer_type(right_type) && is_not_pointer_type(left_type)) {
      mul_for_pointer_arith(reg_X, ref_type_width(right_type));
      width = ref_type_width(right_type);
      is_signed = is_signed_numeric_type(dereference_type(right_type));
    } else {
      fatal_error("codegen_binop: invalid array access operands");
      return;
    }

    add_reg_reg(reg_X, reg_Y);
    load_mem_location(reg_X, reg_X, 0, width, is_signed);
  } else {
    putstr("op="); putint(op); putchar('\n');
    fatal_error("codegen_binop: unknown op");
  }

  push_reg(reg_X);
}

void codegen_rvalue(ast node);
void codegen_statement(ast node);
int codegen_lvalue(ast node);

int codegen_param(ast param) {
  int type = value_type(param);
  int left_width;

  if (is_struct_or_union_type(type)) {
    left_width = codegen_lvalue(param);
    pop_reg(reg_X);
    grow_fs(-1);
    grow_stack_bytes(word_size_align(left_width));
    grow_fs(word_size_align(left_width) / WORD_SIZE);
    copy_obj(reg_SP, 0, reg_X, 0, left_width);
  } else {
    codegen_rvalue(param);
  }

  return type_width(type, false, true) / WORD_SIZE;
}

int codegen_params(ast params) {

  int fs = 0;

  if (params != 0) {
    fs = codegen_params(tail(params));
    fs += codegen_param(car(params));
  }

  return fs;
}

void codegen_call(ast node) {
  ast fun = get_child_('(', node, 0);
  ast params = get_child_('(', node, 1);
  ast nb_params;
  int binding = 0;

  // Check if the function is a direct call, find the binding if it is
  if (get_op(fun) == IDENTIFIER) {
    if (get_val_(IDENTIFIER, fun) == intern_str("PNUT_INLINE_INTERRUPT")) {
      debug_interrupt();
      push_reg(reg_X); // Dummy push to keep the stack balanced
      return;
    }
    binding = resolve_identifier(get_val_(IDENTIFIER, fun));
    if (binding_kind(binding) != BINDING_FUN) binding = 0;
  }

  nb_params = codegen_params(params);

  if (binding != 0) {
    // Generate a fast path for direct calls
    call(heap[binding+4]);
  } else {
    // Otherwise we go through the function pointer
    codegen_rvalue(fun);
    pop_reg(reg_X);
    grow_fs(-1);
    call_reg(reg_X);
  }

  grow_stack(-nb_params);
  grow_fs(-nb_params);

  push_reg(reg_X);
}

void codegen_goto(ast node) {
  ast label_ident = get_val_(IDENTIFIER, get_child__(GOTO_KW, IDENTIFIER, node, 0));

  int binding = cgc_lookup_goto_label(label_ident, cgc_locals_fun);
  int goto_lbl;

  if (binding == 0) {
    goto_lbl = alloc_goto_label();
    cgc_add_goto_label(label_ident, goto_lbl);
    binding = cgc_locals_fun;
  }

  jump_to_goto_label(heap[binding + 3]); // Label
}

// Return the width of the lvalue
int codegen_lvalue(ast node) {
  int op = get_op(node);
  int nb_children = get_nb_children(node);
  int binding;
  int lvalue_width = 0;
  ast type;
  ast child0, child1;

  if (nb_children >= 1) child0 = get_child(node, 0);
  if (nb_children >= 2) child1 = get_child(node, 1);

  if (nb_children == 0) {
    if (op == IDENTIFIER) {
      binding = resolve_identifier(get_val_(IDENTIFIER, node));
      switch (binding_kind(binding)) {
        case BINDING_PARAM_LOCAL:
        case BINDING_VAR_LOCAL:
          mov_reg_imm(reg_X, (cgc_fs - heap[binding+3]) * WORD_SIZE);
          add_reg_reg(reg_X, reg_SP);
          push_reg(reg_X);
          break;
        case BINDING_VAR_GLOBAL:
          mov_reg_imm(reg_X, heap[binding+3]);
          add_reg_reg(reg_X, reg_glo);
          push_reg(reg_X);
          break;
        case BINDING_FUN:
          mov_reg_lbl(reg_X, heap[binding+4]);
          push_reg(reg_X);
          break;
        default:
          fatal_error("codegen_lvalue: identifier not found");
          break;
      }
      lvalue_width = type_width(heap[binding+4], true, false);
    } else {
      putstr("op="); putint(op); putchar('\n');
      fatal_error("codegen_lvalue: unknown lvalue with nb_children == 0");
    }

  } else if (nb_children == 1) {

    if (op == '*') {
      codegen_rvalue(child0);
      grow_fs(-1);
      lvalue_width = ref_type_width(value_type(child0));
    } else if (op == PARENS) {
      lvalue_width = codegen_lvalue(child0);
      grow_fs(-1);
    } else {
      putstr("op="); putint(op); putchar('\n');
      fatal_error("codegen_lvalue: unexpected operator");
    }

  } else if (nb_children == 2) {

    if (op == '[') {
      type = value_type(child0);
      codegen_rvalue(child0);
      codegen_rvalue(child1);
      codegen_binop('+', child0, child1);
      grow_fs(-2);
      lvalue_width = ref_type_width(type);
    } else if (op == '.') {
      type = value_type(child0);
      if (is_struct_or_union_type(type)) {
        codegen_lvalue(child0);
        pop_reg(reg_X);
        // union members are at the same offset: 0
        if (get_op(type) == STRUCT_KW) {
          add_reg_imm(reg_X, struct_member_offset(type, child1));
        }
        push_reg(reg_X);
        grow_fs(-1);
        lvalue_width = type_width(get_child_(DECL, struct_member(type, child1), 1), true, false); // child 1 of member is the type
      } else {
        fatal_error("codegen_lvalue: . operator on non-struct type");
      }
    } else if (op == ARROW) {
      // Same as '.', but type must be a pointer
      type = value_type(child0);
      if (get_op(type) == '*' && is_struct_or_union_type(get_child_('*', type, 1))) {
        type = get_child_('*', type, 1);
        codegen_rvalue(child0);
        pop_reg(reg_X);
        // union members are at the same offset: 0
        if (get_op(type) == STRUCT_KW) {
          add_reg_imm(reg_X, struct_member_offset(type, child1));
        }
        push_reg(reg_X);
        grow_fs(-1);
        lvalue_width = type_width(get_child_(DECL, struct_member(type, child1), 1), true, false); // child 1 of member is the type
      } else {
        fatal_error("codegen_lvalue: -> operator on non-struct pointer type");
      }
    } else if (op == CAST) {
      codegen_lvalue(child1);
      lvalue_width = type_width(child0, true, false);
      grow_fs(-1); // grow_fs is called at the end of the function, so we need to decrement it here
    } else if (op == PARENS) {
      lvalue_width = codegen_lvalue(child0);
      grow_fs(-1);
    } else {
      fatal_error("codegen_lvalue: unknown lvalue with 2 children");
    }

  } else if (nb_children == 3) {

    if (op == '?') {

      int lbl1 = alloc_label(0); // false label
      int lbl2 = alloc_label(0); // end label
      codegen_rvalue(child0);
      pop_reg(reg_X);
      grow_fs(-1);
      xor_reg_reg(reg_Y, reg_Y);
      jump_cond_reg_reg(EQ, lbl1, reg_X, reg_Y);
      lvalue_width = codegen_lvalue(child1); // value when true, assume that lvalue_width is the same for both cases
      jump(lbl2);
      def_label(lbl1);
      grow_fs(-1); // here, the child#1 is not on the stack, so we adjust it
      codegen_lvalue(get_child_('?', node, 2)); // value when false
      grow_fs(-1); // grow_fs(1) is called by codegen_rvalue and at the end of the function
      def_label(lbl2);

    } else {
      putstr("op="); putint(op); putchar('\n');
      fatal_error("codegen_lvalue: unknown lvalue with 3 children");
    }

  } else {
    putstr("op="); putint(op); putchar('\n');
    fatal_error("codegen_lvalue: unknown lvalue with >2 children");
  }

  if (lvalue_width == 0) {
    fatal_error("codegen_lvalue: lvalue_width == 0");
  }

  grow_fs(1);
  return lvalue_width;
}

void codegen_string(int string_probe) {
  int lbl = alloc_label(0);
  char *string_start = STRING_BUF(string_probe);
  char *string_end = string_start + heap[string_probe + 4];

  call(lbl);

  while (string_start != string_end) {
    emit_i8(*string_start);
    string_start += 1;
  }

  emit_i8(0);

  def_label(lbl);
}

void codegen_rvalue(ast node) {
  int op = get_op(node);
  int nb_children = get_nb_children(node);
  int binding;
  int lbl1, lbl2;
  int left_width;
  ast type1, type2;
  ast child0, child1;

  if (nb_children >= 1) child0 = get_child(node, 0);
  if (nb_children >= 2) child1 = get_child(node, 1);

  if (nb_children == 0) {
    if ( op == INTEGER
      || op == INTEGER_L || op == INTEGER_LL || op == INTEGER_U || op == INTEGER_UL || op == INTEGER_ULL
       ) {
      mov_reg_imm(reg_X, -get_val(node));
      push_reg(reg_X);
    } else if (op == CHARACTER) {
      mov_reg_imm(reg_X, get_val_(CHARACTER, node));
      push_reg(reg_X);
    } else if (op == IDENTIFIER) {
      binding = resolve_identifier(get_val_(IDENTIFIER, node));
      switch (binding_kind(binding)) {
        case BINDING_PARAM_LOCAL:
          mov_reg_imm(reg_X, (cgc_fs - heap[binding+3]) * WORD_SIZE);
          add_reg_reg(reg_X, reg_SP);
          // structs/unions are allocated on the stack, so no need to dereference
          // For arrays, we need to dereference the pointer since they are passed as pointers
          if (get_op(heap[binding+4]) != STRUCT_KW && get_op(heap[binding+4]) != UNION_KW) {
            load_mem_location(reg_X, reg_X, 0, type_width(heap[binding+4], false, false), is_signed_numeric_type(heap[binding+4]));
          }
          push_reg(reg_X);
          break;

        case BINDING_VAR_LOCAL:
          mov_reg_imm(reg_X, (cgc_fs - heap[binding+3]) * WORD_SIZE);
          add_reg_reg(reg_X, reg_SP);
          // local arrays/structs/unions are allocated on the stack, so no need to dereference
          if (get_op(heap[binding+4]) != '[' && get_op(heap[binding+4]) != STRUCT_KW && get_op(heap[binding+4]) != UNION_KW) {
            load_mem_location(reg_X, reg_X, 0, type_width(heap[binding+4], false, false), is_signed_numeric_type(heap[binding+4]));
          }
          push_reg(reg_X);
          break;
        case BINDING_VAR_GLOBAL:
          mov_reg_imm(reg_X, heap[binding+3]);
          add_reg_reg(reg_X, reg_glo);
          // global arrays/structs/unions are also allocated on the stack, so no need to dereference
          if (get_op(heap[binding+4]) != '[' && get_op(heap[binding+4]) != STRUCT_KW && get_op(heap[binding+4]) != UNION_KW) {
            load_mem_location(reg_X, reg_X, 0, type_width(heap[binding+4], false, false), is_signed_numeric_type(heap[binding+4]));
          }
          push_reg(reg_X);
          break;
        case BINDING_ENUM_CST:
          mov_reg_imm(reg_X, -get_val_(INTEGER, heap[binding+3]));
          push_reg(reg_X);
          break;

        case BINDING_FUN:
          mov_reg_lbl(reg_X, heap[binding+4]);
          push_reg(reg_X);
          break;

        default:
          putstr("ident = "); putstr(STRING_BUF(get_val_(IDENTIFIER, node))); putchar('\n');
          fatal_error("codegen_rvalue: identifier not found");
          break;
      }
    } else if (op == STRING) {
      codegen_string(get_val_(STRING, node));
    } else {
      putstr("op="); putint(op); putchar('\n');
      fatal_error("codegen_rvalue: unknown rvalue with nb_children == 0");
    }

  } else if (nb_children == 1) {
    if (op == '*') {
      type1 = value_type(child0);
      codegen_rvalue(child0);
      grow_fs(-1);
      if (is_function_type(type1)) {
      } else if (is_pointer_type(type1)) {
        pop_reg(reg_X);
        load_mem_location(reg_X, reg_X, 0, ref_type_width(type1), is_signed_numeric_type(dereference_type(type1)));
        push_reg(reg_X);
      } else {
        fatal_error("codegen_rvalue: non-pointer is being dereferenced with *");
      }
    } else if (op == '+' || op == PARENS) {
      codegen_rvalue(child0);
      grow_fs(-1);
    } else if (op == '-') {
      codegen_rvalue(child0);
      pop_reg(reg_Y);
      grow_fs(-1);
      xor_reg_reg(reg_X, reg_X);
      sub_reg_reg(reg_X, reg_Y);
      push_reg(reg_X);
    } else if (op == '~') {
      codegen_rvalue(child0);
      pop_reg(reg_Y);
      grow_fs(-1);
      mov_reg_imm(reg_X, -1);
      xor_reg_reg(reg_X, reg_Y);
      push_reg(reg_X);
    } else if (op == '!') {
      xor_reg_reg(reg_X, reg_X);
      push_reg(reg_X);
      grow_fs(1);
      codegen_rvalue(child0);
      codegen_binop(EQ_EQ, new_ast0(INTEGER, 0), child0);
      grow_fs(-2);
    } else if (op == MINUS_MINUS_POST || op == PLUS_PLUS_POST) {
      left_width = codegen_lvalue(child0);
      pop_reg(reg_Y);
      load_mem_location(reg_X, reg_Y, 0, left_width, is_signed_numeric_type(value_type(child0)));
      push_reg(reg_X); // saves the original value of lvalue
      push_reg(reg_Y);
      push_reg(reg_X); // saves the value of lvalue to be modified
      mov_reg_imm(reg_X, 1); // Equivalent to calling codegen rvalue with INTEGER 1 (subtraction or addition handled in codegen_binop)
      push_reg(reg_X);
      codegen_binop(op, child0, new_ast0(INTEGER, 0)); // Pops two values off the stack and pushes the result
      pop_reg(reg_X); // result
      pop_reg(reg_Y); // address
      grow_fs(-1);
      write_mem_location(reg_Y, 0, reg_X, left_width); // Store the result in the address
    } else if (op == MINUS_MINUS_PRE || op == PLUS_PLUS_PRE) {
      left_width = codegen_lvalue(child0);
      pop_reg(reg_Y);
      push_reg(reg_Y);
      load_mem_location(reg_X, reg_Y, 0, left_width, is_signed_numeric_type(value_type(child0)));
      push_reg(reg_X);
      mov_reg_imm(reg_X, 1); // equivalent to calling codegen rvalue with INTEGER 1 (subtraction or addition handled in codegen_binop)
      push_reg(reg_X);
      codegen_binop(op, child0, new_ast0(INTEGER, 0)); // Pops two values off the stack and pushes the result
      pop_reg(reg_X); // result
      pop_reg(reg_Y); // address
      grow_fs(-1);
      write_mem_location(reg_Y, 0, reg_X, left_width); // store the result in the address
      push_reg(reg_X);
    } else if (op == '&') {
      codegen_lvalue(child0);
      grow_fs(-1);
    } else if (op == SIZEOF_KW) {
      if (get_op(child0) == DECL) {
        mov_reg_imm(reg_X, type_width(get_child_(DECL, child0, 1), true, false));
      } else {
        mov_reg_imm(reg_X, type_width(value_type(child0), true, false));
      }
      push_reg(reg_X);
    } else {
      putstr("op="); putint(op); putchar('\n');
      fatal_error("codegen_rvalue: unexpected operator");
    }

  } else if (nb_children == 2) {
    if (op == '+' || op == '-' || op == '*' || op == '/' || op == '%' || op == '&' || op == '|' || op == '^' || op == LSHIFT || op == RSHIFT || op == '<' || op == '>' || op == EQ_EQ || op == EXCL_EQ || op == LT_EQ || op == GT_EQ || op == '[' || op == ',') {
      codegen_rvalue(child0);
      codegen_rvalue(child1);
      codegen_binop(op, child0, child1);
      grow_fs(-2);
    } else if (op == '=') {
      type1 = value_type(child0);
      left_width = codegen_lvalue(child0);
      if (is_struct_or_union_type(type1)) {
        // Struct assignment, we copy the struct.
        codegen_lvalue(child1);
        pop_reg(reg_X);
        pop_reg(reg_Y);
        grow_fs(-2);
        copy_obj(reg_Y, 0, reg_X, 0, left_width);
      } else {
        codegen_rvalue(child1);
        pop_reg(reg_X);
        pop_reg(reg_Y);
        grow_fs(-2);
        write_mem_location(reg_Y, 0, reg_X, left_width);
      }
      push_reg(reg_X);
    } else if (op == AMP_EQ || op == BAR_EQ || op == CARET_EQ || op == LSHIFT_EQ || op == MINUS_EQ || op == PERCENT_EQ || op == PLUS_EQ || op == RSHIFT_EQ || op == SLASH_EQ || op == STAR_EQ) {
      left_width = codegen_lvalue(child0);
      pop_reg(reg_Y);
      push_reg(reg_Y);
      load_mem_location(reg_X, reg_Y, 0, left_width, is_signed_numeric_type(value_type(child0)));
      push_reg(reg_X);
      grow_fs(1);
      codegen_rvalue(child1);
      codegen_binop(op, child0, child1);
      pop_reg(reg_X);
      pop_reg(reg_Y);
      grow_fs(-3);
      write_mem_location(reg_Y, 0, reg_X, left_width);
      push_reg(reg_X);
    } else if (op == AMP_AMP || op == BAR_BAR) {
      lbl1 = alloc_label(0);
      codegen_rvalue(child0);
      pop_reg(reg_X);
      push_reg(reg_X);
      xor_reg_reg(reg_Y, reg_Y);
      jump_cond_reg_reg(op == AMP_AMP ? EQ : NE, lbl1, reg_X, reg_Y);
      pop_reg(reg_X); grow_fs(-1);
      codegen_rvalue(child1);
      grow_fs(-1);
      def_label(lbl1);
    } else if (op == '(') {
      codegen_call(node);
    } else if (op == '.') {
      type1 = value_type(child0);
      if (is_struct_or_union_type(type1)) {
        type2 = get_child_(DECL, struct_member(type1, child1), 1);
        codegen_lvalue(child0);
        pop_reg(reg_Y);
        grow_fs(-1);
        // union members are at the same offset: 0
        if (get_op(type1) == STRUCT_KW) {
          add_reg_imm(reg_Y, struct_member_offset(type1, child1));
        }
        if (!is_aggregate_type(type2)) {
          load_mem_location(reg_Y, reg_Y, 0, type_width(type2, false, false), is_signed_numeric_type(type2));
        }
        push_reg(reg_Y);
      } else {
        fatal_error("codegen_rvalue: . operator on non-struct type");
      }
    } else if (op == ARROW) {
      type1 = value_type(child0);
      if (get_op(type1) == '*' && is_struct_or_union_type(get_child_('*', type1, 1))) {
        type1 = get_child_('*', type1, 1);
        type2 = get_child_(DECL, struct_member(type1, child1), 1);
        codegen_rvalue(child0);
        pop_reg(reg_Y);
        grow_fs(-1);
        // union members are at the same offset: 0
        if (get_op(type1) == STRUCT_KW) {
          add_reg_imm(reg_Y, struct_member_offset(type1, child1));
        }
        if (!is_aggregate_type(type2)) {
          load_mem_location(reg_Y, reg_Y, 0, type_width(type2, false, false), is_signed_numeric_type(type2));
        }
        push_reg(reg_Y);
      } else {
        fatal_error("codegen_rvalue: -> operator on non-struct pointer type");
      }
    } else if (op == CAST) {
      codegen_rvalue(child1);
      // If the cast is to a value narrower than the width of the value, we need
      // to truncate the value. This is done by writing the value to the stack
      // and then reading it back, sign extending it if necessary.
      child0 = get_child_(DECL, child0, 1); // child 1 of cast is the type
      if (type_width(child0, false, false) < type_width(value_type(child1), false, false)) {
        load_mem_location(reg_X, reg_SP, 0, type_width(child0, false, false), is_signed_numeric_type(child0));
        grow_stack(-1);
        push_reg(reg_X);
      }
      grow_fs(-1); // grow_fs(1) is called by codegen_rvalue and at the end of the function
    } else {
      fatal_error("codegen_rvalue: unknown rvalue with 2 children");
    }

  } else if (nb_children == 3) {

    if (op == '?') {
      lbl1 = alloc_label(0); // false label
      lbl2 = alloc_label(0); // end label
      codegen_rvalue(child0);
      pop_reg(reg_X);
      grow_fs(-1);
      xor_reg_reg(reg_Y, reg_Y);
      jump_cond_reg_reg(EQ, lbl1, reg_X, reg_Y);
      codegen_rvalue(child1); // value when true
      jump(lbl2);
      def_label(lbl1);
      grow_fs(-1); // here, the child#1 is not on the stack, so we adjust it
      codegen_rvalue(get_child(node, 2)); // value when false
      grow_fs(-1); // grow_fs(1) is called by codegen_rvalue and at the end of the function
      def_label(lbl2);
    } else {
      putstr("op="); putint(op); putchar('\n');
      fatal_error("codegen_rvalue: unknown rvalue with 3 children");
    }

  } else {
    putstr("op="); putint(op); putchar('\n');
    fatal_error("codegen_rvalue: unknown rvalue with >4 children");
  }

  grow_fs(1);
}

void codegen_begin() {

  setup_lbl = alloc_label("setup");
  init_start_lbl = alloc_label("init_start");
  init_next_lbl = init_start_lbl;

  // Make room for heap start and malloc bump pointer.
  // reg_glo[0]: heap start
  // reg_glo[WORD_SIZE]: malloc bump pointer
  cgc_global_alloc += 2 * WORD_SIZE;

  int_type = new_ast0(INT_KW, 0);
  uint_type = new_ast0(INT_KW, MK_TYPE_SPECIFIER(UNSIGNED_KW));
  char_type = new_ast0(CHAR_KW, 0);
  string_type = pointer_type(new_ast0(CHAR_KW, 0), false);
  void_type = new_ast0(VOID_KW, 0);
  void_star_type = pointer_type(new_ast0(VOID_KW, 0), false);

  main_lbl = alloc_label("main");
  cgc_add_global_fun(init_ident(IDENTIFIER, "main"), main_lbl, function_type(void_type, 0));

  exit_lbl = alloc_label("exit");
  cgc_add_global_fun(init_ident(IDENTIFIER, "exit"), exit_lbl, function_type1(void_type, int_type));

  read_lbl = alloc_label("read");
  cgc_add_global_fun(init_ident(IDENTIFIER, "read"), read_lbl, function_type3(int_type, int_type, void_star_type, int_type));

  write_lbl = alloc_label("write");
  cgc_add_global_fun(init_ident(IDENTIFIER, "write"), write_lbl, function_type3(int_type, int_type, void_star_type, int_type));

  open_lbl = alloc_label("open");
  cgc_add_global_fun(init_ident(IDENTIFIER, "open"), open_lbl, make_variadic_func(function_type2(int_type, string_type, int_type)));

  close_lbl = alloc_label("close");
  cgc_add_global_fun(init_ident(IDENTIFIER, "close"), close_lbl, function_type1(int_type, int_type));

  seek_lbl = alloc_label("lseek");
  cgc_add_global_fun(init_ident(IDENTIFIER, "lseek"), seek_lbl, function_type3(int_type, int_type, int_type, int_type));

  unlink_lbl = alloc_label("unlink");
  cgc_add_global_fun(init_ident(IDENTIFIER, "unlink"), unlink_lbl, function_type1(int_type, string_type));

  mkdir_lbl = alloc_label("mkdir");
  cgc_add_global_fun(init_ident(IDENTIFIER, "mkdir"), mkdir_lbl, function_type2(int_type, string_type, int_type));

  chmod_lbl = alloc_label("chmod");
  cgc_add_global_fun(init_ident(IDENTIFIER, "chmod"), chmod_lbl, function_type2(int_type, string_type, int_type));

  access_lbl = alloc_label("access");
  cgc_add_global_fun(init_ident(IDENTIFIER, "access"), access_lbl, function_type2(int_type, string_type, int_type));

  putchar_lbl = alloc_label("putchar");
  cgc_add_global_fun(init_ident(IDENTIFIER, "putchar"), putchar_lbl, function_type1(void_type, char_type));

  getchar_lbl = alloc_label("getchar");
  cgc_add_global_fun(init_ident(IDENTIFIER, "getchar"), getchar_lbl, function_type(char_type, 0));

  fopen_lbl = alloc_label("fopen");
  cgc_add_global_fun(init_ident(IDENTIFIER, "fopen"), fopen_lbl, function_type2(int_type, string_type, string_type));

  fclose_lbl = alloc_label("fclose");
  cgc_add_global_fun(init_ident(IDENTIFIER, "fclose"), fclose_lbl, function_type1(int_type, int_type));

  fgetc_lbl = alloc_label("fgetc");
  cgc_add_global_fun(init_ident(IDENTIFIER, "fgetc"), fgetc_lbl, function_type1(int_type, int_type));

  malloc_lbl = alloc_label("malloc");
  cgc_add_global_fun(init_ident(IDENTIFIER, "malloc"), malloc_lbl, function_type1(void_star_type, int_type));

  free_lbl = alloc_label("free");
  cgc_add_global_fun(init_ident(IDENTIFIER, "free"), free_lbl, function_type1(void_type, void_star_type));

  printf_lbl = alloc_label("printf");
  cgc_add_global_fun(init_ident(IDENTIFIER, "printf"), printf_lbl, make_variadic_func(function_type1(int_type, string_type)));

  jump(setup_lbl);
}

void handle_enum_struct_union_type_decl(ast type);

void codegen_enum(ast node) {
  ast name = get_child_opt_(ENUM_KW, IDENTIFIER, node, 1);
  ast cases = get_child_opt_(ENUM_KW, LIST, node, 2);
  ast cas;
  int binding;

  if (name != 0 && cases != 0) { // if enum has a name and members (not a reference to an existing type)
    binding = cgc_lookup_enum(get_val_(IDENTIFIER, name), cgc_globals);
    if (binding != 0) { fatal_error("codegen_enum: enum already declared"); }
    cgc_add_typedef(get_val_(IDENTIFIER, name), BINDING_TYPE_ENUM, node);
  }

  while (cases != 0) {
    cas = car_('=', cases);
    cgc_add_enum(get_val_(IDENTIFIER, get_child__('=', IDENTIFIER, cas, 0)), get_child_('=', cas, 1));
    cases = tail(cases);
  }
}

void codegen_struct_or_union(ast node, enum BINDING kind) {
  ast name = get_child(node, 1);
  ast members = get_child(node, 2);
  int binding;

  // if struct has a name and members (not a reference to an existing type)
  if (name != 0 && members != 0) {
    binding = cgc_lookup_binding_ident(kind, get_val_(IDENTIFIER, name), cgc_globals);
    if (binding != 0 && heap[binding + 3] != node && get_child(heap[binding + 3], 2) != members) {
      fatal_error("codegen_struct_or_union: struct/union already declared");
    }
    cgc_add_typedef(get_val_(IDENTIFIER, name), kind, node);
  }

  // Traverse the structure to find any other declarations.
  // This is not the right semantic because inner declarations are scoped to
  // this declaration, but it's probably good enough for TCC.
  while (members != 0) {
    handle_enum_struct_union_type_decl(get_child_(DECL, car_(DECL, members), 1));
    members = tail(members);
  }
}

void handle_enum_struct_union_type_decl(ast type) {
  if (get_op(type) == ENUM_KW) {
    codegen_enum(type);
  } else if (get_op(type) == STRUCT_KW) {
    codegen_struct_or_union(type, BINDING_TYPE_STRUCT);
  } else if (get_op(type) == UNION_KW) {
    codegen_struct_or_union(type, BINDING_TYPE_UNION);
  } else if (get_op(type) == '*') {
    handle_enum_struct_union_type_decl(get_child_('*', type, 1));
  } else if (get_op(type) == '[') {
    handle_enum_struct_union_type_decl(get_child_('[', type, 0));
  }

  // If not an enum, struct, or union, do nothing
}

void codegen_initializer_string(int string_probe, ast type, int base_reg, int offset) {
  char *string_start = STRING_BUF(string_probe);
  int i = 0;
  int str_len = STRING_LEN(string_probe);
  int arr_len;

  // Only acceptable types are char[] or char*
  if (get_op(type) == '[' && get_op(get_child_('[', type, 0)) == CHAR_KW) {
    arr_len = get_child_('[', type, 1);
    if (str_len > arr_len) fatal_error("codegen_initializer: string initializer is too long for char[]");

    // Place the bytes of the string in the memory location allocated for the array
    for (; i < arr_len; i += 1) {
      mov_reg_imm(reg_X, i < str_len ? string_start[i] : 0);
      write_mem_location(base_reg, offset + i, reg_X, 1);
    }
  } else if (get_op(type) == '*' && get_op(get_child_('*', type, 1)) == CHAR_KW) {
    // Create the string and assign global variable to the pointer
    codegen_string(string_probe);
    pop_reg(reg_X);
    mov_mem_reg(base_reg, offset, reg_X);
  } else {
    fatal_error("codegen_initializer: string initializer must be assigned to a char[] or char*");
  }
}

// Initialize a variable with an initializer
void codegen_initializer(bool local, ast init, ast type, int base_reg, int offset) {
  ast members;
  ast inner_type;
  int arr_len;
  int inner_type_width;

  type = canonicalize_type(type);

  switch (get_op(init)) {
    case STRING:
      codegen_initializer_string(get_val_(STRING, init), type, base_reg, offset);
      break;

    case INITIALIZER_LIST:
      init = get_child_(INITIALIZER_LIST, init, 0);
      // Acceptable types are:
      //  arrays
      //  structs
      //  union   (if the initializer list has only one element)
      //  scalars (if the initializer list has only one element)
      switch (get_op(type)) {
        case '[':
          inner_type = get_child_('[', type, 0);
          arr_len = get_child_('[', type, 1);
          inner_type_width = type_width(get_child_('[', type, 0), true, false);

          while (init != 0 && arr_len != 0) {
            codegen_initializer(local, car(init), inner_type, base_reg, offset);
            offset += inner_type_width;
            init = tail(init);
            arr_len -= 1; // decrement the number of elements left to initialize to make sure we don't overflow
          }

          if (init != 0) {
            fatal_error("codegen_initializer: too many elements in initializer list");
          }

          // If there are still elements to initialize, set them to 0.
          // If it's not a local variable, we don't need to initialize the
          // memory since the stack is zeroed during setup.
          if (local && arr_len > 0) initialize_memory(0, base_reg, offset, inner_type_width * arr_len);
          break;

        case STRUCT_KW:
          members = get_child_(STRUCT_KW, type, 2);
          while (init != 0 && members != 0) {
            inner_type = get_child_(DECL, car_(DECL, members), 1);
            codegen_initializer(local, car(init), inner_type, base_reg, offset);
            offset += type_width(inner_type, true, false);
            init = tail(init);
            members = tail(members);
          }

          // Initialize rest of the members to 0
          while (local && members != 0) {
            inner_type = get_child_(DECL, car_(DECL, members), 1);
            initialize_memory(0, base_reg, offset, type_width(inner_type, true, false));
            offset += type_width(inner_type, true, false);
            members = tail(members);
          }
          break;

        case UNION_KW:
          members = get_child_(STRUCT_KW, type, 2);
          if (tail(init) != 0) {
            fatal_error("codegen_initializer: union initializer list has more than one element");
          } else if (members == 0) {
            fatal_error("codegen_initializer: union has no members");
          }
          codegen_initializer(local, car(init), get_child_(DECL, car_(DECL, members), 1), base_reg, offset);
          break;

        default:
          if (tail(init) != 0 // More than 1 element
           || get_op(car(init)) == INITIALIZER_LIST) { // Or nested initializer list
            fatal_error("codegen_initializer: scalar initializer list has more than one element");
          }
          codegen_rvalue(car(init));
          pop_reg(reg_X);
          grow_fs(-1);
          write_mem_location(base_reg, offset, reg_X, type_width(type, true, false));
          break;
      }

      break;

    default:
      if (is_struct_or_union_type(type)) {
        // Struct assignment, we copy the struct.
        codegen_lvalue(init);
        pop_reg(reg_X);
        grow_fs(-1);
        copy_obj(base_reg, offset, reg_X, 0, type_width(type, true, true));
      } else if (get_op(type) != '[') {
        codegen_rvalue(init);
        pop_reg(reg_X);
        grow_fs(-1);
        write_mem_location(base_reg, offset, reg_X, type_width(type, true, false));
      } else {
        fatal_error("codegen_initializer: cannot initialize array with scalar value");
      }
      break;
  }
}

// Return size of initializer.
// If it's an initializer list, return the number of elements
// If it's a string, return the length of the string and delimiter.
int initializer_size(ast initializer) {
  int size = 0;

  switch (get_op(initializer)) {
    case INITIALIZER_LIST:
      initializer = get_child_(INITIALIZER_LIST, initializer, 0);
      while (initializer != 0) {
        size += 1;
        initializer = tail(initializer);
      }
      return size;

    case STRING:
      return heap[get_val_(STRING, initializer) + 4] + 1;

    default:
      fatal_error("initializer_size: unknown initializer");
      return -1;
  }
}

void infer_array_length(ast type, ast init) {
  // Array declaration with no size
  if (get_op(type) == '[' && get_child_('[', type, 1) == 0) {
    if (init == 0) {
      fatal_error("Array declaration with no size must have an initializer");
    }
    set_child(type, 1, initializer_size(init));
  }
}

void codegen_glo_var_decl(ast node) {
  ast name = get_child__(DECL, IDENTIFIER, node, 0);
  ast type = get_child_(DECL, node, 1);
  ast init = get_child_(DECL, node, 2);
  int name_probe = get_val_(IDENTIFIER, name);
  int binding = cgc_lookup_var(name_probe, cgc_globals);

  if (get_op(type) == '(') {
    // Forward declaration
    binding = cgc_lookup_fun(name_probe, cgc_globals);
    if (binding == 0) cgc_add_global_fun(name_probe, alloc_label(STRING_BUF(name_probe)), type);

  } else {
    handle_enum_struct_union_type_decl(type);
    infer_array_length(type, init);

    if (binding == 0) {
      cgc_add_global(name_probe, type_width(type, true, true), type, false);
      binding = cgc_globals;
    }

    if (init != 0) {
      def_label(init_next_lbl);
      init_next_lbl = alloc_label("init_next");
      codegen_initializer(false, init, type, reg_glo, heap[binding + 3]); // heap[binding + 3] = offset
      jump(init_next_lbl);
    }
  }
}

// Compute the size of a local variable declaration in bytes
int compute_local_var_decl_size(ast type, ast init) {
  infer_array_length(type, init);

  if (is_aggregate_type(type)) { // Array/struct/union declaration
    return type_width(type, true, true);  // size in bytes (word aligned)
  } else {
    return WORD_SIZE;
  }
}

void codegen_local_var_decl(ast node) {
  ast name = get_child__(DECL, IDENTIFIER, node, 0);
  ast type = get_child_(DECL, node, 1);
  ast init = get_child_(DECL, node, 2);
  // For local variables, the smallest unit of memory is a word, so the size is in words
  int size = compute_local_var_decl_size(type, init) / WORD_SIZE;

  cgc_add_local_var(get_val_(IDENTIFIER, name), size, type);
  grow_stack(size); // Make room for the local variable

  if (init != 0) {
    // offset (cgc_fs - heap[cgc_locals + 3]) should be 0 since we just allocated the space
    codegen_initializer(true, init, type, reg_SP, 0);
  }
}

void codegen_static_local_var_decl(ast node) {
  ast name = get_child__(DECL, IDENTIFIER, node, 0);
  ast type = get_child_(DECL, node, 1);
  ast init = get_child_(DECL, node, 2);
  int size = compute_local_var_decl_size(type, init);
  int skip_init_lbl;

  cgc_add_global(get_val_(IDENTIFIER, name), size, type, true);

  if (init != 0) {
    // Skip over the initialization code that will run during program initialization
    skip_init_lbl = alloc_label("skip_init");
    jump(skip_init_lbl);
    def_label(init_next_lbl);
    init_next_lbl = alloc_label("init_next");
    codegen_initializer(false, init, type, reg_glo, heap[cgc_locals + 3]); // heap[cgc_locals + 3] = offset
    jump(init_next_lbl);
    def_label(skip_init_lbl);
  }
}

void codegen_local_var_decls(ast node) {
  bool is_static = false;

  switch (get_child_(DECLS, node, 1)) {
    // AUTO_KW and REGISTER_KW can simply be ignored.
    case STATIC_KW:
      is_static = true;
      break;
    case EXTERN_KW:
      fatal_error("Extern class specifier not supported");
      break;
  }

  node = get_child__(DECLS, LIST, node, 0);
  while (node != 0) { // Multiple variable declarations
    if (is_static) {
      codegen_static_local_var_decl(car_(DECL, node));
    } else {
      codegen_local_var_decl(car_(DECL, node));
    }
    node = tail(node);
  }
}

void codegen_body(ast node) {
  int save_fs = cgc_fs;
  int save_locals = cgc_locals;
  ast stmt;

  while (node != 0) {
    stmt = get_child_('{', node, 0);
    if (get_op(stmt) == DECLS) { // Variable declaration
      codegen_local_var_decls(stmt);
    } else {
      codegen_statement(stmt);
    }
    node = get_child_opt_('{', '{', node, 1);
  }

  grow_stack(save_fs - cgc_fs);

  cgc_fs = save_fs;
  cgc_locals = save_locals;
}

void codegen_statement(ast node) {
  int op;
  int lbl1, lbl2, lbl3;
  int save_fs;
  int save_locals;
  int binding;

  if (node == 0) return;

  op = get_op(node);

  if (op == IF_KW) {

    lbl1 = alloc_label(0); // else statement
    lbl2 = alloc_label(0); // join point after if
    codegen_rvalue(get_child_(IF_KW, node, 0));
    pop_reg(reg_X);
    grow_fs(-1);
    xor_reg_reg(reg_Y, reg_Y);
    jump_cond_reg_reg(EQ, lbl1, reg_X, reg_Y);
    codegen_statement(get_child_(IF_KW, node, 1));
    jump(lbl2);
    def_label(lbl1);
    codegen_statement(get_child_(IF_KW, node, 2));
    def_label(lbl2);

  } else if (op == WHILE_KW) {

    lbl1 = alloc_label(0); // while statement start
    lbl2 = alloc_label(0); // join point after while

    save_fs = cgc_fs;
    save_locals = cgc_locals;

    cgc_add_enclosing_loop(cgc_fs, lbl2, lbl1);

    def_label(lbl1);
    codegen_rvalue(get_child_(WHILE_KW, node, 0));
    pop_reg(reg_X);
    grow_fs(-1);
    xor_reg_reg(reg_Y, reg_Y);
    jump_cond_reg_reg(EQ, lbl2, reg_X, reg_Y);
    codegen_statement(get_child_(WHILE_KW, node, 1));
    jump(lbl1);
    def_label(lbl2);

    cgc_fs = save_fs;
    cgc_locals = save_locals;

  } else if (op == FOR_KW) {

    lbl1 = alloc_label(0); // while statement start
    lbl2 = alloc_label(0); // join point after while
    lbl3 = alloc_label(0); // initial loop starting point

    save_fs = cgc_fs;
    save_locals = cgc_locals;

    cgc_add_enclosing_loop(cgc_fs, lbl2, lbl1);

    codegen_statement(get_child_(FOR_KW, node, 0)); // init
    jump(lbl3); // skip post loop action
    def_label(lbl1);
    codegen_statement(get_child_(FOR_KW, node, 2)); // post loop action
    def_label(lbl3);
    if (get_child_(FOR_KW, node, 1) != 0) {
      codegen_rvalue(get_child_(FOR_KW, node, 1)); // test
      pop_reg(reg_X);
      grow_fs(-1);
      xor_reg_reg(reg_Y, reg_Y);
      jump_cond_reg_reg(EQ, lbl2, reg_X, reg_Y);
    }
    // if no test, we always fall down to the body

    codegen_statement(get_child_(FOR_KW, node, 3));
    jump(lbl1);
    def_label(lbl2);

    cgc_fs = save_fs;
    cgc_locals = save_locals;

  } else if (op == DO_KW) {

    lbl1 = alloc_label(0); // do statement start
    lbl2 = alloc_label(0); // break point

    save_fs = cgc_fs;
    save_locals = cgc_locals;

    cgc_add_enclosing_loop(cgc_fs, lbl2, lbl1);
    def_label(lbl1);
    codegen_statement(get_child_(DO_KW, node, 0));
    codegen_rvalue(get_child_(DO_KW, node, 1));
    pop_reg(reg_X);
    grow_fs(-1);
    xor_reg_reg(reg_Y, reg_Y);
    jump_cond_reg_reg(NE, lbl1, reg_X, reg_Y);

    def_label(lbl2);

    cgc_fs = save_fs;
    cgc_locals = save_locals;

  } else if (op == SWITCH_KW) {

    save_fs = cgc_fs;
    save_locals = cgc_locals;

    lbl1 = alloc_label(0); // lbl1: end of switch
    lbl2 = alloc_label(0); // lbl2: next case

    cgc_add_enclosing_switch(cgc_fs, lbl1, lbl2);
    binding = cgc_locals;

    codegen_rvalue(get_child_(SWITCH_KW, node, 0));    // switch operand
    jump(lbl2);                                        // Jump to first case
    codegen_statement(get_child_(SWITCH_KW, node, 1)); // switch body

    // The switch can fall through in 2 distinct ways:
    //  1. The conditional block had no break statement
    //  2. No cases (excluding the default) matched
    //
    // In both cases, the switch operand needs to be removed from the stack.
    // But in the second case, we first need to jump to the default label if it
    // exists.
    //
    // The code is laid out as follows:
    //  [eval switch opnd]
    //  [cases]
    //  ...
    //   <- Control is here
    //  [jump to adjust stack]
    //  [jump to default if it exists]
    //  [adjust stack]
    //  [end of switch]

    // In case #1 control ends up here
    lbl3 = alloc_label(0);
    jump(lbl3);

    // In case #2 control ends up here
    lbl2 = heap[binding + 4]; // Reload because the label is overwritten by CASE statements
    def_label(lbl2);
    // If the default statement is present, we jump to it. Otherwise, we'll fall
    // through to the end of the switch and remove the switch operand from the
    // stack.
    if (heap[binding + 5]) jump(heap[binding + 5]);

    def_label(lbl3);

    // If we fell through the switch, we need to remove the switch operand.
    // This is done before lbl1 because the stack is adjusted before the break statement.
    grow_stack(-1);
    grow_fs(-1);

    def_label(lbl1); // End of switch label

    cgc_fs = save_fs;
    cgc_locals = save_locals;

  } else if (op == CASE_KW) {

    binding = cgc_lookup_enclosing_switch(cgc_locals);

    // Logic is as follows:
    //  if falling through:
    //    jump to statements
    //  else if top_of_stack == case_value:
    //   jump to statements
    // else:
    //   jump to next case
    if (binding != 0) {
      lbl1 = alloc_label(0);                  // skip case when falling through
      jump(lbl1);
      def_label(heap[binding + 4]);           // false jump location of previous case
      heap[binding + 4] = alloc_label(0);     // create false jump location for current case
      codegen_rvalue(get_child_(CASE_KW, node, 0)); // evaluate case expression and compare it
      pop_reg(reg_Y); grow_fs(-1);
      mov_reg_mem(reg_X, reg_SP, 0);          // get switch operand without popping it
      jump_cond_reg_reg(EQ, lbl1, reg_X, reg_Y);
      jump(heap[binding + 4]);                // condition is false => jump to next case
      def_label(lbl1);                        // start of case conditional block
      codegen_statement(get_child_(CASE_KW, node, 1));  // case statement
    } else {
      fatal_error("case outside of switch");
    }

  } else if (op == DEFAULT_KW) {

    binding = cgc_lookup_enclosing_switch(cgc_locals);

    if (binding != 0) {
      if (heap[binding + 5]) fatal_error("default already defined in switch");
      heap[binding + 5] = alloc_label(0);                 // create label for default
      def_label(heap[binding + 5]);                       // default label
      codegen_statement(get_child_(DEFAULT_KW, node, 0)); // default statement
    } else {
      fatal_error("default outside of switch");
    }

  } else if (op == BREAK_KW) {

    binding = cgc_lookup_enclosing_loop_or_switch(cgc_locals);
    if (binding != 0) {
      grow_stack(heap[binding+2] - cgc_fs);
      jump(heap[binding+3]); // jump to break label
    } else {
      fatal_error("break is not in the body of a loop");
    }

  } else if (op == CONTINUE_KW) {

    binding = cgc_lookup_enclosing_loop(cgc_locals);
    if (binding != 0 && heap[binding+4] != 0) {
      grow_stack(heap[binding+2] - cgc_fs);
      jump(heap[binding+4]); // jump to continue label
    } else {
      fatal_error("continue is not in the body of a loop");
    }

  } else if (op == RETURN_KW) {

    if (get_child_(RETURN_KW, node, 0) != 0) {
      codegen_rvalue(get_child_(RETURN_KW, node, 0));
      pop_reg(reg_X);
      grow_fs(-1);
    }

    grow_stack(-cgc_fs);

    ret();

  } else if (op == '{') {

    codegen_body(node);

  } else if (op == ':') {

    binding = cgc_lookup_goto_label(get_val_(IDENTIFIER, get_child_(':', node, 0)), cgc_locals_fun);

    if (binding == 0) {
      cgc_add_goto_label(get_val_(IDENTIFIER, get_child_(':', node, 0)), alloc_goto_label());
      binding = cgc_locals_fun;
    }

    def_goto_label(heap[binding + 3]);
    codegen_statement(get_child_(':', node, 1)); // labelled statement

  } else if (op == GOTO_KW) {

    codegen_goto(node);

  } else {

    codegen_rvalue(node);
    pop_reg(reg_X);
    grow_fs(-1);

  }
}

function add_params(ast params) {
  ast decl, type;
  int ident;

  while (params != 0) {
    decl = car_(DECL, params);
    ident = get_val_(IDENTIFIER, get_child__(DECL, IDENTIFIER, decl, 0));
    type = get_child_(DECL, decl, 1);

    // Array to pointer decay
    if (get_op(type) == '[') { type = pointer_type(dereference_type(type), false); }

    if (cgc_lookup_var(ident, cgc_locals) != 0) fatal_error("add_params: duplicate parameter");

    cgc_add_local_param(ident, type_width(type, false, true) / WORD_SIZE, type);
    params = tail(params);
  }
}

void codegen_glo_fun_decl(ast node) {
  ast decl = get_child__(FUN_DECL, DECL, node, 0);
  ast body = get_child_opt_(FUN_DECL, '{', node, 1);
  ast name_probe = get_val_(IDENTIFIER, get_child__(DECL, IDENTIFIER, decl, 0));
  ast fun_type = get_child__(DECL, '(', decl, 1);
  ast params = get_child_opt_('(', LIST, fun_type, 1);
  ast fun_return_type = get_child_('(', fun_type, 0);
  int binding;
  int save_locals_fun = cgc_locals_fun;

  if (is_aggregate_type(fun_return_type)) {
    fatal_error("Returning arrays or structs from function not supported");
  }

  // If the function is main
  if (name_probe == MAIN_ID) {
    // Check if main returns an exit code.
    if (get_op(fun_return_type) != VOID_KW) main_returns = true;
  }

  binding = cgc_lookup_fun(name_probe, cgc_globals);

  if (binding == 0) {
    cgc_add_global_fun(name_probe, alloc_label(STRING_BUF(name_probe)), fun_type);
    binding = cgc_globals;
  }

  // Poor man's debug info
  debug_interrupt(); // Marker to helps us find the function in the disassembly
  codegen_string(name_probe);

  def_label(heap[binding+4]);

  cgc_fs = -1; // space for return address
  cgc_locals = 0;
  add_params(params);
  cgc_fs = 0;

  codegen_body(body);

  grow_stack(-cgc_fs);
  cgc_fs = 0;

  ret();

  cgc_locals_fun = save_locals_fun;
}

// For now, we don't do anything with the declarations in a typedef.
// The only thing we need to do is to call handle_enum_struct_union_type_decl
// on the type specifier, which is the same for all declarations.
void handle_typedef(ast node) {
  ast decls = get_child__(TYPEDEF_KW, LIST, node, 0);
  ast decl = car_(DECL, decls);
  ast type = get_child_(DECL, decl, 1);

  handle_enum_struct_union_type_decl(get_type_specifier(type));
}

void codegen_glo_decl(ast node) {
  ast decls;
  int op = get_op(node);

  if (op == DECLS) {
    // AUTO_KW and REGISTER_KW can simply be ignored. STATIC_KW is the default
    // storage class for global variables since pnut-sh only supports 1
    // translation unit.
    if (get_child_(DECLS, node, 1) == EXTERN_KW) fatal_error("Extern storage class specifier not supported");

    decls = get_child__(DECLS, LIST, node, 0); // Declaration list
    while (decls != 0) { // Multiple variable declarations
      codegen_glo_var_decl(car_(DECL, decls));
      decls = tail(decls); // Next variable declaration
    }
  } else if (op == FUN_DECL) {
    codegen_glo_fun_decl(node);
  } else if (op == TYPEDEF_KW) {
    handle_typedef(node);
  } else if (op == ENUM_KW || op == STRUCT_KW || op == UNION_KW) {
    handle_enum_struct_union_type_decl(node);
  } else {
    putstr("op="); putint(op);
    putstr(" with "); putint(get_nb_children(node)); putstr(" children\n");
    fatal_error("codegen_glo_decl: unexpected declaration");
  }
}

void rt_putchar() {
  push_reg(reg_X);            // Allocate buffer on stack containing the character
  mov_reg_imm(reg_X, 1);      // reg_X = file descriptor (stdout)
  mov_reg_reg(reg_Y, reg_SP); // reg_Y = buffer size
  mov_reg_imm(reg_Z, 1);      // reg_Z = buffer address
  os_write();
  pop_reg(reg_X);             // Deallocate buffer
}

void rt_debug(char* msg) {
  while (*msg != 0) {
    mov_reg_imm(reg_X, *msg);
    rt_putchar();
    msg += 1;
  }
}

void rt_crash(char* msg) {
  rt_debug(msg);
  mov_reg_imm(reg_X, 42); // exit code
  os_exit();
}

void rt_fgetc(int fd_reg) {
  int success_lbl = alloc_label("rt_fgetc_success");
  push_reg(reg_X);            // Allocate buffer on stack, initialized with some random value
  mov_reg_reg(reg_X, fd_reg); // reg_X = file descriptor (stdin)
  mov_reg_reg(reg_Y, reg_SP); // reg_Y = buffer size
  mov_reg_imm(reg_Z, 1);      // reg_Z = buffer address
  os_read();                  // reg_X = number of bytes read, buffer[0] = character

  pop_reg(reg_Z);             // Get character from buffer and deallocate buffer
  mov_reg_imm(reg_Y, 0);      // If read returned 0, then we're at EOF (-1)
  jump_cond_reg_reg(NE, success_lbl, reg_X, reg_Y);
  mov_reg_imm(reg_Z, -1);     // mov  eax, -1  # -1 on EOF
  def_label(success_lbl);     // end label
  mov_reg_reg(reg_X, reg_Z);  // return value
}

void rt_fopen() {
  int fopen_success_lbl = alloc_label("fopen_success");

  mov_reg_mem(reg_X, reg_SP, WORD_SIZE);
  mov_reg_imm(reg_Y, 0); // mode
  mov_reg_imm(reg_Z, 0); // flag
  os_open();
  // If open fails, it returns -1, but we need to return NULL
  mov_reg_imm(reg_Y, 0);
  jump_cond_reg_reg(GE, fopen_success_lbl, reg_X, reg_Y);
  mov_reg_imm(reg_X, 0); // NULL
  def_label(fopen_success_lbl);
}

void rt_malloc() {
  int end_lbl = alloc_label("rt_malloc_success");

  mov_reg_mem(reg_Y, reg_glo, WORD_SIZE); // Bump pointer
  add_reg_reg(reg_X, reg_Y);              // New bump pointer
  mov_reg_mem(reg_Y, reg_glo, 0);         // Heap start
  add_reg_imm(reg_Y, RT_HEAP_SIZE);       // End of heap

  // Make sure the heap is large enough.
  // new bump pointer (reg_x) >= end of heap (reg_y)
  jump_cond_reg_reg(LE, end_lbl, reg_X, reg_Y);
  rt_crash("Heap overflow\n");

  def_label(end_lbl);
  mov_reg_mem(reg_Y, reg_glo, WORD_SIZE); // Old bump pointer
  mov_mem_reg(reg_glo, WORD_SIZE, reg_X); // Adjust the bump pointer
  mov_reg_reg(reg_X, reg_Y);              // Return the old bump pointer
}

void codegen_end() {
  def_label(setup_lbl);

  // Allocate some space for the global variables.
  // The global variables used to be on the stack, but because the stack has a
  // limited size, it is better to allocate a separate memory region so global
  // variables are not limited by the stack size.
  //
  // We then allocate a separate memory region for the heap. Having a separate
  // memory space for the heap makes it easier to detect out-of-bound accesses
  // on global variables.
  //
  // Regarding initialization, os_allocate_memory uses mmap with the
  // MAP_ANONYMOUS flag so the memory should already be zeroed.
  //
  os_allocate_memory(cgc_global_alloc);   // Returns the globals table start address in reg_X
  mov_reg_reg(reg_glo, reg_X);            // reg_glo = globals table start

  os_allocate_memory(RT_HEAP_SIZE);       // Returns the heap start address in reg_X
  mov_mem_reg(reg_glo, 0, reg_X);         // Set init heap start
  mov_mem_reg(reg_glo, WORD_SIZE, reg_X); // init bump pointer

  jump(init_start_lbl);

  def_label(init_next_lbl);
  setup_proc_args(0);
  call(main_lbl);
  if (!main_returns) mov_reg_imm(reg_X, 0); // exit process with 0 if main returns void
  push_reg(reg_X); // exit process with result of main
  push_reg(reg_X); // dummy return address (exit never needs it)

  // exit function
  def_label(exit_lbl);
  mov_reg_mem(reg_X, reg_SP, WORD_SIZE);
  os_exit();

  // read function
  def_label(read_lbl);
  mov_reg_mem(reg_X, reg_SP, WORD_SIZE);
  mov_reg_mem(reg_Y, reg_SP, 2*WORD_SIZE);
  mov_reg_mem(reg_Z, reg_SP, 3*WORD_SIZE);
  os_read();
  ret();

  // write function
  def_label(write_lbl);
  mov_reg_mem(reg_X, reg_SP, WORD_SIZE);
  mov_reg_mem(reg_Y, reg_SP, 2*WORD_SIZE);
  mov_reg_mem(reg_Z, reg_SP, 3*WORD_SIZE);
  os_write();
  ret();

  // open function
  // Regarding the mode parameter, it is required if the flag allows the
  // creation of a new file. Otherwise, it may be omitted and is ignored by the
  // OS.
  // The manual says:
  // > If neither O_CREAT nor O_TMPFILE is specified in flags, then mode is
  // > ignored (and can thus be specified as 0, or simply omitted).  The mode
  // > argument must be supplied if O_CREAT or O_TMPFILE is specified in flags;
  // > if it is not supplied, some arbitrary bytes from the stack will be
  // > applied as the file mode.
  def_label(open_lbl);
  mov_reg_mem(reg_X, reg_SP, WORD_SIZE);
  mov_reg_mem(reg_Y, reg_SP, 2*WORD_SIZE);
  mov_reg_mem(reg_Z, reg_SP, 3*WORD_SIZE);
  os_open();
  ret();

  // close function
  def_label(close_lbl);
  // fclose is just like close because FILE * is just the file descriptor in the builtin libc
  def_label(fclose_lbl);
  mov_reg_mem(reg_X, reg_SP, WORD_SIZE);
  os_close();
  ret();

  // seek function
  def_label(seek_lbl);
  mov_reg_mem(reg_X, reg_SP, WORD_SIZE);   // fd
  mov_reg_mem(reg_Y, reg_SP, 2*WORD_SIZE); // offset
  mov_reg_mem(reg_Z, reg_SP, 3*WORD_SIZE); // whence
  os_seek();
  ret();

  // unlink function
  def_label(unlink_lbl);
  mov_reg_mem(reg_X, reg_SP, WORD_SIZE);   // filename
  os_unlink();
  ret();

  // mkdir function
  def_label(mkdir_lbl);
  mov_reg_mem(reg_X, reg_SP, WORD_SIZE);   // pathname
  mov_reg_mem(reg_Y, reg_SP, 2*WORD_SIZE); // mode
  os_mkdir();

  // chmod function
  def_label(chmod_lbl);
  mov_reg_mem(reg_X, reg_SP, WORD_SIZE);   // pathname
  mov_reg_mem(reg_Y, reg_SP, 2*WORD_SIZE); // mode
  os_chmod();
  ret();

  // stat function
  def_label(access_lbl);
  mov_reg_mem(reg_X, reg_SP, WORD_SIZE);   // pathname
  mov_reg_mem(reg_Y, reg_SP, 2*WORD_SIZE); // mode
  os_access();
  ret();

  // putchar function
  def_label(putchar_lbl);
  mov_reg_mem(reg_X, reg_SP, WORD_SIZE);
  rt_putchar();
  ret();

  // getchar function
  def_label(getchar_lbl);
  mov_reg_imm(reg_X, 0); // stdin
  rt_fgetc(reg_X);
  ret();

  // fopen function
  def_label(fopen_lbl);
  rt_fopen();
  ret();

  // fgetc function
  def_label(fgetc_lbl);
  mov_reg_mem(reg_X, reg_SP, WORD_SIZE);
  rt_fgetc(reg_X);
  ret();

  // malloc function
  def_label(malloc_lbl);
  mov_reg_mem(reg_X, reg_SP, WORD_SIZE);
  rt_malloc();
  ret();

  // free function
  def_label(free_lbl);
  mov_reg_mem(reg_X, reg_SP, WORD_SIZE);
  // Free is NO-OP
  ret();

  // printf function stub
  def_label(printf_lbl);
  rt_crash("printf is not supported yet.\n");
  ret();

  assert_all_labels_defined();

  generate_exe();
}

// ELF file output

// Minimal i386 bit ELF header.
// https://web.archive.org/web/20240409140025/http://www.muppetlabs.com/~breadbox/software/tiny/teensy.html
// https://web.archive.org/web/20240414151854/https://en.wikipedia.org/wiki/Executable_and_Linkable_Format

void write_elf_e_header() {
  write_4_i8(0x7f, 0x45, 0x4c, 0x46); // e_ident
  write_4_i8(0x01, 0x01, 0x01, 0x00);
  write_4_i8(0x00, 0x00, 0x00, 0x00);
  write_4_i8(0x00, 0x00, 0x00, 0x00);
  write_2_i8(0x02, 0x00);             // e_type (2=ET_EXEC = an executable file)
  write_2_i8(0x03, 0x00);             // e_machine (3=x86)
  write_4_i8(0x01, 0x00, 0x00, 0x00); // e_version
  write_4_i8(0x54, 0x80, 0x04, 0x08); // e_entry
  write_4_i8(0x34, 0x00, 0x00, 0x00); // e_phoff
  write_4_i8(0x00, 0x00, 0x00, 0x00); // e_shoff
  write_4_i8(0x00, 0x00, 0x00, 0x00); // e_flags
  write_2_i8(0x34, 0x00);             // e_ehsize
  write_2_i8(0x20, 0x00);             // e_phentsize
  write_2_i8(0x01, 0x00);             // e_phnum
  write_2_i8(0x00, 0x00);             // e_shentsize
  write_2_i8(0x00, 0x00);             // e_shnum
  write_2_i8(0x00, 0x00);             // e_shstrndx
}

void write_elf_p_header() {
  write_i32_le(1);                 // p_type (1=PT_LOAD = a loadable segment)
  write_i32_le(0);                 // p_offset
  write_i32_le(0x08048000);        // p_vaddr
  write_i32_le(0x08048000);        // p_paddr
  write_i32_le(0x54 + code_alloc); // p_filesz
  write_i32_le(0x54 + code_alloc); // p_memsz
  write_i32_le(5);                 // p_flags
  write_i32_le(0x1000);            // p_align
}

// common part of machine code generators
function generate_exe() {
  int i = 0;

  write_elf_e_header();
  write_elf_p_header();

  while (i < code_alloc) {
    write_i8(code[i]);
    i += 1;
  }
}
// end exe.c

// Registers common to i386 and x86-64 (E and R prefixes are omitted).

const int AX = 0;
const int CX = 1;
const int DX = 2;
const int BX = 3;
const int SP = 4;
const int BP = 5;
const int SI = 6;
const int DI = 7;
const int R8 = 8;
const int R9 = 9;
const int R10 = 10;
const int R11 = 11;
const int R12 = 12;
const int R13 = 13;
const int R14 = 14;
const int R15 = 15;

// On old versions of gcc (such as on debian woody), setting a const variable
// to another const variable produces an error. This is a workaround.
const int reg_X = 0; // AX: temporary register
const int reg_Y = 1; // CX: temporary register
const int reg_Z = 2; // DX: temporary register
const int reg_SP = 4; // SP: stack pointer
const int reg_glo = 3; // BX: global variables table

#define rex_prefix(reg1, reg2) ((void)0)

void mod_rm(int reg1, int reg2) {
  // ModR/M byte
  //
  // It is used to encode the operand(s) to an instruction.
  // The format is the following:
  // Bit    7   6   5   4   3   2   1   0
  //        -----   ---------   ---------
  // Usage   Mod       Reg         R/M
  //
  // Operations that use 1 operand generally use the R/M field to specify it.
  // In that case, the Reg field may be repurposed as an "opcode extension" to
  // allow multiple instructions to share the same opcode. This is generally
  // indicated as /digit in the opcode table.
  //
  // The mod field encodes the addressing mode for the register/memory ("r/m") operand.
  // When the mod field is 11, the r/m field is used to specify a register operand.
  // Otherwise, 00, 01 and 10 specify different addressing modes.
  //
  // When mod specifies an addressing mode, the ModR/M byte may be followed by
  // a SIB byte (Scale Index Base) and/or a displacement.
  //
  // See https://web.archive.org/web/20250207155122/https://en.wikipedia.org/wiki/ModR/M
  //
  // For our purposes, we only use the case where both operands are registers,
  // and so we always emit 0xc0 (mod = 11) with the reg1 and reg2 fields.
  emit_i8(0xc0 + ((reg1 & 7) << 3) + (reg2 & 7));
}

// ModR/M byte with /digit opcode extension => The reg1 field is repurposed as an opcode extension.
#define mod_rm_slash_digit(digit, reg1) mod_rm(digit, reg1)

// For instructions with 2 register operands
void op_reg_reg(int opcode, int dst, int src, int reg_width) {
  // 16-bit operand size override prefix
  // See section on Legacy Prefixes: https://web.archive.org/web/20250210181519/https://wiki.osdev.org/X86-64_Instruction_Encoding#ModR/M
  if (reg_width == 2) emit_i8(0x66);
  if (reg_width == 8) rex_prefix(src, dst);
  emit_i8(opcode);
  mod_rm(src, dst);
}

// For instructions with 1 register operand and /digit opcode extension
void op_reg_slash_digit(int opcode, int digit, int reg) {
  rex_prefix(0, reg);
  emit_i8(opcode);
  mod_rm_slash_digit(digit, reg);
}

function add_reg_reg(int dst, int src) {

  // ADD dst_reg, src_reg ;; dst_reg = dst_reg + src_reg
  // See: https://web.archive.org/web/20240407051903/https://www.felixcloutier.com/x86/add

  op_reg_reg(0x01, dst, src, WORD_SIZE);
}

function or_reg_reg (int dst, int src) {

  // OR dst_reg, src_reg ;; dst_reg = dst_reg | src_reg
  // See: https://web.archive.org/web/20231002205127/https://www.felixcloutier.com/x86/or

  op_reg_reg(0x09, dst, src, WORD_SIZE);
}

function and_reg_reg(int dst, int src) {

  // AND dst_reg, src_reg ;; dst_reg = dst_reg & src_reg
  // See: https://web.archive.org/web/20240228122102/https://www.felixcloutier.com/x86/and

  op_reg_reg(0x21, dst, src, WORD_SIZE);
}

function sub_reg_reg(int dst, int src) {

  // SUB dst_reg, src_reg ;; dst_reg = dst_reg - src_reg
  // See: https://web.archive.org/web/20240118202232/https://www.felixcloutier.com/x86/sub

  op_reg_reg(0x29, dst, src, WORD_SIZE);
}

function xor_reg_reg(int dst, int src) {

  // XOR dst_reg, src_reg ;; dst_reg = dst_reg ^ src_reg
  // See: https://web.archive.org/web/20240323052259/https://www.felixcloutier.com/x86/xor

  op_reg_reg(0x31, dst, src, WORD_SIZE);
}

function cmp_reg_reg(int dst, int src) {

  // CMP dst_reg, src_reg  ;; Set condition flags according to dst_reg-src_reg
  // See: https://web.archive.org/web/20240407051947/https://www.felixcloutier.com/x86/cmp
  // Note: For byte comparison, opcode is 0x38, for word/dword/qword comparison, opcode is 0x39

  op_reg_reg(0x39, dst, src, WORD_SIZE);
}

function mov_reg_reg(int dst, int src) {

  // MOV dst_reg, src_reg  ;; dst_reg = src_reg
  // See: https://web.archive.org/web/20240407051903/https://www.felixcloutier.com/x86/mov

  op_reg_reg(0x89, dst, src, WORD_SIZE);
}

function mov_reg_imm(int dst, int imm) {

  // MOV dst_reg, imm  ;; Move 32 bit immediate value to register
  // See: https://web.archive.org/web/20240407051903/https://www.felixcloutier.com/x86/mov

  rex_prefix(0, dst);
  emit_i8(0xb8 + (dst & 7));
  emit_i32_le(imm);
}

function add_reg_imm(int dst, int imm) {

  // ADD dst_reg, imm  ;; Add 32 bit immediate value to register
  // See: https://web.archive.org/web/20240407051903/https://www.felixcloutier.com/x86/add

  rex_prefix(0, dst);
  emit_i8(0x81);
  mod_rm(0, dst);
  emit_i32_le(imm);
}

function add_reg_lbl(int dst, int lbl) {

  // ADD dst_reg, rel addr  ;; Add 32 bit displacement between next instruction and label to register
  // See: https://web.archive.org/web/20240407051903/https://www.felixcloutier.com/x86/add

  rex_prefix(0, dst);
  emit_i8(0x81);
  mod_rm(0, dst);
  use_label(lbl); // 32 bit placeholder for distance
}

function mov_memory(int op, int reg, int base, int offset, int reg_width) {

  // Move word between register and memory
  // See: https://web.archive.org/web/20240407051903/https://www.felixcloutier.com/x86/mov

  // 16-bit operand size override prefix
  // See section on Legacy Prefixes: https://web.archive.org/web/20250210181519/https://wiki.osdev.org/X86-64_Instruction_Encoding#ModR/M
  if (reg_width == 2) emit_i8(0x66);
  if (reg_width == 8) rex_prefix(reg, base);
  emit_i8(op);
  emit_i8(0x80 + (reg & 7) * 8 + (base & 7));
  if (base == SP || base == R12) emit_i8(0x24); // SIB byte. See 32/64-bit addressing mode
  emit_i32_le(offset);
}

function mov_memory_extend(int op, int reg, int base, int offset, bool include_0f) {

  // Move word between register and memory with sign extension
  // See: https://web.archive.org/web/20250121105942/https://www.felixcloutier.com/x86/movsx:movsxd
  // And  https://web.archive.org/web/20250109202608/https://www.felixcloutier.com/x86/movzx

  // From webpage:
  //  > The use of MOVSXD without REX.W in 64-bit mode is discouraged. Regular
  //  > MOV should be used instead of using MOVSXD without REX.W.
  rex_prefix(reg, base);
  if (include_0f) emit_i8(0x0f); // Most sign/zero extend instructions have a 0x0f prefix
  emit_i8(op);
  emit_i8(0x80 + (reg & 7) * 8 + (base & 7));
  if (base == SP || base == R12) emit_i8(0x24); // SIB byte. See 32/64-bit addressing mode
  emit_i32_le(offset);
}

function mov_mem8_reg(int base, int offset, int src) {

  // MOVB [base_reg + offset], src_reg  ;; Move byte from register to memory
  // See: https://web.archive.org/web/20240407051903/https://www.felixcloutier.com/x86/mov

  mov_memory(0x88, src, base, offset, 1);
}

function mov_mem16_reg(int base, int offset, int src) {

  // MOVB [base_reg + offset], src_reg  ;; Move word (2 bytes) from register to memory
  // See: https://web.archive.org/web/20240407051903/https://www.felixcloutier.com/x86/mov

  mov_memory(0x89, src, base, offset, 2);
}

function mov_mem32_reg(int base, int offset, int src) {

  // MOVB [base_reg + offset], src_reg  ;; Move dword (4 bytes) from register to memory
  // See: https://web.archive.org/web/20240407051903/https://www.felixcloutier.com/x86/mov

  mov_memory(0x89, src, base, offset, 4);
}

function mov_reg_mem8(int dst, int base, int offset) {

  // MOVB dst_reg, [base_reg + offset]  ;; Move byte from memory to register, zero-extended
  // See: https://web.archive.org/web/20250109202608/https://www.felixcloutier.com/x86/movzx

  mov_memory_extend(0xb6, dst, base, offset, true);
}

function mov_reg_mem16(int dst, int base, int offset) {

  // MOVB dst_reg, [base_reg + offset]  ;; Move word (2 bytes) from memory to register, zero-extended
  // See: https://web.archive.org/web/20250109202608/https://www.felixcloutier.com/x86/movzx

  mov_memory_extend(0xb7, dst, base, offset, true);
}

function mov_reg_mem32(int dst, int base, int offset) {

  // MOV dst_reg, [base_reg + offset]  ;; Move dword (4 bytes) from memory to register, zero-extended
  // See: https://web.archive.org/web/20240407051903/https://www.felixcloutier.com/x86/mov

  // Operations writing to the lower 32 bits of a register zero-extend the
  // result, so there's no movzx instruction for 32-bit operands.
  mov_memory(0x8b, dst, base, offset, 4);
}

function mov_reg_mem8_sign_ext(int dst, int base, int offset) {

  // MOVB dst_reg, [base_reg + offset]  ;; Move byte from memory to register, sign-extended
  // See: https://web.archive.org/web/20250121105942/https://www.felixcloutier.com/x86/movsx:movsxd

  mov_memory_extend(0xbe, dst, base, offset, true);
}

function mov_reg_mem16_sign_ext(int dst, int base, int offset) {

  // MOVB dst_reg, [base_reg + offset]  ;; Move word (2 bytes) from memory to register, sign-extended
  // See: https://web.archive.org/web/20250121105942/https://www.felixcloutier.com/x86/movsx:movsxd

  mov_memory_extend(0xbf, dst, base, offset, true);
}

function imul_reg_reg(int dst, int src) {

  // IMUL dst_reg, src_reg ;; dst_reg = dst_reg * src_reg
  // See: https://web.archive.org/web/20240228122220/https://www.felixcloutier.com/x86/imul

  rex_prefix(dst, src);
  emit_2_i8(0x0f, 0xaf);
  mod_rm(dst, src);
}

function mul_reg_reg(int dst, int src) {

  // For our purposes, this is the same as imul_reg_reg.
  // https://web.archive.org/web/20240914145321/https://stackoverflow.com/questions/42587607/why-is-imul-used-for-multiplying-unsigned-numbers

  imul_reg_reg(dst, src);
}

function idiv_reg(int src) {

  // IDIV src_reg ;; AX = DX:AX / src_reg ; DX = DX:AX % src_reg
  // See: https://web.archive.org/web/20240407195950/https://www.felixcloutier.com/x86/idiv

  op_reg_slash_digit(0xf7, 7, src);
}

void div_reg(int src) {

  // DIV src_reg ;; AX = DX:AX / src_reg ; DX = DX:AX % src_reg
  // See: https://web.archive.org/web/20250202075400/https://www.felixcloutier.com/x86/div

  op_reg_slash_digit(0xf7, 6, src);
}

void cdq_cqo() {

  // CDQ ;; Convert Doubleword to Quadword (EDX:EAX = sign-extend EAX)
  // x86-64: CQO ;; Convert Quadword to Octoword (RDX:RAX = sign-extend RAX)
  // See: https://web.archive.org/web/20240118201956/https://www.felixcloutier.com/x86/cwd:cdq:cqo

  rex_prefix(0, 0);
  emit_i8(0x99);
}

function idiv_reg_reg(int dst, int src) {

  // Computes dst_reg = dst_reg / src_reg
  // This is not an actual instruction on x86. The operation
  // is emulated with a sequence of instructions that will clobber the
  // registers AX and DX.

  mov_reg_reg(AX, dst);
  cdq_cqo(); // sign extend AX to DX:AX
  idiv_reg(src);
  mov_reg_reg(dst, AX);
}

function irem_reg_reg(int dst, int src) {

  // Computes dst_reg = dst_reg % src_reg
  // This is not an actual instruction on x86. The operation
  // is emulated with a sequence of instructions that will clobber the
  // registers AX and DX.

  mov_reg_reg(AX, dst);
  cdq_cqo(); // sign extend AX to DX:AX
  idiv_reg(src);
  mov_reg_reg(dst, DX);
}

function div_reg_reg(int dst, int src) {

  // Computes dst_reg = dst_reg / src_reg
  // This is not an actual instruction on x86. The operation
  // is emulated with a sequence of instructions that will clobber the
  // registers AX and DX.

  mov_reg_reg(AX, dst);
  mov_reg_imm(DX, 0); // Clear DX
  div_reg(src);
  mov_reg_reg(dst, AX);
}

function rem_reg_reg(int dst, int src) {

  // Computes dst_reg = dst_reg % src_reg
  // This is not an actual instruction on x86. The operation
  // is emulated with a sequence of instructions that will clobber the
  // registers AX and DX.

  mov_reg_reg(AX, dst);
  mov_reg_imm(DX, 0); // Clear DX
  div_reg(src);
  mov_reg_reg(dst, DX);
}

function s_l_reg_cl(int dst) {

  // SHL dst_reg, cl ;; dst_reg = dst_reg << cl (arithmetic or logical shift, they are the same)
  // See: https://web.archive.org/web/20240405194323/https://www.felixcloutier.com/x86/sal:sar:shl:shr

  op_reg_slash_digit(0xd3, 4, dst);
}

function s_l_reg_reg(int dst, int src) {

  // Computes dst_reg = dst_reg << src_reg (arithmetic or logical shift, they are the same)
  // This is not an actual instruction on x86. The operation
  // is emulated with a sequence of instructions that clobbers the
  // register CX, and does not work if dst = CX.

  mov_reg_reg(CX, src);
  s_l_reg_cl(dst);
}

function sar_reg_cl(int dst) {

  // SAR dst_reg, cl ;; dst_reg = dst_reg >> cl (arithmetic shift)
  // See: https://web.archive.org/web/20240405194323/https://www.felixcloutier.com/x86/sal:sar:shl:shr

  op_reg_slash_digit(0xd3, 7, dst);
}

function sar_reg_reg(int dst, int src) {

  // Computes dst_reg = dst_reg >> src_reg (arithmetic shift)
  // This is not an actual instruction on x86. The operation
  // is emulated with a sequence of instructions that clobbers the
  // register CX, and does not work if dst = CX.

  mov_reg_reg(CX, src);
  sar_reg_cl(dst);
}

function shr_reg_cl(int dst) {

  // SHR dst_reg, cl ;; dst_reg = dst_reg >> cl (logical shift)
  // See: https://web.archive.org/web/20240405194323/https://www.felixcloutier.com/x86/sal:sar:shl:shr

  op_reg_slash_digit(0xd3, 5, dst);
}

function shr_reg_reg(int dst, int src) {

  // Computes dst_reg = dst_reg >> src_reg (logical shift)
  // This is not an actual instruction on x86. The operation
  // is emulated with a sequence of instructions that clobbers the
  // register CX, and does not work if dst = CX.

  mov_reg_reg(CX, src);
  shr_reg_cl(dst);
}

function push_reg(int src) {

  // PUSH src_reg  ;; Push word from register to stack
  // See: https://web.archive.org/web/20240407051929/https://www.felixcloutier.com/x86/push

  emit_i8(0x50 + src);
}

function pop_reg (int dst) {

  // POP dst_reg  ;; Pop word from stack to register
  // See: https://web.archive.org/web/20240204122208/https://www.felixcloutier.com/x86/pop

  emit_i8(0x58 + dst);
}

function jump(int lbl) {

  // JMP rel32  ;; Jump to 32 bit displacement relative to next instruction
  // See: https://web.archive.org/web/20240407051904/https://www.felixcloutier.com/x86/jmp

  emit_i8(0xe9);
  use_label(lbl);
}

function jump_rel(int offset) {

  // JMP rel32  ;; Jump to 32 bit displacement relative to next instruction
  // See: https://web.archive.org/web/20240407051904/https://www.felixcloutier.com/x86/jmp

  emit_i8(0xe9);
  emit_i32_le(offset);
}

function call(int lbl) {

  // CALL rel32  ;; Call to 32 bit displacement relative to next instruction
  // See: https://web.archive.org/web/20240323052931/https://www.felixcloutier.com/x86/call

  emit_i8(0xe8);
  use_label(lbl);
}

function call_reg(int reg) {

  // CALL reg  ;; Indirect call to address in register
  // See: https://web.archive.org/web/20240323052931/https://www.felixcloutier.com/x86/call

  emit_i8(0xff);
  mod_rm(2, reg);
}

function ret() {

  // RET  ;; Return to calling procedure
  // See: https://web.archive.org/web/20240302232015/https://www.felixcloutier.com/x86/ret

  emit_i8(0xc3);
}

function debug_interrupt() {

  // INT 3  ;; Debug interrupt
  // See: https://web.archive.org/web/20250118000553/https://www.felixcloutier.com/x86/intn:into:int3:int1

  emit_i8(0xcc);
}

// Conditions for use by jump_cond:

const int EQ   = 0x4; // x == y
const int NE   = 0x5; // x != y
const int LT   = 0xc; // x < y
const int LT_U = 0x2; // x < y  (Jump near if not above or equal (CF=1))
const int GE   = 0xd; // x >= y
const int GE_U = 0x3; // x >= y (Jump near if above or equal (CF=0))
const int LE   = 0xe; // x <= y
const int LE_U = 0x6; // x <= y (Jump near if below or equal (CF=1 or ZF=1))
const int GT   = 0xf; // x > y
const int GT_U = 0x7; // x > y  (Jump near if not below or equal (CF=0 and ZF=0))

function jump_cond(int cond, int lbl) {

  // JE rel32, JNE rel32, JL rel32, JLE rel32, JG rel32, JGE rel32, ...
  // Jump conditionally to 32 bit displacement relative to next instruction
  // See: https://web.archive.org/web/20231007122614/https://www.felixcloutier.com/x86/jcc

  emit_2_i8(0x0f, 0x80 + cond);
  use_label(lbl);
}

function jump_cond_reg_reg(int cond, int lbl, int reg1, int reg2) {
  cmp_reg_reg(reg1, reg2);
  jump_cond(cond, lbl);
}

void int_i8(int n) {

  // INT imm8 ;; Software interrupt with vector specified by immediate byte
  // See: https://web.archive.org/web/20240407051903/https://www.felixcloutier.com/x86/intn:into:int3:int1

  emit_2_i8(0xcd, n);
}

void setup_proc_args(int global_vars_size) {
  // See page 54 of Intel386 System V ABI document:
  // https://web.archive.org/web/20240107061436/https://www.sco.com/developers/devspecs/abi386-4.pdf
  // See page 29 of AMD64 System V ABI document:
  // https://refspecs.linuxbase.org/elf/x86_64-abi-0.99.pdf
  //
  // On x86-32, argc is at [esp+0] and the content of argv directly follows at program start.
  // At this point, we've initialized the globals table so the stack looks like this:
  // [esp + 0]: global table start (global_vars_size bytes long)
  // ...
  // [esp + global_vars_size]     : argc
  // [esp + global_vars_size + 4] : argv[0]
  // [esp + global_vars_size + 8] : argv[1]
  // ...
  // The main function expects argv to be a char**, so it's missing an indirection, which is added here.
  // The stack will then look like this:
  // [esp + 0] : argc
  // [esp + 4] : argv
  // [esp + 8] : global table start (global_vars_size bytes long)
  // ...
  // For x86-64, it works similarly with [rsp + 0] for argc and [rsp + 8] for argv.
  //
  // Note(13/02/2025): Global variables are now allocated in a separate memory region so global_vars_size is 0.

  mov_reg_reg(reg_X, SP);
  add_reg_imm(reg_X, global_vars_size + WORD_SIZE); // compute address of argv
  push_reg(reg_X); // push argv address

  mov_reg_mem(reg_Y, reg_X, -WORD_SIZE); // load argc
  push_reg(reg_Y); // push argc
}

function mov_reg_lbl(int reg, int lbl) {
  // Since we can't do rip-relative addressing in 32 bit mode, we need to push
  // the address to the stack and then some arithmetic to get the address in a
  // register.

  int lbl_for_pc = alloc_label("lbl_for_pc");

  call(lbl_for_pc);        // call lbl
  def_label(lbl_for_pc);   // end label
                           // <--- The stack now has the address of the next instruction
  pop_reg(reg);            // pop reg_X (1 byte)
  add_reg_lbl(reg, lbl);   // load address of label to reg_X (6 or 7 bytes if 32 or 64 bit)
  add_reg_imm(reg, WORD_SIZE == 8 ? 8 : 7); // adjust for the pop and add instructions
}

// For 32 bit linux.

// Regular system calls for 32 bit linux.
// The system call number is passed in the rax register.
// Other arguments are passed in ebx, ecx and edx.
// The return value is in rax.
// If the parameter registers are ebx, ecx or edx, the function assume they may
// be clobberred in the order of the mov instructions.
// i.e. syscall_3(SYS_READ, ..., ebx, ...) is not valid because ebx is clobberred by the first mov instructions.
// For syscalls that use less than 3 parameters, the extra register params are set to -1.
void syscall_3(int syscall_code, int bx_reg, int cx_reg, int dx_reg) {
  push_reg(BX);                  // save address of global variables table
  if (bx_reg >= 0) mov_reg_reg(BX, bx_reg);
  if (cx_reg >= 0) mov_reg_reg(CX, cx_reg);
  if (dx_reg >= 0) mov_reg_reg(DX, dx_reg);
  mov_reg_imm(AX, syscall_code); // AX = syscall_code
  int_i8(0x80);                  // syscall
  pop_reg(BX);                   // restore address of global variables table
}

void os_allocate_memory(int size) {
  push_reg(BX);           // save address of global variables table
  mov_reg_imm(AX, 192);   // mov  eax, 192 == SYS_MMAP2
  mov_reg_imm(BX, 0);     // mov  ebx, 0 | NULL
  mov_reg_imm(CX, size);  // mov  ecx, size | size
  mov_reg_imm(DX, 0x3);   // mov  edx, 0x3 | PROT_READ (0x1) | PROT_WRITE (0x2)
  mov_reg_imm(SI, 0x22);  // mov  esi, 0x21 | MAP_ANONYMOUS (0x20) | MAP_PRIVATE (0x2)
  mov_reg_imm(DI, -1);    // mov  edi, -1 (file descriptor)
  mov_reg_imm(BP, 0);     // mov  ebp, 0 (offset)
  int_i8(0x80);           // int  0x80     # system call
  pop_reg(BX);            // restore address of global variables table
}

void os_exit() {
  syscall_3(1, reg_X, -1, -1); // SYS_EXIT = 1
}

void os_read() {
  syscall_3(3, reg_X, reg_Y, reg_Z); // SYS_READ = 3
}

void os_write() {
  syscall_3(4, reg_X, reg_Y, reg_Z); // SYS_WRITE = 4
}

void os_open() {
  syscall_3(5, reg_X, reg_Y, reg_Z); // SYS_OPEN = 5
}

void os_close() {
  syscall_3(6, reg_X, -1, -1); // SYS_CLOSE = 6
}

void os_seek() {
  syscall_3(19, reg_X, reg_Y, reg_Z); // SYS_LSEEK = 19
}

void os_unlink() {
  syscall_3(10, reg_X, -1, -1); // SYS_UNLINK = 10
}

void os_mkdir() {
  syscall_3(39, reg_X, reg_Y, -1); // SYS_MKDIR = 39
}

void os_chmod() {
  syscall_3(15, reg_X, reg_Y, -1); // SYS_CHMOD = 15
}

void os_access() {
  syscall_3(21, reg_X, reg_Y, -1); // SYS_ACCESS = 21
}
// end x86.c

//-----------------------------------------------------------------------------

void handle_macro_D(char *opt) {
  char *start = opt;
  char *macro_buf;
  char *buf2;
  int acc;
  while (*opt != 0 && *opt != '=') opt += 1; // Find = sign if any

  macro_buf = malloc(opt - start + 1);
  memcpy(macro_buf, start, opt - start);
  macro_buf[opt - start] = '\0';

  if (*opt == '=') {
    opt += 1;
    if (*opt == '"') { // Start of string literal
      opt += 1;
      start = opt;
      while (*opt != 0 && *opt != '"') opt += 1;
      if (*opt == 0) fatal_error("Unterminated string literal");
      buf2 = malloc(opt - start + 1);
      memcpy(buf2, start, opt - start);
      buf2[opt - start] = '\0';
      init_builtin_string_macro(macro_buf, buf2);
      free(buf2);
    } else if ('0' <= *opt && *opt <= '9') { // Start of integer token
      acc = 0;
      while ('0' <= *opt && *opt <= '9') {
        acc *= 10;
        acc += *opt - '0';
        opt += 1;
      }
      if (*opt != 0) fatal_error("Invalid macro definition value");
      init_builtin_int_macro(macro_buf, acc);
    } else if (*opt == '\0') { // No value given, empty macro
      init_builtin_empty_macro(macro_buf);
    } else {
      fatal_error("Invalid macro definition value");
    }
  } else {
    // Default to 1 when no value is given
    init_builtin_int_macro(macro_buf, 1);
  }

  free(macro_buf);
}

function init_globals() {
  O_WRONLY = 01;
  O_CREAT  = 0100;
  O_TRUNC  = 01000;

  line_number = 1;
  column_number = 0;
  last_tok_line_number = 1;
  last_tok_column_number = 0;

  string_pool_alloc = 0;

// These parameters give a perfect hashing of the C keywords
  HASH_PARAM = 1026;
  HASH_PRIME = 1009;

  heap_alloc = HASH_PRIME;

  STRING_POOL_SIZE = 500000;
  string_pool = malloc(STRING_POOL_SIZE);
}

int main(int argc, char **argv) {
  int i;
  ast decl;

  init_c();
  init_globals();

  init_ident_table();
  init_pnut_macros();

  for (i = 1; i < argc; i += 1) {
    if (argv[i][0] == '-') {
      switch (argv[i][1]) {
        case 'o':
          // Output file name
          if (argv[i][2] == 0) { // rest of option is in argv[i + 1]
            i += 1;
            output_fd = open(argv[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
          } else {
            output_fd = open(argv[i] + 2, O_WRONLY | O_CREAT | O_TRUNC, 0644);
          }
          break;

        case 'D':
          if (argv[i][2] == 0) { // rest of option is in argv[i + 1]
            i += 1;
            handle_macro_D(argv[i]);
          } else {
            handle_macro_D(argv[i] + 2); // skip '-D'
          }
          break;
        case 'U':
          if (argv[i][2] == 0) { // rest of option is in argv[i + 1]
            i += 1;
            init_ident(IDENTIFIER, argv[i]);
          } else {
            init_ident(IDENTIFIER, argv[i] + 2); // skip '-U'
          }
          break;

        case 'I':
          if (include_search_path != 0) fatal_error("only one include path allowed");

          if (argv[i][2] == 0) { // rest of option is in argv[i + 1]
            i += 1;
            include_search_path = argv[i];
          } else {
            include_search_path = argv[i] + 2; // skip '-I'
          }
          break;

        default:
          putstr("Option "); putstr(argv[i]); putchar('\n');
          fatal_error("unknown option");
          break;
      }
    } else {
      // Options that don't start with '-' are file names
      include_file(argv[i], 0);
    }
  }

  if (fp == 0) {
    putstr("Usage: "); putstr(argv[0]); putstr(" <filename>\n");
    fatal_error("no input file");
  }

  ch = '\n';

  codegen_begin();
  get_tok();
  while (tok != EOF) {
    decl = parse_declaration(false);
    codegen_glo_decl(decl);
  }
  codegen_end();

  return 0;
}
