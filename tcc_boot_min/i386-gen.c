extern int func_sub_sp_offset;

/* 15 */
/* generate function prolog of type 't' */
int gfunc_prolog(CType *func_type) {
    int addr;
    int align;
    int size;
    int param_addr;
    Sym *sym;
    int type;

    enter();
    align = v_alloca(4);

    sym = gct_ref(func_type);
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
        sym_push(and(gsym_v(sym) , not(SYM_FIELD)), type,
                 or(VT_LOCAL, lvalue_type(gct_t(type))), param_addr);
    }
    leave(0);
}
