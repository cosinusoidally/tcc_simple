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

int o(int c) {
    while (neq(0, c)) {
        g(c);
        c = shr(c, 8);
    }
}

int gen_le32(int c) {
    g(c);
    g(shr(c, 8));
    g(shr(c, 16));
    g(shr(c, 24));
}

/* output a symbol and patch all calls to it */
int gsym_addr(int t, int a) {
    int ptr;
    int n;

    while (t) {
        ptr = add(cur_text_section->data, t);
        n = read32le(ptr); /* next value */
        write32le(ptr, a - t - 4);
        t = n;
    }
}

int gsym(int t) {
    gsym_addr(t, ind);
}

/* instruction + 4 bytes data. Return the address of the data */
int oad(int c, int s) {
    int t;
    o(c);
    t = ind;
    gen_le32(s);
    return t;
}

/* generate jmp to a label */
int gjmp2(instr,lbl) {
    return oad(instr,lbl);
}

/* output constant with relocation if 'r & VT_SYM' is true */
int gen_addr32(int r, Sym *sym, int c) {
    if (r & VT_SYM)
        greloc(cur_text_section, sym, ind, R_386_32);
    gen_le32(c);
}

/* generate a modrm reference. 'op_reg' contains the additional 3
   opcode bits */
int gen_modrm(int op_reg, int r, Sym *sym, int c) {
    op_reg = op_reg << 3;
    if ((r & VT_VALMASK) == VT_CONST) {
        /* constant memory reference */
        o(0x05 | op_reg);
        gen_addr32(r, sym, c);
    } else if ((r & VT_VALMASK) == VT_LOCAL) {
        /* currently, we use only ebp as base */
        if (c == (char)c) {
            /* short reference */
            o(0x45 | op_reg);
            g(c);
        } else {
            oad(0x85 | op_reg, c);
        }
    } else {
        g(0x00 | op_reg | (r & VT_VALMASK));
    }
}

/* load 'r' from value 'sv' */
int load(int r, SValue *sv) {
    int v, fc, fr;

    fr = sv->r;
    fc = sv->c.i;

    if (fr & VT_LVAL) {
        o(0x8b);     /* movl */
        gen_modrm(r, fr, sv->sym, fc);
    } else {
            o(0xb8 + r); /* mov $xx, r */
            gen_addr32(fr, sv->sym, fc);
    }
}

/* store register 'r' in lvalue 'v' */
int store(int r, SValue *v) {
    o(0x89);
    gen_modrm(r, v->r, v->sym, v->c.i);
}

int gadd_sp(int val) {
    if (val == (char)val) {
        o(0xc483);
        g(val);
    } else {
        oad(0xc481, val); /* add $xxx, %esp */
    }
}

/* 'is_jmp' is '1' if it is a jump */
int gcall_or_jmp(int is_jmp) {
    int r;
    /* constant and relocation case */
    greloc(cur_text_section, vtop->sym, ind + 1, R_386_PC32);
    oad(0xe8 + is_jmp, vtop->c.i - 4); /* call/jmp im */
}

/* Generate function call. The function address is pushed first, then
   all the parameters in call order. This functions pops all the
   parameters and the function address. */
int gfunc_call(int nb_args) {
    int r, args_size, i;
    Sym *func_sym;
    
    args_size = 0;
    for(i = 0;i < nb_args; i++) {
        r = gv(RC_INT);
        o(0x50 + r); /* push r */
        args_size += 4;
        vtop--;
    }
    save_regs(0); /* save used temporary registers */
    func_sym = vtop->type.ref;

    gcall_or_jmp(0);

    if (args_size)
        gadd_sp(args_size);
    vtop--;
}

/* generate function prolog of type 't' */
int gfunc_prolog(CType *func_type) {
    int addr, align, size;
    int param_index, param_addr;
    Sym *sym;
    CType *type;

    sym = func_type->ref;
    addr = 8;
    loc = 0;
    func_vc = 0;

    param_index = 0;

    ind = add(ind,9); /* FUNC_PROLOG_SIZE */
    func_sub_sp_offset = ind;
    /* define parameters */
    while ((sym = sym->next) != NULL) {
        type = &sym->type;
        size = type_size(type, &align);
        size = (size + 3) & ~3;
        param_addr = addr;
        addr += size;
        sym_push(sym->v & ~SYM_FIELD, type,
                 VT_LOCAL | lvalue_type(type->t), param_addr);
        param_index++;
    }
}

/* generate function epilog */
int gfunc_epilog(void) {
    addr_t v, saved_ind;

    /* align local size to word & save local variables */
    v = (-loc + 3) & -4;

    o(0xc9); /* leave */
    o(0xc3); /* ret */
    saved_ind = ind;
    ind = sub(func_sub_sp_offset, 9); /* - FUNC_PROLOG_SIZE */
    o(0xe58955);  /* push %ebp, mov %esp, %ebp */
    o(0xec81);  /* sub esp, stacksize */
    gen_le32(v);
    ind = saved_ind;
}

/* generate a jump to a label */
int gjmp(int t) {
    return gjmp2(0xe9, t);
}

/* generate a jump to a fixed address */
int gjmp_addr(int a) {
    int r;
    r = a - ind - 2;
    if (r == (char)r) {
        g(0xeb);
        g(r);
    } else {
        oad(0xe9, a - ind - 5);
    }
}

/* generate a test. set 'inv' to invert test. Stack entry is popped */
int gtst(int inv, int t) {
    g(0x0f);
    t = gjmp2((vtop->c.i - 16) ^ inv, t);
    vtop--;
    return t;
}

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
    o(0xc0 | (opc << 3) | r);
    g(c);
    vtop--;
    vtop->r = VT_CMP;
    vtop->c.i = op;
}

/* end of X86 code generator */
/*************************************************************/
