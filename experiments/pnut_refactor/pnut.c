#include "support.c"
#include "support_common.c"

#define function int
#define var int

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
function jump_cond_reg_reg(cond, lbl, reg1, reg2);

function os_exit();
function os_allocate_memory(size);
function os_read();
function os_write();
function os_open();
function os_close();
function os_seek();
function os_unlink();
function os_mkdir();
function os_chmod();
function os_access();
function rt_putchar();
function rt_debug(msg);
function rt_crash(msg);
function setup_proc_args(global_vars_size);
function alloc_label_();
function struct_union_size(struct_type);
function codegen_rvalue(node);
function codegen_statement(node);
function codegen_lvalue(node);
function handle_enum_struct_union_type_decl(type);

#include "pnut.js"
