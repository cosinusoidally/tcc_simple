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

int init_c(){
//  puts("init_c called");
}

/* ================= end support_common.c ================================= */

