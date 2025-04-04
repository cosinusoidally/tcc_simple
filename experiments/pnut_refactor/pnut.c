#define O_WRONLY 01
#define O_CREAT  0100
#define O_TRUNC  01000

#define function int
#define var int
#define bool int
#define FILE int

#define ast int
#define true 1
#define false 0
#define EOF (-1)

var line_number = 1;
var column_number = 0;
var last_tok_line_number = 1;
var last_tok_column_number = 0;

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

void putstr(char *str) {
  while (*str) {
    putchar(*str);
    str += 1;
  }
}

void putint_aux(int n) {
  if (n <= -10) putint_aux(n / 10);
  putchar('0' - (n % 10));
}

void putint(int n) {
  if (n < 0) {
    putchar('-');
    putint_aux(n);
  } else {
    putint_aux(-n);
  }
}

void fatal_error(char *msg) {
  if (include_stack != 0) {
    putstr(include_stack->filepath); putchar(':');
    putint(last_tok_line_number); putchar(':'); putint(last_tok_column_number);
    putstr("  "); putstr(msg); putchar('\n');
  } else {
    putstr(msg); putchar('\n');
  }
  exit(1);
}

void syntax_error(char *msg) {
  putstr(include_stack->filepath); putchar(':');
  putint(last_tok_line_number); putchar(':'); putint(last_tok_column_number);
  putstr("  syntax error: "); putstr(msg); putchar('\n');
  exit(1);
}

// tokenizer

var ch;
var tok;
var val;

#define STRING_POOL_SIZE 500000
char string_pool[STRING_POOL_SIZE];
var string_pool_alloc = 0;
var string_start;
var hash;

// These parameters give a perfect hashing of the C keywords
#define HASH_PARAM 1026
#define HASH_PRIME 1009
#define HEAP_SIZE 2000000
int heap[HEAP_SIZE];
var heap_alloc = HASH_PRIME;

var alloc_result;

function alloc_obj(size) {
  alloc_result = heap_alloc;
  heap_alloc += size;

  if (heap_alloc > HEAP_SIZE) {
    fatal_error("heap overflow");
  }

  return alloc_result;
}

function get_op(node) {
  return heap[node] & 1023;
}

function get_nb_children(node) {
  return heap[node] >> 10;
}

function get_val(node) {
  return heap[node+1];
}

function set_val(node, val) {
  heap[node+1] = val;
}

function get_child(node, i) {
  return heap[node+i+1];
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

  heap[ast_result] = op + 1024;
  set_child(ast_result, 0, child0);

  return ast_result;
}

function new_ast2(op, child0, child1) {

  ast_result = alloc_obj(3);

  heap[ast_result] = op + 2048;
  set_child(ast_result, 0, child0);
  set_child(ast_result, 1, child1);

  return ast_result;
}

function new_ast3(op, child0, child1, child2) {

  ast_result = alloc_obj(4);

  heap[ast_result] = op + 3072;
  set_child(ast_result, 0, child0);
  set_child(ast_result, 1, child1);
  set_child(ast_result, 2, child2);

  return ast_result;
}

function new_ast4(op, child0, child1, child2, child3) {

  ast_result = alloc_obj(5);

  heap[ast_result] = op + 4096;
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
  if (nb_children == 0) nb_children = 1;

  ast_result = alloc_obj(nb_children + 1);

  heap[ast_result] = heap[orig]; // copy operator and nb of children
  for (i = 0; i < nb_children; i += 1) {
    set_child(ast_result, i, get_child(orig, i));
  }

  return ast_result;
}

// TODO: Use macro to avoid indirection?
// Functions used to create and access lists.
ast cons(int child0, int child1)    { return new_ast2(LIST, child0, child1); }
ast car(int pair)                   { return get_child_(LIST, pair, 0); }
ast car_(int expected_op, int pair) { return get_child__(LIST, expected_op, pair, 0); }
ast cdr(int pair)                   { return get_child_(LIST, pair, 1); }
ast cdr_(int expected_op, int pair) { return get_child_opt_(LIST, expected_op, pair, 1); }
void set_car(int pair, int value)    { set_child(pair, 0, value); }
void set_cdr(int pair, int value)    { set_child(pair, 1, value); }
#define tail(x) cdr_(LIST, x)

// Returns the only element of a singleton list, if it is a singleton list.
// Otherwise, returns 0.
ast list_singleton(ast list) {
  if (list != 0 && tail(list) == 0) {
    return car(list);
  } else {
    return 0;
  }
}

// Simple accessor to get the string from the string pool
function STRING_BUF(string_val) {
  return (string_pool + heap[string_val+1]);
}

function STRING_LEN(string_val) {
  return (heap[string_val+4]);
}

function STRING_BUF_END(string_val) {
  return (STRING_BUF(string_val) + STRING_LEN(string_val));
}

void begin_string() {
  string_start = string_pool_alloc;
  hash = 0;
}

// Append the current character (ch) to the string under construction in the pool
void accum_string() {
  hash = (ch + (hash ^ HASH_PARAM)) % HASH_PRIME;
  string_pool[string_pool_alloc] = ch;
  string_pool_alloc += 1;
  if (string_pool_alloc >= STRING_POOL_SIZE) {
    fatal_error("string pool overflow");
  }
}

// Append a character to the current string under construction in the pool
void accum_string_char(char c) {
  hash = (c + (hash ^ HASH_PARAM)) % HASH_PRIME;
  string_pool[string_pool_alloc] = c;
  string_pool_alloc += 1;
  if (string_pool_alloc >= STRING_POOL_SIZE) {
    fatal_error("string pool overflow");
  }
}

// Append a string from the string_pool to the string under construction
void accum_string_string(int string_probe) {
  char *string_start = STRING_BUF(string_probe);
  char *string_end = string_start + STRING_LEN(string_probe);
  while (string_start < string_end) {
    accum_string_char(*string_start);
    string_start += 1;
  }
}

// Similar to accum_string_string, but writes an integer to the string pool
// Note that this function only supports small integers, represented as positive number.
void accum_string_integer(int n) {
  if (n < 0) {
    accum_string_char('-');
    accum_string_integer(-n);
  } else {
    if (n > 9) accum_string_integer(n / 10);
    accum_string_char('0' + n % 10);
  }
}

int probe;
int probe_start;
int c1;
int c2;
int end_ident_i;

// Like end_ident, but for strings instead of identifiers
// We want to deduplicate strings to reuse memory if possible.
#define end_string end_ident

int end_ident() {
  string_pool[string_pool_alloc] = 0; // terminate string
  string_pool_alloc += 1; // account for terminator

  probe = heap[hash];

  while (probe != 0) {
    probe_start = heap[probe+1];
    end_ident_i = 0;
    c1 = string_pool[string_start+end_ident_i];
    c2 = string_pool[probe_start+end_ident_i];
    while (c1 == c2) {
      if (c1 == 0) {
        string_pool_alloc = string_start; // undo string allocation
        return probe;
      }
      end_ident_i += 1;
      c1 = string_pool[string_start+end_ident_i];
      c2 = string_pool[probe_start+end_ident_i];
    }
    hash = probe; // remember previous ident
    probe = heap[probe];
  }

  // a new ident has been found

  probe = alloc_obj(5);

  heap[hash] = probe; // add new ident at end of chain

  heap[probe] = 0; // no next ident
  heap[probe+1] = string_start;
  heap[probe+2] = IDENTIFIER;
  heap[probe+3] = 0; // Token tag
  heap[probe+4] = string_pool_alloc - string_start - 1; // string length (excluding terminator)

  return probe;
}

int probe_string(int probe) {
  return heap[probe+1]; // return the start of the string
}

void expect_tok_(int expected_tok, char* file, int line);

function expect_tok(expected_tok) {
  expect_tok_(expected_tok, __FILE__, __LINE__);
}

void get_tok();
void get_ident();

#define IFDEF_DEPTH_MAX 20
bool if_macro_stack[IFDEF_DEPTH_MAX]; // Stack of if macro states
bool if_macro_stack_ix = 0;
bool if_macro_mask = true;      // Indicates if the current if/elif block is being executed
bool if_macro_executed = false; // If any of the previous if/elif conditions were true

// get_tok parameters:
// Whether to expand macros or not.
// Useful to parse macro definitions containing other macros without expanding them.
bool expand_macro = true;
// Don't expand macro arguments. Used for stringification and token pasting.
bool expand_macro_arg = true;
// Don't produce newline tokens. Used when reading the tokens of a macro definition.
bool skip_newlines = true;

#define MACRO_RECURSION_MAX 180 // Supports up to 60 (180 / 3) nested macro expansions.
int macro_stack[MACRO_RECURSION_MAX];
int macro_stack_ix = 0;

int macro_tok_lst = 0;  // Current list of tokens to replay for the macro being expanded
int macro_args = 0;     // Current list of arguments for the macro being expanded
int macro_ident = 0;    // The identifier of the macro being expanded (if any)
int macro_args_count;   // Number of arguments for the current macro being expanded
bool paste_last_token = false; // Whether the last token was a ## or not

bool prev_macro_mask() {
  return if_macro_stack_ix == 0 || if_macro_stack[if_macro_stack_ix - 2];
}

void push_if_macro_mask(bool new_mask) {
  if (if_macro_stack_ix >= IFDEF_DEPTH_MAX) {
    fatal_error("Too many nested #ifdef/#ifndef directives. Maximum supported is 20.");
  }
  // Save current mask on the stack because it's about to be overwritten
  if_macro_stack[if_macro_stack_ix] = if_macro_mask;
  if_macro_stack[if_macro_stack_ix + 1] = if_macro_executed;
  if_macro_stack_ix += 2;

  // If the current block is masked off, then the new mask is the logical AND of the current mask and the new mask
  new_mask = if_macro_mask & new_mask;

  // Then set the new mask value and reset the executed flag
  if_macro_mask = if_macro_executed = new_mask;
}

void pop_if_macro_mask() {
  if (if_macro_stack_ix == 0) {
    fatal_error("Unbalanced #ifdef/#ifndef/#else/#endif directives.");
  }
  if_macro_stack_ix -= 2;
  if_macro_mask = if_macro_stack[if_macro_stack_ix];
  if_macro_executed = if_macro_stack[if_macro_stack_ix + 1];
}

void get_ch() {
  ch = fgetc(fp);

  if (ch == EOF) {
    // If it's not the last file on the stack, EOF means that we need to switch to the next file
    if (include_stack->next != 0) {
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
      ch = '\n';
    }
  }
  else if (ch == '\n') {
    line_number += 1;
    column_number = 0;
  } else {
    column_number += 1;
  }
}

// TODO: It would be nice to not have to duplicate this code
int strlen(char *str) {
  int i = 0;
  while (str[i] != '\0') i += 1;
  return i;
}

void memcpy(char *dest, char *src, int n) {
  int i;
  for (i = 0; i < n; i += 1) {
    dest[i] = src[i];
  }
}

char *substr(char *str, int start, int end) {
  int len = end - start;
  char *temp = malloc(len + 1);
  memcpy(temp, str + start, len);
  temp[len] = '\0';
  return temp;
}

char *str_concat(char *s1, char *s2) {
  int s1_len = strlen(s1);
  int s2_len = strlen(s2);
  char *temp = malloc(s1_len + s2_len + 1);
  memcpy(temp, s1, s1_len);
  memcpy(temp + s1_len, s2, s2_len);
  temp[s1_len + s2_len] = '\0';
  return temp;
}

// Removes the last component of the path, keeping the trailing slash if any.
// For example, /a/b/c.txt -> /a/b/
// If the path does not contain a slash, it returns "".
char *file_parent_directory(char *path) {
  int i = 0;
  int last_slash = -1;
  while (path[i] != '\0') {
    if (path[i] == '/') last_slash = i;

    i += 1;
  }
  if (last_slash == -1) {
    path = malloc(1);
    path[0] = '\0';
  } else {
    path = substr(path, 0, last_slash + 1);
  }
  return path;
}

FILE *fopen_source_file(char *file_name, char *relative_to) {
  FILE *fp;
  fp_filepath = file_name;
  if (relative_to) {
    fp_filepath = str_concat(relative_to, fp_filepath);
  }
  fp = fopen(fp_filepath, "r");
  if (fp == 0) {
    putstr(fp_filepath); putchar('\n');
    fatal_error("Could not open file");
  }
  return fp;
}

void include_file(char *file_name, char *relative_to) {
  fp = fopen_source_file(file_name, relative_to);
  include_stack2 = malloc(sizeof(struct IncludeStack));
  include_stack2->next = include_stack;
  include_stack2->fp = fp;
  include_stack2->dirname = file_parent_directory(fp_filepath);
  include_stack2->filepath = fp_filepath;
  include_stack2->line_number = 1;
  include_stack2->column_number = 0;
  // Save the current file position so we can return to it after the included file is done
  if (include_stack != 0) {
    include_stack->line_number = line_number;
    include_stack->column_number = column_number;
  }
  line_number = 1;
  column_number = 1;
  include_stack = include_stack2;
}

function DIGIT_BYTE(val) {
  return (-val % 256);
}

function INIT_ACCUM_DIGIT() {
  return 0;
}

int accum_digit(int base) {
  int digit = 99;
  if ('0' <= ch && ch <= '9') {
    digit = ch - '0';
  } else if ('A' <= ch && ch <= 'Z') {
    digit = ch - 'A' + 10;
  } else if ('a' <= ch && ch <= 'z') {
    digit = ch - 'a' + 10;
  }
  if (digit >= base) {
    return 0; // character is not a digit in that base
  } else {
    // TODO: Put overflow check back
    // if ((val < limit) || ((val == limit) && (digit > limit * base - MININT))) {
    //   fatal_error("literal integer overflow");
    // }

    val = val * base - digit;
    get_ch();
    return 1;
  }
}

void get_string_char() {

  val = ch;
  get_ch();

  if (val == '\\') {
    if ('0' <= ch && ch <= '7') {
      // Parse octal character, up to 3 digits.
      // Note that \1111 is parsed as '\111' followed by '1'
      // See https://en.wikipedia.org/wiki/Escape_sequences_in_C#Notes
      val = INIT_ACCUM_DIGIT();
      accum_digit(8);
      accum_digit(8);
      accum_digit(8);
      val = DIGIT_BYTE(val); // keep low 8 bits, without overflowing
    } else if (ch == 'x' || ch == 'X') {
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
      if (ch == 'a') {
        val = 7;
      } else if (ch == 'b') {
        val = 8;
      } else if (ch == 'f') {
        val = 12;
      } else if (ch == 'n') {
        val = 10;
      } else if (ch == 'r') {
        val = 13;
      } else if (ch == 't') {
        val = 9;
      } else if (ch == 'v') {
        val = 11;
      } else if (ch == '\\' || ch == '\'' || ch == '\"') {
        val = ch;
      } else {
        syntax_error("unimplemented string character escape");
      }
      get_ch();
    }
  }
}

void accum_string_until(char end) {
  while (ch != end && ch != EOF) {
    get_string_char();
    tok = ch;
    ch = val;
    accum_string();
    ch = tok;
  }

  if (ch != end) {
    syntax_error("unterminated string literal");
  }

  get_ch();
}

// We add the preprocessor keywords to the ident table so they can be easily
// recognized by the preprocessor. Because these are not C keywords, their kind
// is still IDENTIFIER so the parser (which runs after the preprocessor) can
// treat them as such.
int IFDEF_ID;
int IFNDEF_ID;
int ELIF_ID;
int ENDIF_ID;
int DEFINE_ID;
int UNDEF_ID;
int INCLUDE_ID;
int DEFINED_ID;
int WARNING_ID;
int ERROR_ID;
int INCLUDE_SHELL_ID;

int NOT_SUPPORTED_ID;

// We want to recognize certain identifers without having to do expensive string comparisons
int ARGV__ID;
int ARGV_ID;
int IFS_ID;
int MAIN_ID;

int PUTCHAR_ID;
int GETCHAR_ID;
int EXIT_ID;
int MALLOC_ID;
int FREE_ID;
int PRINTF_ID;
int FOPEN_ID;
int FCLOSE_ID;
int FGETC_ID;
int PUTSTR_ID;
int PUTS_ID;
int READ_ID;
int WRITE_ID;
int OPEN_ID;
int CLOSE_ID;

// Macros that are defined by the preprocessor
int FILE__ID;
int LINE__ID;

// When we parse a macro, we generally want the tokens as they are, without expanding them.
void get_tok_macro() {
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
void get_tok_macro_expand() {
  bool prev_expand_macro = expand_macro;
  bool prev_macro_mask = if_macro_mask;

  expand_macro = false;
  if_macro_mask = true;
  get_tok();
  expand_macro = prev_expand_macro;
  if_macro_mask = prev_macro_mask;
}

int lookup_macro_token(int args, int tok, int val) {
  int ix = 0;

  if (tok < IDENTIFIER) return cons(tok, val); // Not an identifier

  while (args != 0) {
    if (car(args) == val) break; // Found!
    args = cdr(args);
    ix += 1;
  }

  if (args == 0) { // Identifier is not a macro argument
    return cons(tok, val);
  } else {
    return cons(MACRO_ARG, ix);
  }
}

int read_macro_tokens(int args) {
  int toks = 0; // List of token to replay
  int tail;

  // Accumulate tokens so they can be replayed when the macro is used
  if (tok != '\n' && tok != EOF) {
    // Append the token/value pair to the replay list
    toks = cons(lookup_macro_token(args, tok, val), 0);
    tail = toks;
    get_tok_macro();
    while (tok != '\n' && tok != EOF) {
      set_cdr(tail, cons(lookup_macro_token(args, tok, val), 0));
      tail = cdr(tail); // Advance tail
      get_tok_macro();
    }

    // Check that there are no leading or trailing ##
    if (car(car(toks)) == HASH_HASH || car(car(tail)) == HASH_HASH) {
      syntax_error("'##' cannot appear at either end of a macro expansion");
    }
  }

  return toks;
}

// A few things that are different from the standard:
// - We allow sequence of commas in the argument list
// - Function-like macros with 0 arguments can be called either without parenthesis or with ().
// - No support for variadic macros. Tcc only uses them in tests so it should be ok.
void handle_define() {
  int macro;    // The identifier that is being defined as a macro
  int args = 0; // List of arguments for a function-like macro
  int args_count = -1; // Number of arguments for a function-like macro. -1 means it's an object-like macro

  if (tok != IDENTIFIER && tok != MACRO && (tok < AUTO_KW || tok > WHILE_KW)) {
    putstr("tok="); putint(tok); putchar('\n');
    syntax_error("#define directive can only be followed by a identifier");
  }

  heap[val + 2] = MACRO; // Mark the identifier as a macro
  macro = val;
  if (ch == '(') { // Function-like macro
    args_count = 0;
    get_tok_macro(); // Skip macro name
    get_tok_macro(); // Skip '('
    while (tok != '\n' && tok != EOF) {
      if (tok == ',') {
        // Allow sequence of commas, this is more lenient than the standard
        get_tok_macro();
        continue;
      } else if (tok == ')') {
        get_tok_macro();
        break;
      }
      get_tok_macro();
      // Accumulate parameters in reverse order. That's ok because the arguments
      // to the macro will also be in reverse order.
      args = cons(val, args);
      args_count += 1;
    }
  } else {
    get_tok_macro(); // Skip macro name
  }

  // Accumulate tokens so they can be replayed when the macro is used
  heap[macro + 3] = cons(read_macro_tokens(args), args_count);

}

int eval_constant(ast expr, bool if_macro) {
  int op = get_op(expr);
  int op1;
  int op2;
  ast child0, child1;

  if (get_nb_children(expr) >= 1) child0 = get_child(expr, 0);
  if (get_nb_children(expr) >= 2) child1 = get_child(expr, 1);

  switch (op) {
    case PARENS:      return eval_constant(child0, if_macro);
    case INTEGER:
    case INTEGER_L:
    case INTEGER_LL:
    case INTEGER_U:
    case INTEGER_UL:
    case INTEGER_ULL:
      return -get_val(expr);
    case CHARACTER:   return get_val_(CHARACTER, expr);
    case '~':         return ~eval_constant(child0, if_macro);
    case '!':         return !eval_constant(child0, if_macro);
    case '-':
    case '+':
      op1 = eval_constant(child0, if_macro);
      if (get_nb_children(expr) == 1) {
        return op == '-' ? -op1 : op1;
      } else {
        op2 = eval_constant(child1, if_macro);
        return op == '-' ? op1 - op2 : op1 + op2;
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
        case '*':     return op1 * op2;
        case '/':     return op1 / op2;
        case '%':     return op1 % op2;
        case '&':     return op1 & op2;
        case '|':     return op1 | op2;
        case '^':     return op1 ^ op2;
        case LSHIFT:  return op1 << op2;
        case RSHIFT:  return op1 >> op2;
        case EQ_EQ:   return op1 == op2;
        case EXCL_EQ: return op1 != op2;
        case LT_EQ:   return op1 <= op2;
        case GT_EQ:   return op1 >= op2;
        case '<':     return op1 < op2;
        case '>':     return op1 > op2;
      }
      return 0; // Should never reach here

    case AMP_AMP:
      op1 = eval_constant(child0, if_macro);
      if (!op1) return 0;
      else return eval_constant(child1, if_macro);

    case BAR_BAR:
      op1 = eval_constant(child0, if_macro);
      if (op1) return 1;
      else return eval_constant(child1, if_macro);

    case '(': // defined operators are represented as fun calls
      if (if_macro && get_val_(IDENTIFIER, child0) == DEFINED_ID) {
        return child1 == MACRO;
      } else {
        fatal_error("unknown function call in constant expressions");
        return 0;
      }

    case IDENTIFIER:
      if (if_macro) {
        // Undefined identifiers are 0
        // At this point, macros have already been expanded so we can't have a macro identifier
        return 0;
      } else {
        // TODO: Enums when outside of if_macro
        fatal_error("identifiers are not allowed in constant expression");
        return 0;
      }

    default:
      putstr("op="); putint(op); putchar('\n');
      fatal_error("unsupported operator in constant expression");
      return 0;
  }
}

ast parse_assignment_expression();

int evaluate_if_condition() {
  bool prev_skip_newlines = skip_newlines;
  int previous_mask = if_macro_mask;
  ast expr;
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

void handle_include() {
  if (tok == STRING) {
    include_file(STRING_BUF(val), include_stack->dirname);
    get_tok_macro(); // Skip the string
  } else if (tok == '<') {
    accum_string_until('>');
    val = end_string();
    // #include <file> directives only take effect if the search path is provided
    // TODO: Issue a warning to stderr when skipping the directive
    if (include_search_path != 0) {
      include_file(STRING_BUF(val), include_search_path);
    }
    get_tok_macro(); // Skip the string
  } else {
    putstr("tok="); putint(tok); putchar('\n');
    syntax_error("expected string to #include directive");
  }
}

void handle_preprocessor_directive() {
  int temp;
  get_tok_macro(); // Get the # token
  get_tok_macro(); // Get the directive

  if (tok == IDENTIFIER && (val == IFDEF_ID || val == IFNDEF_ID)) {
    temp = val;
    get_tok_macro(); // Get the macro name
      push_if_macro_mask(temp == IFDEF_ID ? tok == MACRO : tok != MACRO);
    get_tok_macro(); // Skip the macro name
  } else if (tok == IF_KW) {
    temp = evaluate_if_condition() != 0;
    push_if_macro_mask(temp);
  } else if (tok == IDENTIFIER && val == ELIF_ID) {
    temp = evaluate_if_condition() != 0;
    if (prev_macro_mask() && !if_macro_executed) {
      if_macro_executed |= temp;
      if_macro_mask = temp;
    } else {
      if_macro_mask = false;
    }
  } else if (tok == ELSE_KW) {
    if (prev_macro_mask()) { // If the parent block mask is true
      if_macro_mask = !if_macro_executed;
      if_macro_executed = true;
    } else {
      if_macro_mask = false;
    }
    get_tok_macro(); // Skip the else keyword
  } else if (tok == IDENTIFIER && val == ENDIF_ID) {
      pop_if_macro_mask();
    get_tok_macro(); // Skip the else keyword
  } else if (if_macro_mask) {
    if (tok == IDENTIFIER && val == INCLUDE_ID) {
      get_tok_macro(); // Get the STRING token
      handle_include();
    }
    else if (tok == IDENTIFIER && val == UNDEF_ID) {
      get_tok_macro(); // Get the macro name
      if (tok == IDENTIFIER || tok == MACRO) {
        // TODO: Doesn't play nice with typedefs, because they are not marked as macros
        heap[val + 2] = IDENTIFIER; // Unmark the macro identifier
        get_tok_macro(); // Skip the macro name
      } else {
        putstr("tok="); putint(tok); putchar('\n');
        syntax_error("#undef directive can only be followed by a identifier");
      }
    } else if (tok == IDENTIFIER && val == DEFINE_ID) {
      get_tok_macro(); // Get the macro name
      handle_define();
    } else if (tok == IDENTIFIER && (val == WARNING_ID || val == ERROR_ID)) {
      temp = val;
      putstr(temp == WARNING_ID ? "warning:" : "error:");
      // Print the rest of the line, it does not support \ at the end of the line but that's ok
      while (ch != '\n' && ch != EOF) {
        putchar(ch); get_ch();
      }
      putchar('\n');
      tok = '\n';
      if (temp == ERROR_ID) exit(1);
    } else {
      putstr("tok="); putint(tok); putstr(": "); putstr(STRING_BUF(val)); putchar('\n');
      syntax_error("unsupported preprocessor directive");
    }
  } else {
    // Skip the rest of the directive
    while (tok != '\n' && tok != EOF) get_tok_macro();
  }

  if (tok != '\n' && tok != EOF) {
    putstr("tok="); putint(tok); putchar('\n');
    putstr("directive="); putint(tok); putchar('\n');
    if (tok == IDENTIFIER || tok == MACRO) {
      putstr("string = ");
      putstr(STRING_BUF(val));
      putchar('\n');
    }
    syntax_error("preprocessor expected end of line");
  }

  // Because handle_preprocessor_directive is called from get_tok, and it loops
  // after the call to handle_preprocessor_directive, we don't need to call
  // get_tok before returning.
}

void get_ident() {

  begin_string();

  while (('A' <= ch && ch <= 'Z') ||
         ('a' <= ch && ch <= 'z') ||
         ('0' <= ch && ch <= '9') ||
         (ch == '_')) {
    accum_string();
    get_ch();
  }

  val = end_ident();
  tok = heap[val+2];
}

int intern_str(char* name) {
  int i = 0;
  int prev_ch = ch; // The character may be important to the calling function, saving it

  begin_string();

  while (name[i] != 0) {
    ch = name[i];
    accum_string();
    i += 1;
  }

  i = end_string();

  ch = prev_ch;

  return i;
}

int init_ident(int tok, char *name) {
  int i = intern_str(name);
  heap[i+2] = tok;
  return i;
}

void init_ident_table() {

  int i = 0;

  while (i < HASH_PRIME) {
    heap[i] = 0;
    i += 1;
  }

  init_ident(AUTO_KW,     "auto");
  init_ident(BREAK_KW,    "break");
  init_ident(CASE_KW,     "case");
  init_ident(CHAR_KW,     "char");
  init_ident(CONST_KW,    "const");
  init_ident(CONTINUE_KW, "continue");
  init_ident(DEFAULT_KW,  "default");
  init_ident(DO_KW,       "do");
  init_ident(DOUBLE_KW,   "double");
  init_ident(ELSE_KW,     "else");
  init_ident(ENUM_KW,     "enum");
  init_ident(EXTERN_KW,   "extern");
  init_ident(FLOAT_KW,    "float");
  init_ident(FOR_KW,      "for");
  init_ident(GOTO_KW,     "goto");
  init_ident(IF_KW,       "if");
  init_ident(INLINE_KW,   "inline");
  init_ident(INT_KW,      "int");
  init_ident(LONG_KW,     "long");
  init_ident(REGISTER_KW, "register");
  init_ident(RETURN_KW,   "return");
  init_ident(SHORT_KW,    "short");
  init_ident(SIGNED_KW,   "signed");
  init_ident(SIZEOF_KW,   "sizeof");
  init_ident(STATIC_KW,   "static");
  init_ident(STRUCT_KW,   "struct");
  init_ident(SWITCH_KW,   "switch");
  init_ident(TYPEDEF_KW,  "typedef");
  init_ident(UNION_KW,    "union");
  init_ident(UNSIGNED_KW, "unsigned");
  init_ident(VOID_KW,     "void");
  init_ident(VOLATILE_KW, "volatile");
  init_ident(WHILE_KW,    "while");

  // Preprocessor keywords. These are not tagged as keyword since they can be
  // used as identifiers after the preprocessor stage.
  IFDEF_ID   = init_ident(IDENTIFIER, "ifdef");
  IFNDEF_ID  = init_ident(IDENTIFIER, "ifndef");
  ELIF_ID    = init_ident(IDENTIFIER, "elif");
  ENDIF_ID   = init_ident(IDENTIFIER, "endif");
  DEFINE_ID  = init_ident(IDENTIFIER, "define");
  WARNING_ID = init_ident(IDENTIFIER, "warning");
  ERROR_ID   = init_ident(IDENTIFIER, "error");
  UNDEF_ID   = init_ident(IDENTIFIER, "undef");
  INCLUDE_ID = init_ident(IDENTIFIER, "include");
  DEFINED_ID = init_ident(IDENTIFIER, "defined");
  INCLUDE_SHELL_ID = init_ident(IDENTIFIER, "include_shell");

  ARGV_ID = init_ident(IDENTIFIER, "argv");
  ARGV__ID = init_ident(IDENTIFIER, "argv_");
  IFS_ID  = init_ident(IDENTIFIER, "IFS");
  MAIN_ID = init_ident(IDENTIFIER, "main");

  PUTCHAR_ID = init_ident(IDENTIFIER, "putchar");
  GETCHAR_ID = init_ident(IDENTIFIER, "getchar");
  EXIT_ID    = init_ident(IDENTIFIER, "exit");
  MALLOC_ID  = init_ident(IDENTIFIER, "malloc");
  FREE_ID    = init_ident(IDENTIFIER, "free");
  PRINTF_ID  = init_ident(IDENTIFIER, "printf");
  FOPEN_ID   = init_ident(IDENTIFIER, "fopen");
  FCLOSE_ID  = init_ident(IDENTIFIER, "fclose");
  FGETC_ID   = init_ident(IDENTIFIER, "fgetc");
  PUTSTR_ID  = init_ident(IDENTIFIER, "putstr");
  PUTS_ID    = init_ident(IDENTIFIER, "puts");
  READ_ID    = init_ident(IDENTIFIER, "read");
  WRITE_ID   = init_ident(IDENTIFIER, "write");
  OPEN_ID    = init_ident(IDENTIFIER, "open");
  CLOSE_ID   = init_ident(IDENTIFIER, "close");

  // Stringizing is recognized by the macro expander, but it returns a hardcoded
  // string instead of the actual value. This may be enough to compile TCC.
  NOT_SUPPORTED_ID = init_ident(IDENTIFIER, "NOT_SUPPORTED");
}

int init_builtin_string_macro(char *macro_str, char* value) {
  int macro_id = init_ident(MACRO, macro_str);
  // Macro object shape: ([(tok, val)], arity). -1 arity means it's an object-like macro
  heap[macro_id + 3] = cons(cons(cons(STRING, intern_str(value)), 0), -1);
  return macro_id;
}

int init_builtin_int_macro(char *macro_str, int value) {
  int macro_id = init_ident(MACRO, macro_str);
  heap[macro_id + 3] = cons(cons(cons(INTEGER, -value), 0), -1);
  return macro_id;
}

int init_builtin_empty_macro(char *macro_str) {
  int macro_id = init_ident(MACRO, macro_str);
  heap[macro_id + 3] = cons(0, -1); // -1 means it's an object-like macro, 0 means no tokens
  return macro_id;
}

void init_pnut_macros() {
  init_builtin_int_macro("PNUT_CC", 1);

  init_builtin_string_macro("__DATE__", "Jan  1 1970");
  init_builtin_string_macro("__TIME__", "00:00:00");
  init_builtin_string_macro("__TIMESTAMP__", "Jan  1 1970 00:00:00");
  FILE__ID = init_builtin_string_macro("__FILE__", "<unknown>");
  LINE__ID = init_builtin_int_macro("__LINE__", 0);

#if defined(sh)
  init_builtin_int_macro("PNUT_SH", 1);
#elif defined(target_i386_linux)
  init_builtin_int_macro("PNUT_EXE", 1);
  init_builtin_int_macro("PNUT_EXE_32", 1);
  init_builtin_int_macro("PNUT_I386", 1);
  init_builtin_int_macro("PNUT_I386_LINUX", 1);
  init_builtin_int_macro("__linux__", 1);
  init_builtin_int_macro("__i386__", 1);
#elif defined (target_x86_64_linux)
  init_builtin_int_macro("PNUT_EXE", 1);
  init_builtin_int_macro("PNUT_EXE_64", 1);
  init_builtin_int_macro("PNUT_X86_64", 1);
  init_builtin_int_macro("PNUT_X86_64_LINUX", 1);
  init_builtin_int_macro("__linux__", 1);
  init_builtin_int_macro("__x86_64__", 1);
#elif defined (target_x86_64_mac)
  init_builtin_int_macro("PNUT_EXE", 1);
  init_builtin_int_macro("PNUT_EXE_64", 1);
  init_builtin_int_macro("PNUT_X86_64", 1);
  init_builtin_int_macro("PNUT_X86_64_MAC", 1);
  init_builtin_int_macro("__x86_64__", 1);
#endif

}

// A macro argument is represented using a list of tokens.
// Macro arguments are split by commas, but commas can also appear in function
// calls and as operators. To distinguish between the two, we need to keep track
// of the parenthesis depth.
int macro_parse_argument() {
  int arg_tokens = 0;
  int parens_depth = 0;
  int tail;

  while ((parens_depth > 0 || (tok != ',' && tok != ')')) && tok != EOF) {
    if (tok == '(') parens_depth += 1; // Enter parenthesis
    if (tok == ')') parens_depth -= 1; // End of parenthesis

    if (arg_tokens == 0) {
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

void check_macro_arity(int macro_args_count, int macro) {
  int expected_argc = cdr(heap[macro + 3]);
  if (macro_args_count != expected_argc) {
    putstr("expected_argc="); putint(expected_argc);
    putstr(" != macro_args_count="); putint(macro_args_count);
    putchar('\n');
    putstr("macro="); putstr(STRING_BUF(macro)); putchar('\n');
    syntax_error("macro argument count mismatch");
  }
}

// Reads the arguments of a macro call, where the arguments are split by commas.
// Note that args are accumulated in reverse order, as the macro arguments refer
// to the tokens in reverse order.
int get_macro_args_toks(int macro) {
  int args = 0;
  int macro_args_count = 0;
  bool prev_is_comma = tok == ',';
  get_tok_macro_expand(); // Skip '('

  while (tok != ')' && tok != EOF) {
    if (tok == ',') {
      get_tok_macro_expand(); // Skip comma
      if (prev_is_comma) { // Push empty arg
        args = cons(0, args);
        macro_args_count += 1;
      }
      prev_is_comma = true;
      continue;
    } else {
      prev_is_comma = false;
    }

    args = cons(macro_parse_argument(), args);
    macro_args_count += 1;
  }

  if (tok != ')') syntax_error("unterminated macro argument list");

  if (prev_is_comma) {
    args = cons(0, args); // Push empty arg
    macro_args_count += 1;
  }

  check_macro_arity(macro_args_count, macro);

  return args;
}

int get_macro_arg(int ix) {
  int arg = macro_args;
  while (ix > 0) {
    if (arg == 0) syntax_error("too few arguments to macro");
    arg = cdr(arg);
    ix -= 1;
  }
  return car(arg);
}

// "Pops" the current macro expansion and restores the previous macro expansion context.
// This is done when the current macro expansion is done.
void return_to_parent_macro() {
  if (macro_stack_ix == 0) fatal_error("return_to_parent_macro: no parent macro");

  macro_stack_ix -= 3;
  macro_tok_lst   = macro_stack[macro_stack_ix];
  macro_args      = macro_stack[macro_stack_ix + 1];
  macro_ident     = macro_stack[macro_stack_ix + 2];
}

// Begins a new macro expansion context, saving the current context onn the macro stack.
// Takes as argument the name of the macro, the tokens to be expanded and the arguments.
void begin_macro_expansion(int ident, int tokens, int args) {
  if (macro_stack_ix + 3 >= MACRO_RECURSION_MAX) {
    fatal_error("Macro recursion depth exceeded.");
  }

  macro_stack[macro_stack_ix]     = macro_tok_lst;
  macro_stack[macro_stack_ix + 1] = macro_args;
  macro_stack[macro_stack_ix + 2] = macro_ident;
  macro_stack_ix += 3;

  macro_ident   = ident;
  macro_tok_lst = tokens;
  macro_args    = args;
}

// Search the macro stack to see if the macro is already expanding.
bool macro_is_already_expanding(int ident) {
  int i = macro_stack_ix;
  if (ident == 0 || macro_ident == 0) return false; // Unnamed macro or no macro is expanding
  if (ident == macro_ident)           return true;  // The same macro is already expanding

  // Traverse the stack to see if the macro is already expanding
  while (i > 0) {
    i -= 3;
    if (macro_stack[i + 2] == ident) return true;
  }
  return false;
}

// Undoes the effect of get_tok by replacing the current token with the previous
// token and saving the current token to be returned by the next call to get_tok.
void undo_token(int prev_tok, int prev_val) {
  begin_macro_expansion(0, cons(cons(tok, val), 0), 0); // Push the current token back
  tok = prev_tok;
  val = prev_val;
}

// Try to expand a macro and returns if the macro was expanded.
// A macro is not expanded if it is already expanding or if it's a function-like
// macro that is not called with parenthesis. In that case, the macro identifier
// is returned as a normal identifier.
// If the wrong number of arguments is passed to a function-like macro, a fatal error is raised.
bool attempt_macro_expansion(int macro) {
  // We must save the tokens because the macro may be redefined while reading the arguments
  int tokens = car(heap[macro + 3]);

  if (macro_is_already_expanding(macro)) { // Self referencing macro
    tok = IDENTIFIER;
    val = macro;
    return false;
  } else if (cdr(heap[macro + 3]) == -1) { // Object-like macro
    // Note: Redefining __{FILE,LINE}__ macros, either with the #define or #line directives is not supported.
    if (macro == FILE__ID) {
      tokens = cons(cons(STRING, intern_str(fp_filepath)), 0);
    }
#ifdef INCLUDE_LINE_NUMBER_ON_ERROR
    else if (macro == LINE__ID) {
      tokens = cons(cons(INTEGER, -line_number), 0);
    }
#endif
    begin_macro_expansion(macro, tokens, 0);
    return true;
  } else { // Function-like macro
    expect_tok(MACRO); // Skip macro identifier
    if (tok == '(') {
      begin_macro_expansion(macro, tokens, get_macro_args_toks(macro));
      return true;
    } else {
      undo_token(IDENTIFIER, macro);
      return false;
    }
  }
}

// https://gcc.gnu.org/onlinedocs/cpp/Stringizing.html
void stringify() {
  int arg;
  expand_macro_arg = false;
  get_tok_macro();
  expand_macro_arg = true;
  if (tok != MACRO_ARG) {
    putstr("tok="); putint(tok); putchar('\n');
    syntax_error("expected macro argument after #");
  }
  arg = get_macro_arg(val);
  tok = STRING;
  // Support the case where the argument is a single identifier/macro/keyword token
  if ((car(car(arg)) == IDENTIFIER || car(car(arg)) == MACRO || (AUTO_KW <= car(car(arg)) && car(car(arg)) <= WHILE_KW)) && cdr(arg) == 0) {
    val = cdr(car(arg)); // Use the identifier probe
  } else {
    val = NOT_SUPPORTED_ID; // Return string "NOT_SUPPORTED"
  }
}

// Concatenates two non-negative integers into a single integer
// Note that this function only supports small integers, represented as positive integers.
int paste_integers(int left_val, int right_val) {
  int result = left_val;
  int right_digits = right_val;
#ifdef SUPPORT_64_BIT_LITERALS
  if (left_val < 0 || right_val < 0) fatal_error("Only small integers can be pasted");
#endif
  while (right_digits > 0) {
    result *= 10;
    right_digits /= 10;
  }
  return result + right_val;
}

// Support token pasting between identifiers and non-negative integers
void paste_tokens(int left_tok, int left_val) {
  int right_tok;
  int right_val;
  expand_macro_arg = false;
  get_tok_macro();
  expand_macro_arg = true;
  // We need to handle the case where the right-hand side is a macro argument that expands to empty
  // In that case, the left-hand side is returned as is.
  if (tok == MACRO_ARG) {
    if (get_macro_arg(val) == 0) {
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
  if (left_tok == IDENTIFIER || left_tok == TYPE || left_tok == MACRO || left_tok <= WHILE_KW) {
    // Something that starts with an identifier can only be an identifier
    begin_string();
    accum_string_string(left_val);

    if (right_tok == IDENTIFIER || right_tok == TYPE || right_tok == MACRO || right_tok <= WHILE_KW) {
      accum_string_string(right_val);
    } else if (right_tok == INTEGER
            || right_tok == INTEGER_L || right_tok == INTEGER_LL || right_tok == INTEGER_U || right_tok == INTEGER_UL || right_tok == INTEGER_ULL
              ) {
      accum_string_integer(-right_val);
    } else {
      putstr("left_tok="); putint(left_tok); putstr(", right_tok="); putint(right_tok); putchar('\n');
      // show identifier/macro string
      putstr("left="); putstr(STRING_BUF(left_val)); putchar('\n');
      syntax_error("cannot paste an identifier with a non-identifier or non-negative integer");
    }

    val = end_ident();
    tok = heap[val+2]; // The kind of the identifier
  } else if (left_tok == INTEGER
          || left_tok == INTEGER_L || left_tok == INTEGER_LL || left_tok == INTEGER_U || left_tok == INTEGER_UL || left_tok == INTEGER_ULL
            ) {
    if (right_tok == INTEGER
    || right_tok == INTEGER_L || right_tok == INTEGER_LL || right_tok == INTEGER_U || right_tok == INTEGER_UL || right_tok == INTEGER_ULL
       ) {
      val = -paste_integers(-left_val, -right_val);
    } else if (right_tok == IDENTIFIER || right_tok == MACRO || right_tok <= WHILE_KW) {
      begin_string();
      accum_string_integer(-left_val);
      accum_string_string(right_val);

      val = end_ident();
      tok = heap[val+2]; // The kind of the identifier
    } else {
      putstr("left_tok="); putint(left_tok); putstr(", right_tok="); putint(right_tok); putchar('\n');
      syntax_error("cannot paste an integer with a non-integer");
    }
  } else {
    putstr("left_tok="); putint(left_tok); putstr(", right_tok="); putint(right_tok); putchar('\n');
    syntax_error("cannot paste a non-identifier or non-integer");
  }
}

void get_tok() {

  int prev_tok_line_number = line_number;
  int prev_tok_column_number = column_number;

  // This outer loop is used to skip over tokens removed by #ifdef/#ifndef/#else
  do {
    while (1) {
      // Check if there are any tokens to replay. Macros are just identifiers that
      // have been marked as macros. In terms of how we get into that state, a
      // macro token is first returned by the get_ident call a few lines below.
      if (macro_tok_lst != 0) {
        tok = car(car(macro_tok_lst));
        val = cdr(car(macro_tok_lst));
        macro_tok_lst = cdr(macro_tok_lst);
        // Tokens that are identifiers and up are tokens whose kind can change
        // between the moment the macro is defined and where it is used.
        // So we reload the kind from the ident table.
        if (tok >= IDENTIFIER) tok = heap[val + 2];

        // Check if the next token is ## for token pasting
        if (macro_tok_lst != 0 && car(car(macro_tok_lst)) == HASH_HASH) {
          if (tok == MACRO || tok == MACRO_ARG) {
            // If the token is a macro or macro arg, it must be expanded before pasting
            macro_tok_lst = cdr(macro_tok_lst); // We consume the ## token
            paste_last_token = true;
          } else {
            // macro_tok_lst is not empty because read_macro_tokens checked for trailing ##
            macro_tok_lst = cdr(macro_tok_lst); // Skip the ##
            paste_tokens(tok, val);
          }
        } else if (macro_tok_lst == 0 && paste_last_token) { // We finished expanding the left-hand side of ##
          if (macro_stack_ix == 0) {
            // If we are not in a macro expansion, we can't paste the last token
            // This should not happen if the macro is well-formed, which is
            // checked by read_macro_tokens.
            syntax_error("## cannot appear at the end of a macro expansion");
          }
          return_to_parent_macro();
          paste_last_token = false; // We are done pasting
          paste_tokens(tok, val);
        }

        if (tok == MACRO) { // Nested macro expansion!
          if (attempt_macro_expansion(val)) {
            continue;
          }
          break;
        } else if (tok == MACRO_ARG && expand_macro_arg) {
          begin_macro_expansion(0, get_macro_arg(val), 0); // Play the tokens of the macro argument
          continue;
        } else if (tok == '#') { // Stringizing!
          stringify();
          break;
        }
        break;
      } else if (macro_stack_ix != 0) {
        return_to_parent_macro();
        continue;
      } else if (ch <= ' ') {

        if (ch == EOF) {
          tok = EOF;
          break;
        }

        // skip whitespace, detecting when it is at start of line.
        // When skip_newlines is false, produces a '\n' token whenever it
        // encounters whitespace containing at least a newline.
        // This condenses multiple newlines into a single '\n' token and serves
        // to end the current preprocessor directive.

        tok = 0; // Reset the token
        while (0 <= ch && ch <= ' ') {
          if (ch == '\n') tok = ch;
          get_ch();
        }

        if (tok == '\n' && !skip_newlines) {
          // If the newline is followed by a #, the preprocessor directive is
          // handled in the next iteration of the loop.
          break;
        }

        // will continue while (1) loop
      }

      // detect '#' at start of line, possibly preceded by whitespace
      else if (tok == '\n' && ch == '#') {
        tok = 0; // Consume the newline so handle_preprocessor_directive's get_tok doesn't re-enter this case
        handle_preprocessor_directive();
        // will continue while (1) loop
      }

      else if (('a' <= ch && ch <= 'z') ||
               ('A' <= ch && ch <= 'Z') ||
               (ch == '_')) {

        get_ident();

        if (tok == MACRO) {
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
      } else if ('0' <= ch && ch <= '9') {

        val = INIT_ACCUM_DIGIT();

        tok = INTEGER;
        if (ch == '0') { // val == 0 <=> ch == '0'
          get_ch();
          if (ch == 'x' || ch == 'X') {
            get_ch();
            if (accum_digit(16)) {
              while (accum_digit(16));
            } else {
              syntax_error("invalid hex integer -- it must have at least one digit");
            }
          } else {
            while (accum_digit(8));
          }
        } else {
          while (accum_digit(10));
        }

        // If this is enabled with PARSE_NUMERIC_LITERAL_WITH_BASE, using a
        // suffix replaces INTEGER_OCT and INTEGER_HEX with base 10 INTEGER.
        if (ch == 'u' || ch == 'U') {
          // Note: allows suffixes with mixed case, such as lL for simplicity
          tok = INTEGER_U;
          get_ch();
          if (ch == 'l' || ch == 'L') {
            tok = INTEGER_UL;
            get_ch();
            if (ch == 'l' || ch == 'L') {
              tok = INTEGER_ULL;
              get_ch();
            }
          }
        } else if (ch == 'l' || ch == 'L') {
          tok = INTEGER_L;
          get_ch();
          if (ch == 'l' || ch == 'L') {
            tok = INTEGER_LL;
            get_ch();
          }
          if (ch == 'u' || ch == 'U') {
            tok = tok == INTEGER_LL ? INTEGER_ULL : INTEGER_UL;
            get_ch();
          }
        } else if (ch == 'f' || ch == '.') {
          get_ch();
          tok = INTEGER;
          while (accum_digit(10)); // Skip the fractional part
          val = 0; // Force the value to be 0 for now. TODO: Convert to float
        }

        break;

      } else if (ch == '\'') {

        get_ch();
        get_string_char();

        if (ch != '\'') {
          syntax_error("unterminated character literal");
        }

        get_ch();

        tok = CHARACTER;

        break;

      } else if (ch == '\"') {

        get_ch();

        begin_string();
        accum_string_until('\"');

        val = end_string();
        tok = STRING;

        break;

      } else {

        tok = ch; // fallback for single char tokens

        if (ch == '/') {

          get_ch();
          if (ch == '*') {
            get_ch();
            tok = ch; // remember previous char, except first one
            while ((tok != '*' || ch != '/') && ch != EOF) {
              tok = ch;
              get_ch();
            }
            if (ch == EOF) {
              syntax_error("unterminated comment");
            }
            get_ch();
            // will continue while (1) loop
          } else if (ch == '/') {
            while (ch != '\n' && ch != EOF) {
              get_ch();
            }
            // will continue while (1) loop
          } else {
            if (ch == '=') {
              get_ch();
              tok = SLASH_EQ;
            }
            break;
          }

        } else if (ch == '&') {

          get_ch();
          if (ch == '&') {
            get_ch();
            tok = AMP_AMP;
          } else if (ch == '=') {
            get_ch();
            tok = AMP_EQ;
          }

          break;

        } else if (ch == '|') {

          get_ch();
          if (ch == '|') {
            get_ch();
            tok = BAR_BAR;
          } else if (ch == '=') {
            get_ch();
            tok = BAR_EQ;
          }

          break;

        } else if (ch == '<') {

          get_ch();
          if (ch == '=') {
            get_ch();
            tok = LT_EQ;
          } else if (ch == '<') {
            get_ch();
            if (ch == '=') {
              get_ch();
              tok = LSHIFT_EQ;
            } else {
              tok = LSHIFT;
            }
          }

          break;

        } else if (ch == '>') {

          get_ch();
          if (ch == '=') {
            get_ch();
            tok = GT_EQ;
          } else if (ch == '>') {
            get_ch();
            if (ch == '=') {
              get_ch();
              tok = RSHIFT_EQ;
            } else {
              tok = RSHIFT;
            }
          }

          break;

        } else if (ch == '=') {

          get_ch();
          if (ch == '=') {
            get_ch();
            tok = EQ_EQ;
          }

          break;

        } else if (ch == '!') {

          get_ch();
          if (ch == '=') {
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

        } else if (ch == '-') {

          get_ch();
          if (ch == '=') {
            get_ch();
            tok = MINUS_EQ;
          } else if (ch == '>') {
            get_ch();
            tok = ARROW;
          } else if (ch == '-') {
            get_ch();
            tok = MINUS_MINUS;
          }

          break;

        } else if (ch == '*') {

          get_ch();
          if (ch == '=') {
            get_ch();
            tok = STAR_EQ;
          }

          break;

        } else if (ch == '%') {

          get_ch();
          if (ch == '=') {
            get_ch();
            tok = PERCENT_EQ;
          }

          break;

        } else if (ch == '^') {

          get_ch();
          if (ch == '=') {
            get_ch();
            tok = CARET_EQ;
          }

          break;

        } else if (ch == '#') {

          get_ch();
          if (ch == '#') {
            get_ch();
            tok = HASH_HASH;
          }

          break;

        } else if (ch == '.') {
          get_ch();
          if (ch == '.') {
            get_ch();
            if (ch == '.') {
              get_ch();
              tok = ELLIPSIS;
            } else {
              syntax_error("invalid token");
            }
          } else {
            tok = '.';
          }
          break;
        } else if (ch == '~' || ch == '.' || ch == '?' || ch == ',' || ch == ':' || ch == ';' || ch == '(' || ch == ')' || ch == '[' || ch == ']' || ch == '{' || ch == '}') {

          tok = ch;

          get_ch();

          break;

        } else if (ch == '\\') {
          get_ch();

          if (ch == '\n') { // Continues with next token
            get_ch();
          } else {
            putstr("ch="); putint(ch); putchar('\n');
            syntax_error("unexpected character after backslash");
          }
        } else {
          putstr("ch="); putint(ch); putchar('\n');
          syntax_error("invalid token");
        }
      }
    }
  } while (!if_macro_mask);

  last_tok_line_number = prev_tok_line_number;
  last_tok_column_number = prev_tok_column_number;
}

#define parse_error(msg, token) parse_error_internal(msg, token, __FILE__, __LINE__)

void parse_error_internal(char * msg, int token, char * file, int line) {
  putstr(msg);

  putstr("Note, error emitted from ");
  putstr(file);
  putstr(" line ");
  putint(line);
  putstr("\n");

  exit(1);
}

void expect_tok_(int expected_tok, char* file, int line) {
  if (tok != expected_tok) {
    putstr("expected tok="); putint(expected_tok);
    putstr("\ncurrent tok="); putint(tok); putchar('\n');
    parse_error_internal("unexpected token", tok, file, line);
  }
  get_tok();
}

ast parse_comma_expression();
ast parse_call_params();
ast parse_cast_expression();
ast parse_compound_statement();
ast parse_conditional_expression();
ast parse_enum();
ast parse_struct_or_union(int struct_or_union_tok);
ast parse_declarator(bool abstract_decl, ast parent_type);
ast parse_declaration_specifiers(bool allow_typedef);
ast parse_initializer_list();
ast parse_initializer();

// The storage class specifier and type qualifier tokens are all between 300 (AUTO_KW) and 326 (VOLATILE_KW) so we store them as bits in an int.
#define MK_TYPE_SPECIFIER(tok) (1 << (tok - AUTO_KW))
#define TEST_TYPE_SPECIFIER(specifier, tok) ((specifier) & (1 << (tok - AUTO_KW)))

ast get_type_specifier(ast type_or_decl) {
  while (1) {
    switch (get_op(type_or_decl)) {
      case DECL:
        type_or_decl = get_child_(DECL, type_or_decl, 1);
        break;
      case '[':
        type_or_decl = get_child_('[', type_or_decl, 0);
        break;
      case '*':
        type_or_decl = get_child_('*', type_or_decl, 0);
        break;
      default:
        return type_or_decl;
    }
  }
}

ast pointer_type(ast parent_type, bool is_const) {
  return new_ast2('*', is_const ? MK_TYPE_SPECIFIER(CONST_KW) : 0, parent_type);
}

ast function_type(ast parent_type, ast params) {
  return new_ast3('(', parent_type, params, false);
}

ast function_type1(ast parent_type, ast param1) {
  return new_ast3('(', parent_type, cons(param1, 0), 0);
}

ast function_type2(ast parent_type, ast param1, ast param2) {
  return new_ast3('(', parent_type, cons(param1, cons(param2, 0)), 0);
}

ast function_type3(ast parent_type, ast param1, ast param2, ast param3) {
  return new_ast3('(', parent_type, cons(param1, cons(param2, cons(param3, 0))), 0);
}

ast make_variadic_func(ast func_type) {
  set_child(func_type, 2, true); // Set the variadic flag
  return func_type;
}

// Type and declaration parser
bool is_type_starter(int tok) {
  switch (tok) {
    case INT_KW: case CHAR_KW: case SHORT_KW: case LONG_KW: // Numeric types
    case VOID_KW: case FLOAT_KW: case DOUBLE_KW:            // Void and floating point types
    case SIGNED_KW: case UNSIGNED_KW:                       // Signedness
    case TYPE:                                              // User defined types
    case CONST_KW: case VOLATILE_KW:                        // Type attributes
    case ENUM_KW: case STRUCT_KW: case UNION_KW:            // Enum, struct, union
    // Storage class specifiers are not always valid type starters in all
    // contexts, but we allow them here
    case TYPEDEF_KW: case STATIC_KW: case AUTO_KW: case REGISTER_KW: case EXTERN_KW:
    case INLINE_KW:
      return true;
    default:
      return false;
  }
}

ast parse_enum() {
  ast name;
  ast ident;
  ast result = 0;
  ast tail;
  ast value = 0;
  int next_value = 0;
  int last_literal_type = INTEGER; // Default to decimal integer for enum values

  expect_tok(ENUM_KW);

  if (tok == IDENTIFIER || tok == TYPE) {
    // When the enum keyword is used with an identifier that's typedefed, the typedef is ignored.
    name = new_ast0(IDENTIFIER, val);
    get_tok();
  } else {
    name = 0;
  }

  // Note: The parser doesn't distinguish between a reference to an enum type and a declaration.
  // If child#2 is 0, it's either a reference to a type or a forward declaration.
  if (tok == '{') {
    get_tok();

    while (tok != '}') {
      if (tok != IDENTIFIER) {
        parse_error("identifier expected", tok);
      }
      ident = new_ast0(IDENTIFIER, val);
      get_tok();

      if (tok == '=') {
        get_tok();
        value = parse_assignment_expression();
        if (value == 0) parse_error("Enum value must be a constant expression", tok);

#ifdef PARSE_NUMERIC_LITERAL_WITH_BASE
        // Preserve the type of integer literals (dec/hex/oct) by only creating
        // a new node if the value is not already a literal. We use the last
        // literal type to determine which type to use when creating a new node.
        value = non_parenthesized_operand(value);
        if (get_op(value) != INTEGER && get_op(value) != INTEGER_HEX && get_op(value) != INTEGER_OCT) {
          value = new_ast0(last_literal_type, -eval_constant(value, false));
        }
        last_literal_type = get_op(value);
#else
        if (get_op(value) != INTEGER
        && get_op(value) != INTEGER_U && get_op(value) != INTEGER_UL && get_op(value) != INTEGER_ULL
        && get_op(value) != INTEGER_L && get_op(value) != INTEGER_LL
           ) {
        value = new_ast0(last_literal_type, -eval_constant(value, false)); // negative value to indicate it's a small integer
        }
#endif
        next_value = get_val(value) - 1; // Next value is the current value + 1, but val is negative
      } else {
        value = new_ast0(last_literal_type, next_value);
        next_value -= 1;
      }

      if (result == 0) {
        result = cons(new_ast2('=', ident, value), 0);
        tail = result;
      } else {
        set_child(tail, 1, cons(new_ast2('=', ident, value), 0));
        tail = get_child_(LIST, tail, 1);
      }

      if (tok == ',') {
        get_tok();
      } else {
        break;
      }
    }

    expect_tok('}');

  }

  return new_ast3(ENUM_KW, 0, name, result); // child#0 is the storage-class specifiers and type qualifiers
}

ast parse_struct_or_union(int struct_or_union_tok) {
  ast name;
  ast type_specifier, decl;
  ast result = 0;
  ast tail;
  bool ends_in_flex_array = false;

  expect_tok(struct_or_union_tok);

  if (tok == IDENTIFIER || tok == TYPE) {
    // When the struct/union keyword is used with an identifier that's typedefed, the typedef is ignored.
    name = new_ast0(IDENTIFIER, val);
    get_tok();
  } else {
    name = 0; // Unnamed struct
  }

  // Note: The parser doesn't distinguish between a reference to a struct/union type and a declaration.
  // If child#2 is 0, it's either a reference to a type or a forward declaration.
  if (tok == '{') {
    get_tok();

    while (tok != '}') {
      if (!is_type_starter(tok)) parse_error("type expected in struct declaration", tok);
      if (ends_in_flex_array)    parse_error("flexible array member must be last", tok);
      type_specifier = parse_declaration_specifiers(false);

      // If the decl has no name, it's an anonymous struct/union member
      // and there can only be 1 declarator so not looping.
      if (tok == ';') {
        if (get_op(type_specifier) != ENUM_KW && get_op(type_specifier) != STRUCT_KW && get_op(type_specifier) != UNION_KW) {
          parse_error("Anonymous struct/union member must be a struct or union type", tok);
        }
        decl = new_ast3(DECL, 0, type_specifier, 0);

        if (result == 0) {
          tail = result = cons(decl, 0);
        } else {
          set_child(tail, 1, cons(decl, 0));
          tail = get_child_(LIST, tail, 1);
        }
      } else {
        while (1) {
          decl = parse_declarator(false, type_specifier);
          if (result == 0) {
            tail = result = cons(decl, 0);
          } else {
            set_child(tail, 1, cons(decl, 0));
            tail = get_child_(LIST, tail, 1);
          }

          if (get_child_(DECL, decl, 1) == VOID_KW) parse_error("member with void type not allowed in struct/union", tok);
          if (get_child_(DECL, decl, 1) == '[' && get_child_('[', get_child_(DECL, decl, 1), 1) == 0) {
            // Set ends_in_flex_array if the type is an array with no size
            ends_in_flex_array = true;
            break;
          }
          if (tok == ',') get_tok();
          else break;
        }
      }

      expect_tok(';');
    }

    expect_tok('}');
  }

  return new_ast3(struct_or_union_tok, 0, name, result); // child#0 is the storage-class specifiers and type qualifiers
}

ast parse_type_specifier() {
  ast type_specifier = 0;
  switch (tok) {
    case CHAR_KW:
    case INT_KW:
    case VOID_KW:
#ifndef sh
    case FLOAT_KW:
    case DOUBLE_KW:
#endif
      type_specifier = new_ast0(tok, 0);
      get_tok();
      return type_specifier;

    case SHORT_KW:
      get_tok();
      if (tok == INT_KW) get_tok(); // Just "short" is equivalent to "short int"
      return new_ast0(SHORT_KW, 0);

    case SIGNED_KW:
      get_tok();
      type_specifier = parse_type_specifier();
      // Just "signed" is equivalent to "signed int"
      if (type_specifier == 0) type_specifier = new_ast0(INT_KW, 0);
      return type_specifier;

#ifndef sh
    case UNSIGNED_KW:
      get_tok();
      type_specifier = parse_type_specifier();
      // Just "unsigned" is equivalent to "unsigned int"
      if (type_specifier == 0) type_specifier = new_ast0(INT_KW, MK_TYPE_SPECIFIER(UNSIGNED_KW));
      // Set the unsigned flag
      else set_val(type_specifier, get_val(type_specifier) | MK_TYPE_SPECIFIER(UNSIGNED_KW));
      return type_specifier;
#endif

    case LONG_KW:
      get_tok();
#ifndef sh
      if (tok == DOUBLE_KW) {
        get_tok();
        return new_ast0(DOUBLE_KW, 0);
      } else
#endif
      {
        if (tok == LONG_KW) {
          get_tok();
          if (tok == INT_KW) get_tok(); // Just "long long" is equivalent to "long long int"
          return new_ast0(LONG_KW, 0);
        } else if (tok == INT_KW) {
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
int glo_specifier_storage_class = 0;
ast parse_declaration_specifiers(bool allow_typedef) {
  ast type_specifier = 0;
  int type_qualifier = 0;
  bool loop = true;
  int specifier_storage_class = 0;

  while (loop) {
    switch (tok) {
      case AUTO_KW:
      case REGISTER_KW:
      case STATIC_KW:
      case EXTERN_KW:
      case TYPEDEF_KW:
        if (specifier_storage_class != 0) fatal_error("Multiple storage classes not supported");
        if (tok == TYPEDEF_KW && !allow_typedef) parse_error("Unexpected typedef", tok);
        specifier_storage_class = tok;
        get_tok();
        break;

      case INLINE_KW:
        get_tok(); // Ignore inline
        break;

      case CONST_KW:
      case VOLATILE_KW:
        type_qualifier |= MK_TYPE_SPECIFIER(tok);
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
        if (type_specifier != 0) parse_error("Unexpected C type specifier", tok);
        type_specifier = parse_type_specifier();
        if (type_specifier == 0) parse_error("Failed to parse type specifier", tok);
        break;

      case STRUCT_KW:
      case UNION_KW:
        if (type_specifier != 0) parse_error("Multiple types not supported", tok);
        type_specifier = parse_struct_or_union(tok);
        break;

      case ENUM_KW:
        if (type_specifier != 0) parse_error("Multiple types not supported", tok);
        type_specifier = parse_enum();
        break;

      case TYPE:
        if (type_specifier != 0) parse_error("Multiple types not supported", tok);
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
  if (type_specifier == 0) parse_error("Type expected", tok);

  if (type_qualifier != 0) {
    // This can only happen if an array/function type is typedef'ed
    if (get_op(type_specifier) == '[' || get_op(type_specifier) == '(')
      parse_error("Type qualifiers not allowed on typedef'ed array or function type", tok);

    // Set the type qualifier, keeping the storage class specifier from the typedef if it exists
    set_child(type_specifier, 0, get_child(type_specifier, 0) | type_qualifier);
  }
  glo_specifier_storage_class = specifier_storage_class;

  return type_specifier;
}

bool parse_param_list_is_variadic = false;
int parse_param_list() {
  ast result = 0;
  ast tail;
  ast decl;

  parse_param_list_is_variadic = false;

  expect_tok('(');

  while (tok != ')' && tok != EOF) {
    if (is_type_starter(tok)) {
      decl = parse_declarator(true, parse_declaration_specifiers(false));
      if (get_op(get_child_(DECL, decl, 1)) == VOID_KW) {
        if (tok != ')' || result != 0) parse_error("void must be the only parameter", tok);
        break;
      }
    } else if (tok == IDENTIFIER) {
      // Support K&R param syntax in function definition
      decl = new_ast3(DECL, new_ast0(IDENTIFIER, val), new_ast0(INT_KW, 0), 0);
      get_tok();
    } else if (tok == ELLIPSIS) {
      // ignore ELLIPSIS nodes for now, but it should be the last parameter
      if (result == 0) parse_error("Function must have a named parameter before ellipsis parameter", tok);
      get_tok();
      parse_param_list_is_variadic = true;
      break;
    } else {
      parse_error("Parameter declaration expected", tok);
    }

    if (tok == ',') get_tok();

    if (result == 0) {
      tail = result = cons(decl, 0);
    } else {
      set_child(tail, 1, cons(decl, 0));
      tail = get_child_(LIST, tail, 1);
    }
  }

  expect_tok(')');

  return result;
}

ast get_inner_type(ast type) {
  switch (get_op(type)) {
    case DECL:
    case '*':
      return get_child(type, 1);
    case '[':
    case '(':
      return get_child(type, 0);
    default:
      fatal_error("Invalid type");
      return 0;
  }
}

void update_inner_type(ast parent_type, ast inner_type) {
  switch (get_op(parent_type)) {
    case DECL:
    case '*':
      set_child(parent_type, 1, inner_type);
      break;

    case '[':
    case '(':
      set_child(parent_type, 0, inner_type);
      break;
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
ast parse_declarator_parent_type_parent;
ast parse_declarator(bool abstract_decl, ast parent_type) {
  bool first_tok = tok; // Indicates if the declarator is a noptr-declarator
  ast result = 0;
  ast decl;
  ast arr_size_expr;
  ast parent_type_parent;

  switch (tok) {
    case IDENTIFIER:
      result = new_ast3(DECL, new_ast0(IDENTIFIER, val), parent_type, 0); // child#2 is the initializer
      parent_type_parent = result;
      get_tok();
      break;

    case '*':
      get_tok();
      // Pointers may be const-qualified
      parent_type_parent = pointer_type(parent_type, tok == CONST_KW);
      if (tok == CONST_KW) get_tok();
      result = parse_declarator(abstract_decl, parent_type_parent);
      break;

    // Parenthesis delimit the specifier-and-qualifier part of the declaration from the declarator
    case '(':
      get_tok();
      result = parse_declarator(abstract_decl, parent_type);
      parent_type_parent = parse_declarator_parent_type_parent;
      expect_tok(')');
      break;

    default:
      // Abstract declarators don't need names, and so in the base declarator,
      // we don't require an identifier. This is useful for function pointers.
      // In that case, we create a DECL node with no identifier.
      if (abstract_decl) {
        result = new_ast3(DECL, 0, parent_type, 0); // child#0 is the identifier, child#2 is the initializer
        parent_type_parent = result;
      } else {
        parse_error("Invalid declarator, expected an identifier but declarator doesn't have one", tok);
      }
  }

  // At this point, the only non-recursive declarator is an identifier
  // so we know that get_op(result) == DECL.
  // Because we want the DECL to stay as the outermost node, we temporarily
  // unwrap the DECL parent_type.
  decl = result;
  result = get_child_(DECL, decl, 1);

  while (first_tok != '*') {
    // noptr-declarator may be followed by [ constant-expression ] to declare an
    // array or by ( parameter-type-list ) to declare a function. We loop since
    // both may be present.
    if (tok == '[') {
      // Check if not a void array
      if (get_op(result) == VOID_KW) parse_error("void array not allowed", tok);
        get_tok();
      if (tok == ']') {
        val = 0;
      } else {
        arr_size_expr = parse_assignment_expression();
        if (arr_size_expr == 0) parse_error("Array size must be an integer constant", tok);
        val = eval_constant(arr_size_expr, false);
      }
      result = new_ast2('[', get_inner_type(parent_type_parent), val);
      update_inner_type(parent_type_parent, result);
      parent_type_parent = result;
      expect_tok(']');
    } else if (tok == '(') {
      result = new_ast3('(', get_inner_type(parent_type_parent), parse_param_list(), false);
      if (parse_param_list_is_variadic) result = make_variadic_func(result);
      update_inner_type(parent_type_parent, result);
      parent_type_parent = result;
    } else {
      break;
    }
  }

  parse_declarator_parent_type_parent = parent_type_parent;
  return decl;
}

ast parse_initializer_list() {
  ast result = 0, tail = 0;

  expect_tok('{');

  while (tok != '}' && tok != EOF) {
    if (result == 0) {
      tail = result = cons(parse_initializer(), 0);
    } else {
      set_child(tail, 1, cons(parse_initializer(), 0));
      tail = get_child_(LIST, tail, 1);
    }
    if (tok == ',') get_tok();
    else break;
  }

  expect_tok('}');

  return new_ast1(INITIALIZER_LIST, result);
}

ast parse_initializer() {
  if (tok == '{') {
    return parse_initializer_list();
  } else {
    return parse_assignment_expression();
  }
}

ast parse_declarator_and_initializer(bool is_for_typedef, ast type_specifier) {
  ast declarator = parse_declarator(false, type_specifier);

  if (is_for_typedef == 0) {
    if (tok == '=') {
      get_tok();
      // parse_declarator returns a DECL node where the initializer is child#2
      set_child(declarator, 2, parse_initializer());
    }
  }

  return declarator;
}

ast parse_declarators(bool is_for_typedef, ast type_specifier, ast first_declarator) {
  ast declarators = cons(first_declarator, 0); // Wrap the declarators in a list
  ast tail = declarators;

  // Otherwise, this is a variable or declaration
  while (tok != ';') {
    if (tok == ',') {
      get_tok();
      set_child(tail, 1, cons(parse_declarator_and_initializer(is_for_typedef, type_specifier), 0));
      tail = get_child__(LIST, LIST, tail, 1);
    } else {
      parse_error("';' or ',' expected", tok);
    }
  }

  return declarators;
}

void add_typedef(ast declarator) {
  int decl_ident = get_val_(IDENTIFIER, get_child__(DECL, IDENTIFIER, declarator, 0));
  ast decl_type = get_child_(DECL, declarator, 1); // child#1 is the type

  heap[decl_ident + 2] = TYPE;
  heap[decl_ident + 3] = decl_type;
}

ast parse_fun_def(ast declarator) {
  ast fun_type = get_child__(DECL, '(', declarator, 1);
  ast params = get_child_('(', fun_type, 1);

  // Check that the parameters are all named since declarator may be abstract
  while (params != 0) {
    if (get_child_(DECL, get_child__(LIST, DECL, params, 0), 0) == 0) {
      parse_error("Parameter name expected", tok);
    }
    params = get_child_(LIST, params, 1);
  }
  if (get_child_(DECL, declarator, 2) != 0) parse_error("Initializer not allowed in function definition", tok);
  return new_ast2(FUN_DECL, declarator, parse_compound_statement());
}

ast parse_declaration(bool local) {
  ast result;
  ast declarator;
  ast declarators;
  // First we parse the specifiers:
  ast type_specifier = parse_declaration_specifiers(true);

  // From cppreference:
  // > The enum, struct, and union declarations may omit declarators, in which
  // > case they only introduce the enumeration constants and/or tags.
  if (tok == ';') {
    if (get_op(type_specifier) != ENUM_KW && get_op(type_specifier) != STRUCT_KW && get_op(type_specifier) != UNION_KW) {
      parse_error("enum/struct/union declaration expected", tok);
    }
    // If the specifier is a typedef, we add the typedef'ed type to the type table
    // Note: Should this return a DECL node instead of a ENUM, STRUCT, or UNION node?
    // It doesn't have a name so maybe it makes more sense to have a separate node type?
    if (glo_specifier_storage_class == TYPEDEF_KW) add_typedef(new_ast3(DECL, 0, type_specifier, 0));
    result = type_specifier;
  } else if (glo_specifier_storage_class == TYPEDEF_KW) {
    // The type_specifier contained a typedef, it can't be a function or a
    // variable declaration, and the declarators cannot be initialized.
    // The typedef'ed types will be added to the type table.
    declarator = parse_declarator_and_initializer(true, type_specifier); // First declarator
    declarators = parse_declarators(true, type_specifier, declarator);
    type_specifier = declarators; // Save declarators in type_specifier
    while (declarators != 0) {
      add_typedef(get_child__(LIST, DECL, declarators, 0));
      declarators = get_child_opt_(LIST, LIST, declarators, 1);
    }
    result = new_ast1(TYPEDEF_KW, type_specifier);
  } else {
    // Then we parse the declarators and initializers
    declarator = parse_declarator_and_initializer(false, type_specifier);

    // The declarator may be a function definition, in which case we parse the function body
    if (get_op(get_child_(DECL, declarator, 1)) == '(' && tok == '{') {
      if (local) parse_error("Function definition not allowed in local scope", tok);
      return parse_fun_def(declarator);
    }

    declarators = parse_declarators(false, type_specifier, declarator);
    result = new_ast2(DECLS, declarators, glo_specifier_storage_class); // child#1 is the storage class specifier
  }

  expect_tok(';');
  return result;
}

ast parse_parenthesized_expression() {

  ast result;

  expect_tok('(');

  result = parse_comma_expression();

  expect_tok(')');

  return new_ast1(PARENS, result);
}

ast parse_primary_expression() {

  ast result = 0;
  ast tail;

  if (tok == IDENTIFIER || tok == CHARACTER || tok == INTEGER
     || tok == INTEGER_L ||  tok == INTEGER_LL ||  tok == INTEGER_U ||  tok == INTEGER_UL ||  tok == INTEGER_ULL
     ) {

    result = new_ast0(tok, val);
    get_tok();

  } else if (tok == STRING) {
    result = new_ast0(STRING, val);
    get_tok();

    if (tok == STRING) { // Contiguous strings
      result = cons(get_val_(STRING, result), 0); // Result is now a list of string values
      tail = result;
      while (tok == STRING) {
        set_cdr(tail, cons(val, 0));
        tail = cdr(tail);
        get_tok();
      }

      // Unpack the list of strings into a single string
      begin_string();

      while (result != 0) {
        accum_string_string(car(result));
        result = cdr(result);
      }

      result = new_ast0(STRING, end_string());
    }

  } else if (tok == '(') {

    result = parse_parenthesized_expression();

  } else {
    parse_error("identifier, literal, or '(' expected", tok);
  }

  return result;
}

ast parse_postfix_expression() {

  ast result;
  ast child;

  result = parse_primary_expression();

  while (1) {
    if (tok == '[') {

      get_tok();
      child = parse_comma_expression();
      result = new_ast2('[', result, child);
      expect_tok(']');

    } else if (tok == '(') {

      get_tok();
      if (tok == ')') {
        child = 0;
      } else {
        child = parse_call_params();
      }
      result = new_ast2('(', result, child);
      expect_tok(')');

    } else if (tok == '.') {

      get_tok();
      if (tok != IDENTIFIER) {
        parse_error("identifier expected", tok);
      }
      result = new_ast2('.', result, new_ast0(IDENTIFIER, val));
      get_tok();

    } else if (tok == ARROW) {

      get_tok();
      if (tok != IDENTIFIER) {
        parse_error("identifier expected", tok);
      }
      result = new_ast2(ARROW, result, new_ast0(IDENTIFIER, val));
      get_tok();

    } else if (tok == PLUS_PLUS) {

      get_tok();
      result = new_ast1(PLUS_PLUS_POST, result);

    } else if (tok == MINUS_MINUS) {

      get_tok();
      result = new_ast1(MINUS_MINUS_POST, result);

    } else {
      break;
    }
  }

  return result;
}

ast parse_unary_expression() {

  ast result;
  int op;

  if (tok == PLUS_PLUS) {

    get_tok();
    result = parse_unary_expression();
    result = new_ast1(PLUS_PLUS_PRE, result);

  } else if (tok == MINUS_MINUS) {

    get_tok();
    result = parse_unary_expression();
    result = new_ast1(MINUS_MINUS_PRE, result);

  } else if (tok == '&' || tok == '*' || tok == '+' || tok == '-' || tok == '~' || tok == '!') {

    op = tok;
    get_tok();
    result = parse_cast_expression();
    result = new_ast1(op, result);

  } else if (skip_newlines && tok == SIZEOF_KW) { // only parse sizeof if we're not in a #if expression

    get_tok();
    if (tok == '(') {
      get_tok();
      // May be a type or an expression
      if (is_type_starter(tok)) {
      result = parse_declarator(true, parse_declaration_specifiers(false));
      expect_tok(')');
      } else {
        // We need to put the current token and '(' back on the token stream.
        // Otherwise, sizeof (cast_expression) fails to parse.
        undo_token('(', 0);
        result = parse_unary_expression();
      }
    } else {
      result = parse_unary_expression();
    }
    result = new_ast1(SIZEOF_KW, result);

  } else if (!skip_newlines && tok == IDENTIFIER && val == DEFINED_ID) { // Parsing a macro

    get_tok_macro();
    if (tok == '(') {
      get_tok_macro();
      result = new_ast2('(', new_ast0(IDENTIFIER, DEFINED_ID), tok);
      get_tok_macro();
      expect_tok(')');
    } else if (tok == IDENTIFIER || tok == MACRO) {
      result = new_ast2('(', new_ast0(IDENTIFIER, DEFINED_ID), tok);
      get_tok_macro();
    } else {
      parse_error("identifier or '(' expected", tok);
      return 0;
    }

  } else {
    result = parse_postfix_expression();
  }

  return result;
}

ast parse_cast_expression() {
  ast result;
  ast type;

  if (tok == '(') {
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

      expect_tok(')');
      result = new_ast2(CAST, type, parse_cast_expression());
      return result;
    } else {
      // We need to put the current token and '(' back on the token stream.
      undo_token('(', 0);
    }
  }

  return parse_unary_expression();
}

ast parse_multiplicative_expression() {

  ast result = parse_cast_expression();
  ast child;
  int op;

  while (tok == '*' || tok == '/' || tok == '%') {

    op = tok;
    get_tok();
    child = parse_cast_expression();
    result = new_ast2(op, result, child);

  }

  return result;
}

ast parse_additive_expression() {

  ast result = parse_multiplicative_expression();
  ast child;
  int op;

  while (tok == '+' || tok == '-') {

    op = tok;
    get_tok();
    child = parse_multiplicative_expression();
    result = new_ast2(op, result, child);

  }

  return result;
}

ast parse_shift_expression() {

  ast result = parse_additive_expression();
  ast child;
  int op;

  while (tok == LSHIFT || tok == RSHIFT) {

    op = tok;
    get_tok();
    child = parse_additive_expression();
    result = new_ast2(op, result, child);

  }

  return result;
}

ast parse_relational_expression() {

  ast result = parse_shift_expression();
  ast child;
  int op;

  while (tok == '<' || tok == '>' || tok == LT_EQ || tok == GT_EQ) {

    op = tok;
    get_tok();
    child = parse_shift_expression();
    result = new_ast2(op, result, child);

  }

  return result;
}

ast parse_equality_expression() {

  ast result = parse_relational_expression();
  ast child;
  int op;

  while (tok == EQ_EQ || tok == EXCL_EQ) {

    op = tok;
    get_tok();
    child = parse_relational_expression();
    result = new_ast2(op, result, child);

  }

  return result;
}

ast parse_AND_expression() {

  ast result = parse_equality_expression();
  ast child;

  while (tok == '&') {

    get_tok();
    child = parse_equality_expression();
    result = new_ast2('&', result, child);

  }

  return result;
}

ast parse_exclusive_OR_expression() {

  ast result = parse_AND_expression();
  ast child;

  while (tok == '^') {

    get_tok();
    child = parse_AND_expression();
    result = new_ast2('^', result, child);

  }

  return result;
}

ast parse_inclusive_OR_expression() {

  ast result = parse_exclusive_OR_expression();
  ast child;

  while (tok == '|') {

    get_tok();
    child = parse_exclusive_OR_expression();
    result = new_ast2('|', result, child);

  }

  return result;
}

ast parse_logical_AND_expression() {

  ast result = parse_inclusive_OR_expression();
  ast child;

  while (tok == AMP_AMP) {

    get_tok();
    child = parse_inclusive_OR_expression();
    result = new_ast2(AMP_AMP, result, child);

  }

  return result;
}

ast parse_logical_OR_expression() {

  ast result = parse_logical_AND_expression();
  ast child;

  while (tok == BAR_BAR) {

    get_tok();
    child = parse_logical_AND_expression();
    result = new_ast2(BAR_BAR, result, child);

  }

  return result;
}

ast parse_conditional_expression() {

  ast result = parse_logical_OR_expression();
  ast child1;
  ast child2;

  if (tok == '?') {

    get_tok();
    child1 = parse_comma_expression();
    expect_tok(':');
    child2 = parse_conditional_expression();
    result = new_ast3('?', result, child1, child2);

  }

  return result;
}

ast parse_assignment_expression() {

  ast result = parse_conditional_expression();
  ast child;
  int op;

  if (   tok == '='       || tok == PLUS_EQ   || tok == MINUS_EQ
      || tok == STAR_EQ   || tok == SLASH_EQ  || tok == PERCENT_EQ
      || tok == LSHIFT_EQ || tok == RSHIFT_EQ || tok == AMP_EQ
      || tok == CARET_EQ  || tok == BAR_EQ) {

    op = tok;
    get_tok();
    child = parse_assignment_expression();
    result = new_ast2(op, result, child);

  }

  return result;
}

ast parse_comma_expression() {

  ast result = parse_assignment_expression();

  if (tok == ',') { // "comma expressions" without , don't need to be wrapped in a comma node
    get_tok();
    result = new_ast2(',', result, 0);
    set_child(result, 1, parse_comma_expression());
  }

  return result;
}

ast parse_call_params() {
  ast result = parse_assignment_expression();
  result = new_ast2(LIST, result, 0);

  if (tok == ',') {
    get_tok();
    set_child(result, 1, parse_call_params());
  }

  return result;
}

ast parse_comma_expression_opt() {

  ast result;

  if (tok == ':' || tok == ';' || tok == ')') {
    result = 0;
  } else {
    result = parse_comma_expression();
  }

  return result;
}

ast parse_expression() {
  return parse_comma_expression();
}

ast parse_constant_expression() {
  return parse_expression();
}

ast parse_statement() {

  ast result;
  ast child1;
  ast child2;
  ast child3;
  int start_tok;

  if (tok == IF_KW) {

    get_tok();
    result = parse_parenthesized_expression();
    child1 = parse_statement();

    if (tok == ELSE_KW) {
      get_tok();
      child2 = parse_statement();
    } else {
      child2 = 0;
    }

    result = new_ast3(IF_KW, result, child1, child2);

  } else if (tok == SWITCH_KW) {

    get_tok();
    result = parse_parenthesized_expression();
    child1 = parse_statement();

    result = new_ast2(SWITCH_KW, result, child1);

  } else if (tok == CASE_KW) {

    get_tok();
    result = parse_constant_expression();
    expect_tok(':');
    child1 = parse_statement();

    result = new_ast2(CASE_KW, result, child1);

  } else if (tok == DEFAULT_KW) {

    get_tok();
    expect_tok(':');
    result = parse_statement();

    result = new_ast1(DEFAULT_KW, result);

  } else if (tok == WHILE_KW) {

    get_tok();
    result = parse_parenthesized_expression();
    child1 = parse_statement();

    result = new_ast2(WHILE_KW, result, child1);

  } else if (tok == DO_KW) {

    get_tok();
    result = parse_statement();
    expect_tok(WHILE_KW);
    child1 = parse_parenthesized_expression();
    expect_tok(';');

    result = new_ast2(DO_KW, result, child1);

  } else if (tok == FOR_KW) {

    get_tok();
    expect_tok('(');
    result = parse_comma_expression_opt();
    expect_tok(';');
    child1 = parse_comma_expression_opt();
    expect_tok(';');
    child2 = parse_comma_expression_opt();
    expect_tok(')');
    child3 = parse_statement();

    result = new_ast4(FOR_KW, result, child1, child2, child3);

  } else if (tok == GOTO_KW) {

    get_tok();
    expect_tok(IDENTIFIER);
    result = new_ast1(GOTO_KW, new_ast0(IDENTIFIER, val));
    expect_tok(';');

  } else if (tok == CONTINUE_KW) {

    get_tok();
    expect_tok(';');

    result = new_ast0(CONTINUE_KW, 0);

  } else if (tok == BREAK_KW) {

    get_tok();
    expect_tok(';');

    result = new_ast0(BREAK_KW, 0);

  } else if (tok == RETURN_KW) {

    get_tok();
    result = parse_comma_expression_opt();
    expect_tok(';');

    result = new_ast1(RETURN_KW, result);

  } else if (tok == '{') {

    result = parse_compound_statement();

  } else {

    start_tok = tok;

    result = parse_comma_expression_opt();

    if (tok == ':' && start_tok != '(' && get_op(result) == IDENTIFIER) {

      get_tok(); // Skip :

      child1 = parse_statement();

      result = new_ast2(':', result, child1);

    } else {

      expect_tok(';');

    }
  }

  return result;
}

ast parse_compound_statement() {

  ast result = 0;
  ast child1;
  ast tail;

  expect_tok('{');

  // TODO: Simplify this
  if (tok != '}' && tok != EOF) {
    if (is_type_starter(tok)) {
      child1 = parse_declaration(true);
    } else {
      child1 = parse_statement();
    }
    result = new_ast2('{', child1, 0);
    tail = result;
    while (tok != '}' && tok != EOF) {
      if (is_type_starter(tok)) {
        child1 = parse_declaration(true);
      } else {
        child1 = parse_statement();
      }
      child1 = new_ast2('{', child1, 0);
      set_child(tail, 1, child1);
      tail = child1;
    }
  }

  expect_tok('}');

  return result;
}

//-----------------------------------------------------------------------------

// Select code generator

// start x86.c
#define WORD_SIZE 4

// x86 codegen
// start exe.c

// common part of machine code generators
void generate_exe();

// 1MB heap
#define RT_HEAP_SIZE 104857600

#define MAX_CODE_SIZE 5000000
int code[MAX_CODE_SIZE];
int code_alloc = 0;

void emit_i8(int a) {
  if (code_alloc >= MAX_CODE_SIZE) {
    fatal_error("code buffer overflow");
  }
  code[code_alloc] = (a & 0xff);
  code_alloc += 1;
}

void emit_2_i8(int a, int b) {
  emit_i8(a);
  emit_i8(b);
}

void emit_4_i8(int a, int b, int c, int d) {
  emit_2_i8(a, b);
  emit_2_i8(c, d);
}

void emit_i32_le(int n) {
  emit_4_i8(n, n >> 8, n >> 16, n >> 24);
}

void emit_i64_le(int n) {
  emit_i32_le(n);
  // Sign extend to 64 bits. Arithmetic shift by 31 gives -1 for negative numbers and 0 for positive numbers.
  emit_i32_le(n >> 31);
}

#ifdef SUPPORT_64_BIT_LITERALS
void emit_i32_le_large_imm(int imm_obj) {
  if (imm_obj <= 0) {
    emit_i32_le(-imm_obj);
  } else {
    // Check that the number doesn't overflow 64 bits
    if (heap[imm_obj + 1] != 0) fatal_error("emit_i32_le_large_imm: integer overflow");
    emit_i32_le(heap[imm_obj]);
  }
}

void emit_i64_le_large_imm(int imm_obj) {
  if (imm_obj <= 0) {
    emit_i64_le(-imm_obj);
  } else {
    emit_i32_le(heap[imm_obj]);
    emit_i32_le(heap[imm_obj + 1]);
  }
}
#endif

char write_buf[1];
void write_i8(int n) {
  write_buf[0] = (n & 0xff);
  write(output_fd, write_buf, 1);
}

void write_2_i8(int a, int b) {
  write_i8(a);
  write_i8(b);
}

void write_4_i8(int a, int b, int c, int d) {
  write_2_i8(a, b);
  write_2_i8(c, d);
}

void write_i32_le(int n) {
  write_4_i8(n, n >> 8, n >> 16, n >> 24);
}

// If the main function returns a value
bool main_returns = false;

// Environment tracking
int cgc_fs = 0;
// Function bindings that follows lexical scoping rules
int cgc_locals = 0;
// Like cgc_locals, but with 1 scope for the entire function
int cgc_locals_fun = 0;
// Global bindings
int cgc_globals = 0;
// Bump allocator used to allocate static objects
int cgc_global_alloc = 0;

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
#define binding_next(binding)  heap[binding]
#define binding_kind(binding)  heap[binding+1]
#define binding_ident(binding) heap[binding+2]

int cgc_lookup_binding_ident(int binding_type, int ident, int binding) {
  while (binding != 0) {
    if (binding_kind(binding) == binding_type && binding_ident(binding) == ident) {
      break;
    }
    binding = binding_next(binding);
  }
  return binding;
}

int cgc_lookup_last_binding(int binding_type, int binding) {
  while (binding != 0) {
    if (binding_kind(binding) == binding_type) {
      break;
    }
    binding = binding_next(binding);
  }
  return binding;
}

int cgc_lookup_var(int ident, int binding) {
  while (binding != 0) {
    if (binding_kind(binding) <= BINDING_VAR_GLOBAL && binding_ident(binding) == ident) {
      break;
    }
    binding = binding_next(binding);
  }
  return binding;
}

int cgc_lookup_fun(int ident, int env) {
  return cgc_lookup_binding_ident(BINDING_FUN, ident, env);
}

int cgc_lookup_enclosing_loop(int env) {
  return cgc_lookup_last_binding(BINDING_LOOP, env);
}

int cgc_lookup_enclosing_switch(int env) {
  return cgc_lookup_last_binding(BINDING_SWITCH, env);
}

int cgc_lookup_enclosing_loop_or_switch(int binding) {
  while (binding != 0) {
    if (binding_kind(binding) == BINDING_LOOP || binding_kind(binding) == BINDING_SWITCH) {
      break;
    }
    binding = binding_next(binding);
  }
  return binding;
}

int cgc_lookup_goto_label(int ident, int env) {
  return cgc_lookup_binding_ident(BINDING_GOTO_LABEL, ident, env);
}

int cgc_lookup_struct(int ident, int env) {
  return cgc_lookup_binding_ident(BINDING_TYPE_STRUCT, ident, env);
}

int cgc_lookup_union(int ident, int env) {
  return cgc_lookup_binding_ident(BINDING_TYPE_UNION, ident, env);
}

int cgc_lookup_enum(int ident, int env) {
  return cgc_lookup_binding_ident(BINDING_TYPE_ENUM, ident, env);
}

int cgc_lookup_enum_value(int ident, int env) {
  return cgc_lookup_binding_ident(BINDING_ENUM_CST, ident, env);
}

int cgc_loop_depth(int binding) {
  int loop_depth = 0;
  binding = cgc_lookup_enclosing_loop(binding); // Find the first loop
  while (binding != 0) {
    binding = cgc_lookup_enclosing_loop(binding_next(binding));
    loop_depth += 1;
  }
  return loop_depth;
}

int cgc_add_local(enum BINDING binding_type, int ident, ast type, int env) {
  int binding = alloc_obj(5);
  heap[binding+0] = env;
  heap[binding+1] = binding_type;
  heap[binding+2] = ident;
  heap[binding+3] = cgc_fs;
  heap[binding+4] = type;
  return binding;
}

#ifdef sh
void cgc_add_local_var(enum BINDING binding_type, int ident, ast type) {
  cgc_fs += 1;
  cgc_locals = cgc_add_local(binding_type, ident, type, cgc_locals);
  // Add to cgc_locals_fun as well, if not already there
  if (cgc_lookup_var(ident, cgc_locals_fun) == 0) {
    cgc_locals_fun = cgc_add_local(binding_type, ident, type, cgc_locals_fun);
  }
}

void cgc_add_enclosing_loop() {
  int binding = alloc_obj(4);
  heap[binding+0] = cgc_locals;
  heap[binding+1] = BINDING_LOOP;
  heap[binding+2] = 0; // loop end action start
  heap[binding+3] = 0; // loop end action end
  cgc_locals = binding;
}

void cgc_add_enclosing_switch(bool in_tail_position) {
  int binding = alloc_obj(3);
  heap[binding+0] = cgc_locals;
  heap[binding+1] = BINDING_SWITCH;
  heap[binding+2] = in_tail_position;
  cgc_locals = binding;
}
#else
void cgc_add_local_param(int ident, int width, ast type) {
  cgc_locals = cgc_add_local(BINDING_PARAM_LOCAL, ident, type, cgc_locals);
  cgc_fs -= width;
}

void cgc_add_local_var(int ident, int width, ast type) {
  cgc_fs += width;
  cgc_locals = cgc_add_local(BINDING_VAR_LOCAL, ident, type, cgc_locals);
}

void cgc_add_enclosing_loop(int loop_fs, int break_lbl, ast continue_lbl) {
  int binding = alloc_obj(5);
  heap[binding+0] = cgc_locals;
  heap[binding+1] = BINDING_LOOP;
  heap[binding+2] = loop_fs;
  heap[binding+3] = break_lbl;
  heap[binding+4] = continue_lbl;
  cgc_locals = binding;
}

void cgc_add_enclosing_switch(int loop_fs, int break_lbl, int next_case_lbl) {
  int binding = alloc_obj(6);
  heap[binding+0] = cgc_locals;
  heap[binding+1] = BINDING_SWITCH;
  heap[binding+2] = loop_fs;
  heap[binding+3] = break_lbl;
  heap[binding+4] = next_case_lbl;
  heap[binding+5] = 0; // Default label
  cgc_locals = binding;
}

void cgc_add_global(int ident, int width, ast type, bool is_static_local) {
  int binding = alloc_obj(5);
  heap[binding+0] = is_static_local ? cgc_locals : cgc_globals;
  heap[binding+1] = BINDING_VAR_GLOBAL;
  heap[binding+2] = ident;
  heap[binding+3] = cgc_global_alloc;
  heap[binding+4] = type;
  cgc_global_alloc += width;
  if (is_static_local) {
    cgc_locals = binding;
  } else {
    cgc_globals = binding;
  }
}

void cgc_add_global_fun(int ident, int label, ast type) {
  int binding = alloc_obj(6);
  heap[binding+0] = cgc_globals;
  heap[binding+1] = BINDING_FUN;
  heap[binding+2] = ident;
  heap[binding+3] = 0;
  heap[binding+4] = label;
  heap[binding+5] = type;
  cgc_globals = binding;
}

void cgc_add_enum(int ident, int value) {
  int binding = alloc_obj(4);
  heap[binding+0] = cgc_globals;
  heap[binding+1] = BINDING_ENUM_CST;
  heap[binding+2] = ident;
  heap[binding+3] = value;
  cgc_globals = binding;
}

void cgc_add_goto_label(int ident, int lbl) {
  int binding = alloc_obj(5);
  heap[binding+0] = cgc_locals_fun;
  heap[binding+1] = BINDING_GOTO_LABEL;
  heap[binding+2] = ident;
  heap[binding+3] = lbl;
  cgc_locals_fun = binding;
}

void cgc_add_typedef(int ident, enum BINDING struct_or_union_or_enum, ast type) {
  int binding = alloc_obj(4);
  heap[binding+0] = cgc_globals;
  heap[binding+1] = struct_or_union_or_enum;
  heap[binding+2] = ident;
  heap[binding+3] = type;
  cgc_globals = binding;
}
#endif

void grow_fs(int words) {
  cgc_fs += words;
}

const int reg_X;
const int reg_Y;
const int reg_Z;
const int reg_SP;
const int reg_glo;

void mov_reg_imm(int dst, int imm);             // Move 32 bit immediate to register
#ifdef SUPPORT_64_BIT_LITERALS
void mov_reg_large_imm(int dst, int large_imm); // Move large immediate to register
#endif
void mov_reg_reg(int dst, int src);
void mov_mem8_reg(int base, int offset, int src);
void mov_mem16_reg(int base, int offset, int src);
void mov_mem32_reg(int base, int offset, int src);
void mov_mem64_reg(int base, int offset, int src);
void mov_mem8_reg(int base, int offset, int src);
void mov_reg_mem8(int dst, int base, int offset);
void mov_reg_mem16(int dst, int base, int offset);
void mov_reg_mem32(int dst, int base, int offset);
void mov_reg_mem8_sign_ext(int dst, int base, int offset);
void mov_reg_mem16_sign_ext(int dst, int base, int offset);
void mov_reg_mem32_sign_ext(int dst, int base, int offset);
void mov_reg_mem64(int dst, int base, int offset);

#if WORD_SIZE == 4
#define mov_mem_reg(base, offset, src) mov_mem32_reg(base, offset, src)
#define mov_reg_mem(dst, base, offset) mov_reg_mem32(dst, base, offset)
#elif WORD_SIZE == 8
#define mov_mem_reg(base, offset, src) mov_mem64_reg(base, offset, src)
#define mov_reg_mem(dst, base, offset) mov_reg_mem64(dst, base, offset)
#endif

void add_reg_imm(int dst, int imm);
void add_reg_lbl(int dst, int lbl);
void add_reg_reg(int dst, int src);
void or_reg_reg (int dst, int src);
void and_reg_reg(int dst, int src);
void sub_reg_reg(int dst, int src);
void xor_reg_reg(int dst, int src);
void imul_reg_reg(int dst, int src); // signed multiplication
void idiv_reg_reg(int dst, int src); // signed division
void irem_reg_reg(int dst, int src); // signed remainder
void mul_reg_reg(int dst, int src);  // unsigned multiplication
void div_reg_reg(int dst, int src);  // unsigned division
void rem_reg_reg(int dst, int src);  // unsigned remainder
void s_l_reg_reg(int dst, int src);  // signed/unsigned left shift
void sar_reg_reg(int dst, int src);  // signed right shift
void shr_reg_reg(int dst, int src);  // unsigned right shift
void mov_reg_lbl(int reg, int lbl);

void push_reg(int src);
void pop_reg (int dst);

void jump(int lbl);
void jump_rel(int offset);
void call(int lbl);
void call_reg(int reg);
void ret();
void debug_interrupt();

void load_mem_location(int dst, int base, int offset, int width, bool is_signed) {
  if (is_signed) {
    switch (width) {
      case 1: mov_reg_mem8_sign_ext(dst, base, offset);  break;
      case 2: mov_reg_mem16_sign_ext(dst, base, offset); break;
#if WORD_SIZE == 4
      case 4: mov_reg_mem32(dst, base, offset); break;
#elif WORD_SIZE == 8
      case 4: mov_reg_mem32_sign_ext(dst, base, offset); break; // This instruction is only available in 64-bit mode
      case 8: mov_reg_mem64(dst, base, offset);          break; // no sign extension needed
#endif
      default: fatal_error("load_mem_location: unknown width");
    }
  } else {
    switch (width) {
      case 1: mov_reg_mem8(dst, base, offset);  break;
      case 2: mov_reg_mem16(dst, base, offset); break;
      case 4: mov_reg_mem32(dst, base, offset); break;
#if WORD_SIZE == 8
      case 8: mov_reg_mem64(dst, base, offset); break;
#endif
      default: fatal_error("load_mem_location: unknown width");
    }
  }
}

// Write a value from a register to a memory location
void write_mem_location(int base, int offset, int src, int width) {
  if (width > WORD_SIZE) {
    fatal_error("write_mem_location: width > WORD_SIZE");
  }

  switch (width) {
    case 1: mov_mem8_reg(base, offset, src); break;
    case 2: mov_mem16_reg(base, offset, src); break;
    case 4: mov_mem32_reg(base, offset, src); break;
    case 8: mov_mem64_reg(base, offset, src); break;
    default: fatal_error("write_mem_location: unknown width");
  }
}

void copy_obj(int dst_base, int dst_offset, int src_base, int src_offset, int width) {
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
void initialize_memory(int val, int base, int offset, int width) {
  int i;
  mov_reg_imm(reg_Z, val);
  for (i = 0; i < width / WORD_SIZE; i += 1) {
    mov_mem_reg(base, offset + i * WORD_SIZE, reg_Z);
  }
  for (i = width - width % WORD_SIZE; i < width; i += 1) {
    mov_mem8_reg(base, offset + i, reg_Z);
  }
}

int is_power_of_2(int n) {
  return n != 0 && (n & (n - 1)) == 0;
}

int power_of_2_log(int n) {
  int i = 0;
  while (n > 1) {
    n /= 2;
    i += 1;
  }
  return i;
}

void mul_for_pointer_arith(int reg, int width) {
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

void div_for_pointer_arith(int reg, int width) {
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

void jump_cond_reg_reg(int cond, int lbl, int reg1, int reg2);

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

int word_size_align(int n) {
  return (n + WORD_SIZE - 1) / WORD_SIZE * WORD_SIZE;
}

int align_to(int mul, int n) {
  return (n + mul - 1) / mul * mul;
}

void grow_stack(int words) {
  add_reg_imm(reg_SP, -words * WORD_SIZE);
}

// Like grow_stack, but takes bytes instead of words.
// To maintain alignment, the stack is grown by a multiple of WORD_SIZE (rounded
// up from the number of bytes).
void grow_stack_bytes(int bytes) {
  add_reg_imm(reg_SP, -word_size_align(bytes));
}

void rt_debug(char* msg);
void rt_crash(char* msg);

// Label definition

enum {
  GENERIC_LABEL,
  GOTO_LABEL,
};

#if defined (UNDEFINED_LABELS_ARE_RUNTIME_ERRORS) || defined (SAFE_MODE)
#define LABELS_ARR_SIZE 100000
int labels[LABELS_ARR_SIZE];
int labels_ix = 0;

#ifdef UNDEFINED_LABELS_ARE_RUNTIME_ERRORS
void def_label(int lbl);
#endif

void assert_all_labels_defined() {
  int i = 0;
  int lbl;
  // Check that all labels are defined
  for (; i < labels_ix; i++) {
    lbl = labels[i];
#ifdef UNDEFINED_LABELS_ARE_RUNTIME_ERRORS
    if (heap[lbl + 1] > 0) {
      if (heap[lbl] == GENERIC_LABEL && heap[lbl + 2] != 0) {
        def_label(lbl);
        rt_debug("Function or label is not defined\n");
        rt_debug("name = ");
        rt_debug((char*) heap[lbl + 2]);
        rt_debug("\n");
        // TODO: This should crash but let's just return for now to see how far we can get
        ret();
      }
    }
#else
    if (heap[lbl + 1] > 0) {
      putstr("Label ");
      if (heap[lbl] == GENERIC_LABEL && heap[lbl + 2] != 0) {
        putstr((char*) heap[lbl + 2]);
      } else {
        putint(lbl);
      }
      putstr(" is not defined\n");
      exit(1);
    }
#endif
  }
}

void add_label(int lbl) {
  if (labels_ix >= LABELS_ARR_SIZE) fatal_error("labels array is full");

  labels[labels_ix++] = lbl;
}

int alloc_label(char* name) {
  int lbl = alloc_obj(5);
  heap[lbl] = GENERIC_LABEL;
  heap[lbl + 1] = 0; // Address of label
  heap[lbl + 2] = (int) name; // Name of label
  heap[lbl + 3] = (int) fp_filepath;
  heap[lbl + 4] = line_number;
  add_label(lbl);
  return lbl;
}
#else

#define assert_all_labels_defined() // No-op
#define add_label(lbl) // No-op
#define alloc_label(name) alloc_label_()

int alloc_label_() {
  int lbl = alloc_obj(2);
  heap[lbl] = GENERIC_LABEL;
  heap[lbl + 1] = 0; // Address of label
  add_label(lbl);
  return lbl;
}
#endif

int alloc_goto_label() {
  int lbl = alloc_obj(3);
  heap[lbl] = GOTO_LABEL;
  heap[lbl + 1] = 0; // Address of label
  heap[lbl + 2] = 0; // cgc-fs of label
  add_label(lbl);
  return lbl;
}

void use_label(int lbl) {

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

void def_label(int lbl) {

  int addr = heap[lbl + 1];
  int label_addr = code_alloc;
  int next;

  if (heap[lbl] != GENERIC_LABEL) fatal_error("def_label expects generic label");

  if (addr < 0) {
#ifdef SAFE_MODE
    putstr("Label ");
    if (heap[lbl + 2] != 0) {
      putstr((char*) heap[lbl + 2]);
    } else {
      putint(lbl);
    }
    putstr(" previously defined at ");
    putstr((char*) heap[lbl + 3]);
#ifdef INCLUDE_LINE_NUMBER_ON_ERROR
    putstr(":");
    putint(heap[lbl + 4]);
#endif
    fatal_error(" being redefined");
#endif
  } else {
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
}

// Similar to use_label, but for gotos.
// The main difference is that it adjusts the stack and jumps, as opposed to
// simply emitting the address.
void jump_to_goto_label(int lbl) {

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

void def_goto_label(int lbl) {

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

ast dereference_type(ast type) {
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
bool is_pointer_type(ast type) {
  bool op = get_op(type);
  return op == '[' || op == '*';
}

bool is_function_type(ast type) {
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
#if WORD_SIZE == 8
      width = 8;
      break;
#elif defined (BOOTSTRAP_LONG)
      width = 4;
      break;
#else
      fatal_error("type_width: long type not supported");
      return -1;
#endif
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

#ifdef SAFE_MODE
int codegen_params(ast params, ast params_type, bool allow_extra_params) {
#else
int codegen_params(ast params) {
#endif

  int fs = 0;

  if (params != 0) {
#ifdef SAFE_MODE
    if (!allow_extra_params && params_type == 0) {
      fatal_error("codegen_params: Function expects less parameters than provided");
    }

    // Check that the number of parameters is correct
    if (params_type != 0) params_type = tail(params_type);
#endif

#ifdef SAFE_MODE
    fs = codegen_params(tail(params), params_type, allow_extra_params);
#else
    fs = codegen_params(tail(params));
#endif
    fs += codegen_param(car(params));
  }
  #ifdef SAFE_MODE
  else if (params_type != 0) {
    fatal_error("codegen_params: Function expects more parameters than provided");
  }
  #endif

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

#ifdef SAFE_MODE
  // Make sure fun has a type that can be called, either a function pointer or a function
  ast type = value_type(fun);
  if (!is_function_type(type)) {
    putstr("type="); putint(get_op(type)); putchar('\n');
    fatal_error("Called object is not a function or function pointer");
  }
  if (get_op(type) == '*') type = get_child_('*', type, 1); // Dereference function pointer
  // allow_extra_params is true if the function is called indirectly or if the function is variadic
  bool allow_extra_params = binding == 0;
  if (get_child_('(', type, 2)) allow_extra_params = true;
  nb_params = codegen_params(params, get_child_('(', type, 1), allow_extra_params);
#else
  nb_params = codegen_params(params);
#endif

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
#ifdef SUPPORT_64_BIT_LITERALS
          mov_reg_large_imm(reg_X, get_val(heap[binding+3]));
#else
          mov_reg_imm(reg_X, -get_val_(INTEGER, heap[binding+3]));
#endif
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

void add_params(ast params) {
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

void generate_exe() {
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

#ifdef SKIP

// deprecated... kept here in case it might be useful in the future

void inc_reg(int dst) { rex_prefix(dst, 0); emit_2_i8(0xff, 0xc0 + (dst & 7)); }
void dec_reg(int dst) { rex_prefix(dst, 0); emit_2_i8(0xff, 0xc8 + (dst & 7)); }
void xchg_reg_reg(int dst, int src) { op_reg_reg(0x87, dst, src); }
void not_reg(int dst) { rex_prefix(dst, 0); emit_2_i8(0xf7, 0xd0 + (dst & 7)); }
void neg_reg(int dst) { rex_prefix(dst, 0); emit_2_i8(0xf7, 0xd8 + (dst & 7)); }
void shr_reg_cl(int dst) { rex_prefix(dst, 0); emit_2_i8(0xd3, 0xe8 + (dst & 7)); }
void jump_cond_short(int cond, int n) { emit_2_i8(0x70 + cond, n); }

void test_reg_reg(int dst, int src) {

  // TEST dst_reg, src_reg ;; set Z condition flag based on result of dst_reg&src_reg
  // See: https://web.archive.org/web/20231004142335/https://www.felixcloutier.com/x86/test

  op_reg_reg(0x85, dst, src, WORD_SIZE);
}

#endif

void add_reg_reg(int dst, int src) {

  // ADD dst_reg, src_reg ;; dst_reg = dst_reg + src_reg
  // See: https://web.archive.org/web/20240407051903/https://www.felixcloutier.com/x86/add

  op_reg_reg(0x01, dst, src, WORD_SIZE);
}

void or_reg_reg (int dst, int src) {

  // OR dst_reg, src_reg ;; dst_reg = dst_reg | src_reg
  // See: https://web.archive.org/web/20231002205127/https://www.felixcloutier.com/x86/or

  op_reg_reg(0x09, dst, src, WORD_SIZE);
}

void and_reg_reg(int dst, int src) {

  // AND dst_reg, src_reg ;; dst_reg = dst_reg & src_reg
  // See: https://web.archive.org/web/20240228122102/https://www.felixcloutier.com/x86/and

  op_reg_reg(0x21, dst, src, WORD_SIZE);
}

void sub_reg_reg(int dst, int src) {

  // SUB dst_reg, src_reg ;; dst_reg = dst_reg - src_reg
  // See: https://web.archive.org/web/20240118202232/https://www.felixcloutier.com/x86/sub

  op_reg_reg(0x29, dst, src, WORD_SIZE);
}

void xor_reg_reg(int dst, int src) {

  // XOR dst_reg, src_reg ;; dst_reg = dst_reg ^ src_reg
  // See: https://web.archive.org/web/20240323052259/https://www.felixcloutier.com/x86/xor

  op_reg_reg(0x31, dst, src, WORD_SIZE);
}

void cmp_reg_reg(int dst, int src) {

  // CMP dst_reg, src_reg  ;; Set condition flags according to dst_reg-src_reg
  // See: https://web.archive.org/web/20240407051947/https://www.felixcloutier.com/x86/cmp
  // Note: For byte comparison, opcode is 0x38, for word/dword/qword comparison, opcode is 0x39

  op_reg_reg(0x39, dst, src, WORD_SIZE);
}

void mov_reg_reg(int dst, int src) {

  // MOV dst_reg, src_reg  ;; dst_reg = src_reg
  // See: https://web.archive.org/web/20240407051903/https://www.felixcloutier.com/x86/mov

  op_reg_reg(0x89, dst, src, WORD_SIZE);
}

void mov_reg_imm(int dst, int imm) {

  // MOV dst_reg, imm  ;; Move 32 bit immediate value to register
  // See: https://web.archive.org/web/20240407051903/https://www.felixcloutier.com/x86/mov

  rex_prefix(0, dst);
  emit_i8(0xb8 + (dst & 7));
#if WORD_SIZE == 4
  emit_i32_le(imm);
#elif WORD_SIZE == 8
  emit_i64_le(imm);
#else
  #error "mov_reg_imm: unknown word size"
#endif
}

#ifdef SUPPORT_64_BIT_LITERALS
void mov_reg_large_imm(int dst, int large_imm) {

  // MOV dst_reg, large_imm  ;; Move 32 bit or 64 bit immediate value to register
  // See: https://web.archive.org/web/20240407051903/https://www.felixcloutier.com/x86/mov

  rex_prefix(0, dst);
  emit_i8(0xb8 + (dst & 7));

#if WORD_SIZE == 4
  emit_i32_le_large_imm(large_imm);
#elif WORD_SIZE == 8
  emit_i64_le_large_imm(large_imm);
#else
  #error "mov_reg_large_imm: unknown word size"
#endif
}
#endif

void add_reg_imm(int dst, int imm) {

  // ADD dst_reg, imm  ;; Add 32 bit immediate value to register
  // See: https://web.archive.org/web/20240407051903/https://www.felixcloutier.com/x86/add

  rex_prefix(0, dst);
  emit_i8(0x81);
  mod_rm(0, dst);
  emit_i32_le(imm);
}

void add_reg_lbl(int dst, int lbl) {

  // ADD dst_reg, rel addr  ;; Add 32 bit displacement between next instruction and label to register
  // See: https://web.archive.org/web/20240407051903/https://www.felixcloutier.com/x86/add

  rex_prefix(0, dst);
  emit_i8(0x81);
  mod_rm(0, dst);
  use_label(lbl); // 32 bit placeholder for distance
}

void mov_memory(int op, int reg, int base, int offset, int reg_width) {

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

void mov_memory_extend(int op, int reg, int base, int offset, bool include_0f) {

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

void mov_mem8_reg(int base, int offset, int src) {

  // MOVB [base_reg + offset], src_reg  ;; Move byte from register to memory
  // See: https://web.archive.org/web/20240407051903/https://www.felixcloutier.com/x86/mov

#ifdef SAFE_MODE
  // The ModR/M byte cannot encode lower registers that are not AL, CL, DL, or BL
  if (src != AX && src != CX && src != DX && src != BX) {
    fatal_error("mov_mem8_reg: src must one of AX, CX, DX, BX");
  }
#endif

  mov_memory(0x88, src, base, offset, 1);
}

void mov_mem16_reg(int base, int offset, int src) {

  // MOVB [base_reg + offset], src_reg  ;; Move word (2 bytes) from register to memory
  // See: https://web.archive.org/web/20240407051903/https://www.felixcloutier.com/x86/mov

  mov_memory(0x89, src, base, offset, 2);
}

void mov_mem32_reg(int base, int offset, int src) {

  // MOVB [base_reg + offset], src_reg  ;; Move dword (4 bytes) from register to memory
  // See: https://web.archive.org/web/20240407051903/https://www.felixcloutier.com/x86/mov

  mov_memory(0x89, src, base, offset, 4);
}

void mov_mem64_reg(int base, int offset, int src) {

  // MOVB [base_reg + offset], src_reg  ;; Move qword (8 bytes) from register to memory
  // See: https://web.archive.org/web/20240407051903/https://www.felixcloutier.com/x86/mov

  mov_memory(0x89, src, base, offset, 8);
}

void mov_reg_mem8(int dst, int base, int offset) {

  // MOVB dst_reg, [base_reg + offset]  ;; Move byte from memory to register, zero-extended
  // See: https://web.archive.org/web/20250109202608/https://www.felixcloutier.com/x86/movzx

  mov_memory_extend(0xb6, dst, base, offset, true);
}

void mov_reg_mem16(int dst, int base, int offset) {

  // MOVB dst_reg, [base_reg + offset]  ;; Move word (2 bytes) from memory to register, zero-extended
  // See: https://web.archive.org/web/20250109202608/https://www.felixcloutier.com/x86/movzx

  mov_memory_extend(0xb7, dst, base, offset, true);
}

void mov_reg_mem32(int dst, int base, int offset) {

  // MOV dst_reg, [base_reg + offset]  ;; Move dword (4 bytes) from memory to register, zero-extended
  // See: https://web.archive.org/web/20240407051903/https://www.felixcloutier.com/x86/mov

  // Operations writing to the lower 32 bits of a register zero-extend the
  // result, so there's no movzx instruction for 32-bit operands.
  mov_memory(0x8b, dst, base, offset, 4);
}

void mov_reg_mem8_sign_ext(int dst, int base, int offset) {

  // MOVB dst_reg, [base_reg + offset]  ;; Move byte from memory to register, sign-extended
  // See: https://web.archive.org/web/20250121105942/https://www.felixcloutier.com/x86/movsx:movsxd

  mov_memory_extend(0xbe, dst, base, offset, true);
}

void mov_reg_mem16_sign_ext(int dst, int base, int offset) {

  // MOVB dst_reg, [base_reg + offset]  ;; Move word (2 bytes) from memory to register, sign-extended
  // See: https://web.archive.org/web/20250121105942/https://www.felixcloutier.com/x86/movsx:movsxd

  mov_memory_extend(0xbf, dst, base, offset, true);
}

void mov_reg_mem32_sign_ext(int dst, int base, int offset) {

  // MOV dst_reg, [base_reg + offset]  ;; Move dword (4 bytes) from memory to register, sign-extended
  // See: https://web.archive.org/web/20250121105942/https://www.felixcloutier.com/x86/movsx:movsxd

  mov_memory_extend(0x63, dst, base, offset, false);
}

void mov_reg_mem64(int dst, int base, int offset) {

  // MOV dst_reg, [base_reg + offset]  ;; Move qword (8 bytes) from memory to register
  // See: https://web.archive.org/web/20240407051903/https://www.felixcloutier.com/x86/mov

  mov_memory(0x8b, dst, base, offset, 8);
}

void imul_reg_reg(int dst, int src) {

  // IMUL dst_reg, src_reg ;; dst_reg = dst_reg * src_reg
  // See: https://web.archive.org/web/20240228122220/https://www.felixcloutier.com/x86/imul

  rex_prefix(dst, src);
  emit_2_i8(0x0f, 0xaf);
  mod_rm(dst, src);
}

void mul_reg_reg(int dst, int src) {

  // For our purposes, this is the same as imul_reg_reg.
  // https://web.archive.org/web/20240914145321/https://stackoverflow.com/questions/42587607/why-is-imul-used-for-multiplying-unsigned-numbers

  imul_reg_reg(dst, src);
}

void idiv_reg(int src) {

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

void idiv_reg_reg(int dst, int src) {

  // Computes dst_reg = dst_reg / src_reg
  // This is not an actual instruction on x86. The operation
  // is emulated with a sequence of instructions that will clobber the
  // registers AX and DX.

  mov_reg_reg(AX, dst);
  cdq_cqo(); // sign extend AX to DX:AX
  idiv_reg(src);
  mov_reg_reg(dst, AX);
}

void irem_reg_reg(int dst, int src) {

  // Computes dst_reg = dst_reg % src_reg
  // This is not an actual instruction on x86. The operation
  // is emulated with a sequence of instructions that will clobber the
  // registers AX and DX.

  mov_reg_reg(AX, dst);
  cdq_cqo(); // sign extend AX to DX:AX
  idiv_reg(src);
  mov_reg_reg(dst, DX);
}

void div_reg_reg(int dst, int src) {

  // Computes dst_reg = dst_reg / src_reg
  // This is not an actual instruction on x86. The operation
  // is emulated with a sequence of instructions that will clobber the
  // registers AX and DX.

#ifdef SAFE_MODE
  if (src == AX || src == DX) fatal_error("div_reg_reg: src cannot be AX");
#endif

  mov_reg_reg(AX, dst);
  mov_reg_imm(DX, 0); // Clear DX
  div_reg(src);
  mov_reg_reg(dst, AX);
}

void rem_reg_reg(int dst, int src) {

  // Computes dst_reg = dst_reg % src_reg
  // This is not an actual instruction on x86. The operation
  // is emulated with a sequence of instructions that will clobber the
  // registers AX and DX.

  mov_reg_reg(AX, dst);
  mov_reg_imm(DX, 0); // Clear DX
  div_reg(src);
  mov_reg_reg(dst, DX);
}

void s_l_reg_cl(int dst) {

  // SHL dst_reg, cl ;; dst_reg = dst_reg << cl (arithmetic or logical shift, they are the same)
  // See: https://web.archive.org/web/20240405194323/https://www.felixcloutier.com/x86/sal:sar:shl:shr

  op_reg_slash_digit(0xd3, 4, dst);
}

void s_l_reg_reg(int dst, int src) {

  // Computes dst_reg = dst_reg << src_reg (arithmetic or logical shift, they are the same)
  // This is not an actual instruction on x86. The operation
  // is emulated with a sequence of instructions that clobbers the
  // register CX, and does not work if dst = CX.

#ifdef SAFE_MODE
  if (dst == CX) fatal_error("s_l_reg_reg: dst cannot be CX");
#endif

  mov_reg_reg(CX, src);
  s_l_reg_cl(dst);
}

void sar_reg_cl(int dst) {

  // SAR dst_reg, cl ;; dst_reg = dst_reg >> cl (arithmetic shift)
  // See: https://web.archive.org/web/20240405194323/https://www.felixcloutier.com/x86/sal:sar:shl:shr

  op_reg_slash_digit(0xd3, 7, dst);
}

void sar_reg_reg(int dst, int src) {

  // Computes dst_reg = dst_reg >> src_reg (arithmetic shift)
  // This is not an actual instruction on x86. The operation
  // is emulated with a sequence of instructions that clobbers the
  // register CX, and does not work if dst = CX.

  mov_reg_reg(CX, src);
  sar_reg_cl(dst);
}

void shr_reg_cl(int dst) {

  // SHR dst_reg, cl ;; dst_reg = dst_reg >> cl (logical shift)
  // See: https://web.archive.org/web/20240405194323/https://www.felixcloutier.com/x86/sal:sar:shl:shr

  op_reg_slash_digit(0xd3, 5, dst);
}

void shr_reg_reg(int dst, int src) {

  // Computes dst_reg = dst_reg >> src_reg (logical shift)
  // This is not an actual instruction on x86. The operation
  // is emulated with a sequence of instructions that clobbers the
  // register CX, and does not work if dst = CX.

  mov_reg_reg(CX, src);
  shr_reg_cl(dst);
}

void push_reg(int src) {

  // PUSH src_reg  ;; Push word from register to stack
  // See: https://web.archive.org/web/20240407051929/https://www.felixcloutier.com/x86/push

  emit_i8(0x50 + src);
}

void pop_reg (int dst) {

  // POP dst_reg  ;; Pop word from stack to register
  // See: https://web.archive.org/web/20240204122208/https://www.felixcloutier.com/x86/pop

  emit_i8(0x58 + dst);
}

void jump(int lbl) {

  // JMP rel32  ;; Jump to 32 bit displacement relative to next instruction
  // See: https://web.archive.org/web/20240407051904/https://www.felixcloutier.com/x86/jmp

  emit_i8(0xe9);
  use_label(lbl);
}

void jump_rel(int offset) {

  // JMP rel32  ;; Jump to 32 bit displacement relative to next instruction
  // See: https://web.archive.org/web/20240407051904/https://www.felixcloutier.com/x86/jmp

  emit_i8(0xe9);
  emit_i32_le(offset);
}

void call(int lbl) {

  // CALL rel32  ;; Call to 32 bit displacement relative to next instruction
  // See: https://web.archive.org/web/20240323052931/https://www.felixcloutier.com/x86/call

  emit_i8(0xe8);
  use_label(lbl);
}

void call_reg(int reg) {

  // CALL reg  ;; Indirect call to address in register
  // See: https://web.archive.org/web/20240323052931/https://www.felixcloutier.com/x86/call

  emit_i8(0xff);
  mod_rm(2, reg);
}

void ret() {

  // RET  ;; Return to calling procedure
  // See: https://web.archive.org/web/20240302232015/https://www.felixcloutier.com/x86/ret

  emit_i8(0xc3);
}

void debug_interrupt() {

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

void jump_cond(int cond, int lbl) {

  // JE rel32, JNE rel32, JL rel32, JLE rel32, JG rel32, JGE rel32, ...
  // Jump conditionally to 32 bit displacement relative to next instruction
  // See: https://web.archive.org/web/20231007122614/https://www.felixcloutier.com/x86/jcc

  emit_2_i8(0x0f, 0x80 + cond);
  use_label(lbl);
}

void jump_cond_reg_reg(int cond, int lbl, int reg1, int reg2) {
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

void mov_reg_lbl(int reg, int lbl) {
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

int main(int argc, char **argv) {
  int i;
  ast decl;

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
