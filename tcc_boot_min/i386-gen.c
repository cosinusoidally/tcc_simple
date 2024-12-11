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

ST_DATA const int reg_classes[NB_REGS] = {
    /* eax */ RC_INT | RC_EAX,
    /* ecx */ RC_INT | RC_ECX,
};

static unsigned long func_sub_sp_offset;

/* 1 */
/* XXX: make it faster ? */
int g(int c) {
    int ind1;
    ind1 = add(ind, 1);
    if (gt(ind1, cur_text_section->data_allocated)) {
        section_realloc(cur_text_section, ind1);
    }
    wi8(add(cur_text_section->data, ind), c);
    ind = ind1;
}

/* 4 */
/* output a symbol and patch all calls to it */
int gsym_addr(int t, int a) {
    int ptr;
    int n;

    while (t) {
        ptr = add(cur_text_section->data, t);
        n = read32le(ptr); /* next value */
        write32le(ptr, sub(sub(a, t), 4));
        t = n;
    }
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

/* 12 */
int gadd_sp(int val) {
    if (lt(and(val,255), 256)) {
        o(0xc483);
        g(val);
    } else {
        oad(0xc481, val); /* add $xxx, %esp */
    }
}

/* 13 */
/* 'is_jmp' is '1' if it is a jump */
int gcall_or_jmp(int is_jmp) {
    int r;

    /* constant and relocation case */
    greloc(cur_text_section, vtop->sym, add(ind, 1), R_386_PC32);
    oad(add(0xe8, is_jmp), sub(vtop->c.i, 4)); /* call/jmp im */
}

/* 14 */
/* Generate function call. The function address is pushed first, then
   all the parameters in call order. This functions pops all the
   parameters and the function address. */
int gfunc_call(int nb_args) {
    int r;
    int args_size;
    int i;
    Sym *func_sym;
    
    args_size = 0;
    i = 0;
    while(lt(i, nb_args)) {
        r = gv(RC_INT);
        o(add(0x50, r)); /* push r */
        args_size = add(args_size, 4);
        vtop = sub(vtop, sizeof_SValue);
        i = add(i, 1);
    }
    save_regs(0); /* save used temporary registers */
    func_sym = vtop->type.ref;

    gcall_or_jmp(0);

    if (neq(args_size, 0)) {
        gadd_sp(args_size);
    }
    vtop = sub(vtop, sizeof_SValue);
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

/* 16 */
/* generate function epilog */
int gfunc_epilog() {
    int v;
    int saved_ind;

    /* align local size to word & save local variables */
    v = and(add(sub(0,loc), 3), sub(0,4));

    o(0xc9); /* leave */
    o(0xc3); /* ret */
    saved_ind = ind;
    ind = sub(func_sub_sp_offset, FUNC_PROLOG_SIZE);
    o(0xe58955);  /* push %ebp, mov %esp, %ebp */
    o(0xec81);  /* sub esp, stacksize */
    gen_le32(v);
    ind = saved_ind;
}

/* 17 */
/* generate a jump to a label */
int gjmp(int t) {
    return gjmp2(0xe9, t);
}

/* 18 */
/* generate a jump to a fixed address */
int gjmp_addr(int a) {
    int r;
    r = sub(sub(a, ind), 2);
    if (eq(r, (char)r)) {
        g(0xeb);
        g(r);
    } else {
        oad(0xe9, sub(sub(a, ind), 5));
    }
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
