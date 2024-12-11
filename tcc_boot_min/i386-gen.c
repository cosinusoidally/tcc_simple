/*
 *  X86 code generator for TCC
 * 
 *  Copyright (c) 2001-2004 Fabrice Bellard
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

extern int func_sub_sp_offset;

int init_reg_classes() {
  wi32(reg_classes, or(RC_INT, RC_EAX));
  wi32(add(reg_classes, 4), or(RC_INT, RC_ECX));
}

/* 10 */
/* load 'r' from value 'sv' */
int load(int r, SValue *sv) {
    int v;
    int fc;
    int fr;

    fr = sv->r;
    fc = sv->c.i;

    if (and(fr, VT_LVAL)) {
        o(0x8b);     /* movl */
        gen_modrm(r, fr, sv->sym, fc);
    } else {
            o(add(0xb8, r)); /* mov $xx, r */
            gen_addr32(fr, sv->sym, fc);
    }
}

/* 11 */
/* store register 'r' in lvalue 'v' */
int store(int r, SValue *v) {
    o(0x89);
    gen_modrm(r, v->r, v->sym, v->c.i);
}

/* 13 */
/* 'is_jmp' is '1' if it is a jump */
int gcall_or_jmp(int is_jmp) {
    int r;

    /* constant and relocation case */
    greloc(cur_text_section, vtop->sym, add(ind, 1), R_386_PC32);
    oad(add(0xe8, is_jmp), sub(vtop->c.i, 4)); /* call/jmp im */
}

/* 15 */
/* generate function prolog of type 't' */
int gfunc_prolog(CType *func_type) {
    int addr;
    int align;
    int size;
    int param_addr;
    Sym *sym;
    CType *type;

    enter();
    align = v_alloca(4);

    sym = func_type->ref;
    addr = 8;
    loc = 0;

    ind = add(ind, FUNC_PROLOG_SIZE);
    func_sub_sp_offset = ind;
    /* define parameters */
    while (neq((sym = sym->next), 0)) {
        type = &sym->type;
        size = type_size(type, align);
        size = and(add(size, 3), not(3));
        param_addr = addr;
        addr = add(addr, size);
        sym_push(and(sym->v , not(SYM_FIELD)), type,
                 or(VT_LOCAL, lvalue_type(type->t)), param_addr);
    }
    leave(0);
}

/* 19 */
/* generate a test. set 'inv' to invert test. Stack entry is popped */
int gtst(int inv, int t) {
    g(0x0f);
    t = gjmp2(xor(sub(vtop->c.i, 16), inv), t);
    vtop = sub(vtop, sizeof_SValue);
    return t;
}

/* 20 */
/* generate an integer binary operation */
int gen_opi(int op) {
    int r, fr, opc, c;

    opc = 7;
    /* constant case */
    vswap();
    r = gv(RC_INT);
    vswap();
    c = vtop->c.i;
    o(0x83);
    o(or(or(0xc0, shl(opc, 3)), r));
    g(c);
    vtop = sub(vtop, sizeof_SValue);
    vtop->r = VT_CMP;
    vtop->c.i = op;
}

/* end of X86 code generator */
/*************************************************************/
