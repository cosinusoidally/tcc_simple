extern int func_sub_sp_offset;

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
    g(15); /* 0x0f */
    t = gjmp2(xor(sub(gcv_i(gsv_c(vtop)), 16), inv), t);
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
    c = gcv_i(gsv_c(vtop));
    o(131); /* 0x83 */
    o(or(or(192, shl(opc, 3)), r)); /* 0xc0 */
    g(c);
    vtop = sub(vtop, sizeof_SValue);
    vtop->r = VT_CMP;
    vtop->c.i = op;
}
