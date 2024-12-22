/* ================= start support_common.c =============================== */

int ri32(int o) {
        return or(or(and(ri8(o), 255),
                shl(and(ri8(add(o, 1)), 255), 8)),
                or(shl(and(ri8(add(o, 2)), 255), 16),
                shl(and(ri8(add(o, 3)), 255), 24)));
}

int wi32(int o, int v) {
  wi8(o, and(v, 255));
  v = shr(v, 8);
  wi8(add(o, 1), and(v, 255));
  v = shr(v, 8);
  wi8(add(o, 2), and(v, 255));
  v = shr(v, 8);
  wi8(add(o, 3), and(v, 255));
}

int ri16(int o) {
        return or(or(and(ri8(o), 255),
                shl(and(ri8(add(o, 1)), 255), 8)));
}

int wi16(int o, int v) {
  wi8(o, and(v, 255));
  v = shr(v, 8);
  wi8(add(o, 1), and(v, 255));
}

int v_esp; int v_ebp; int v_stack_size; int v_stack;

int init_c(){
  puts("init_c called");
  v_stack_size=mul(64,1024);
  v_stack=calloc(1,v_stack_size);
  v_esp=sub(add(v_stack,v_stack_size),4);
  v_ebp=v_esp;
}

int v_alloca(int x) {
  /* round allocations to nearest 16 to avoid weird crashes */
  x = add(16, and(x,not(15)));
  v_esp=sub(v_esp, x);
  return v_esp;
}

int enter() {
/* FIXME detect overflow */
  v_esp=sub(v_esp, 4);
  wi32(v_esp,v_ebp);
  v_ebp=v_esp;
}

int leave(int x) {
  v_esp=v_ebp;
  v_ebp=ri32(v_esp);
  v_esp=add(v_esp, 4);
  return x;
}

/* ================= end support_common.c ================================= */

