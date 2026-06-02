int memcpy(int s1, int s2, int n);
int process_file(int name);

int l_size;
int ob;
int obo;

int db;
int dbo;

int infile;
int outfd;

int wu8(int o, int v) {
  wi8(o, v);
}

int init_globals(){
  l_size=4096;
  ob=calloc(mul(1024, 1024),1);
  obo=0;
  db=calloc(mul(256, 1024),1);
  dbo=0;
}

int oputc(int c) {
  wi8(add(ob, obo), c);
  obo = add(obo, 1);
  wi8(add(ob, obo), 0);
}

int oputs(int s){
  int l;
  l=strlen(s);
  memcpy(add(ob, obo),s,l);
  obo = add(obo, l);
  wi8(add(ob, obo), 0);
}

int dbputc(int c) {
  wi8(add(db, dbo), c);
  dbo = add(dbo, 1);
  wi8(add(db, dbo), 0);
}

int dbputs(int s){
  int l;
  l=strlen(s);
  memcpy(add(db, dbo),s,l);
  dbo = add(dbo, l);
  wi8(add(db, dbo), 0);
}

int memcmp(int s1, int s2, int n) {
  int i;
  int r;
  int p1;
  int p2;
  r=0;
  p1=s1;
  p2=s2;
  i = 0;
  while(lt(i, n)){
    if(neq(ri8(add(p1, i)), ri8(add(p2, i)))){
      r=1;
    }
    i = add(i, 1);
  }
  return r;
}

int fwrite(int ptr,int size, int nitems, int stream) {
  int t;
  int c;
  t = mul(size, nitems);
  c=ptr;
  while(gt(t, 0)){
    fputc(ri8(c),stream);
    t=sub(t, 1);
    c=add(c, 1);
  }
}

int emit_decimal(int n) {
  int buf;
  int i;
  int q;
  int digit;
  if(eq(n, 0)) {
    oputc('0');
    return;
  }
  buf = malloc(16);
  i = 0;
  while(gt(n, 0)) {
    q = div(n, 10);
    digit = sub(n, mul(q, 10));
    wi8(add(buf, i), add(digit, '0'));
    i = add(i, 1);
    n = q;
  }
  while(gt(i, 0)) {
    i = sub(i, 1);
    oputc(ri8(add(buf, i)));
  }
}

int escaped_char_value(int c) {
  if(eq(c, 'n')) return 10;
  if(eq(c, 't')) return 9;
  if(eq(c, 'r')) return 13;
  if(eq(c, 'f')) return 12;
  if(eq(c, 'v')) return 11;
  if(eq(c, '"')) return 34;
  if(eq(c, '\\')) return 92;
  if(eq(c, '0')) return 0;
  return c;
}

int streq_at(int s, int pos, int pat) {
  int i;
  i = 0;
  while(neq(ri8(add(pat, i)), 0)) {
    if(neq(ri8(add(s, add(pos, i))), ri8(add(pat, i)))) {
      return 0;
    }
    i = add(i, 1);
  }
  return 1;
}

int emit_mkC_number(int line, int pos) {
  int i;
  int c;
  i = add(pos, 5);
  c = ri8(add(line, i));
  if(eq(c, '\\')) {
    i = add(i, 1);
    c = escaped_char_value(ri8(add(line, i)));
  }
  emit_decimal(c);
  i = add(i, 1);
  while(neq(ri8(add(line, i)), 0)) {
    if(and(eq(ri8(add(line, i)), '"'), eq(ri8(add(line, add(i, 1))), ')'))) {
      return add(i, 2);
    }
    i = add(i, 1);
  }
  return i;
}

int emit_mks_string(int line, int pos) {
  int i;
  int c;
  oputs("mks(");
  oputc('"');
  i = add(pos, 5);
  while(neq(ri8(add(line, i)), 0)) {
    c = ri8(add(line, i));
    if(eq(c, '"')) {
      oputc('"');
      return add(i, 1);
    }
    if(and(eq(c, '\\'), eq(ri8(add(line, add(i, 1))), 'n'))) {
      i = add(i, 2);
    } else {
      oputc(c);
      i = add(i, 1);
    }
  }
  return i;
}

int oputs_converted(int l) {
  int i;
  i = 0;
  while(neq(ri8(add(l, i)), 0)) {
    if(eq(streq_at(l, i, "brk(n)"), 1)) {
      oputs("malloc(n)");
      i = add(i, 6);
    } else if(and(eq(streq_at(l, i, "mkC("), 1), eq(ri8(add(l, add(i, 4))), '"'))) {
      i = emit_mkC_number(l, i);
    } else if(and(eq(streq_at(l, i, "mks("), 1), eq(ri8(add(l, add(i, 4))), '"'))) {
      i = emit_mks_string(l, i);
    } else {
      oputc(ri8(add(l, i)));
      i = add(i, 1);
    }
  }
}

int is_main_name(int name, int end) {
  if(neq(sub(end, name), 4)) return 0;
  if(neq(ri8(name), 'm')) return 0;
  if(neq(ri8(add(name, 1)), 'a')) return 0;
  if(neq(ri8(add(name, 2)), 'i')) return 0;
  if(neq(ri8(add(name, 3)), 'n')) return 0;
  return 1;
}

int emit_name(int name, int end) {
  int i;
  if(eq(is_main_name(name, end), 1)) {
    oputs("mawkcc_main");
    return;
  }
  i = name;
  while(lt(i, end)) {
    oputc(ri8(i));
    i = add(i, 1);
  }
}

int emit_decl_name(int name, int end) {
  int i;
  if(eq(is_main_name(name, end), 1)) {
    dbputs("mawkcc_main");
    return;
  }
  i = name;
  while(lt(i, end)) {
    dbputc(ri8(i));
    i = add(i, 1);
  }
}

int emit_typed_args(int args) {
  int i;
  int start;
  int end;
  int count;
  i = 0;
  count = 0;
  while(eq(ri8(add(args, i)), ' ')) {
    i = add(i, 1);
  }
  if(eq(ri8(add(args, i)), 0)) {
    return;
  }
  while(neq(ri8(add(args, i)), 0)) {
    while(eq(ri8(add(args, i)), ' ')) {
      i = add(i, 1);
    }
    start = i;
    while(and(neq(ri8(add(args, i)), 0), neq(ri8(add(args, i)), ','))) {
      i = add(i, 1);
    }
    end = i;
    while(and(gt(end, start), eq(ri8(add(args, sub(end, 1))), ' '))) {
      end = sub(end, 1);
    }
    if(gt(count, 0)) {
      oputs(", ");
    }
    oputs("int ");
    while(lt(start, end)) {
      oputc(ri8(add(args, start)));
      start = add(start, 1);
    }
    count = add(count, 1);
    if(eq(ri8(add(args, i)), ',')) {
      i = add(i, 1);
    }
  }
}

int emit_decl_typed_args(int args) {
  int i;
  int start;
  int end;
  int count;
  i = 0;
  count = 0;
  while(eq(ri8(add(args, i)), ' ')) {
    i = add(i, 1);
  }
  if(eq(ri8(add(args, i)), 0)) {
    return;
  }
  while(neq(ri8(add(args, i)), 0)) {
    while(eq(ri8(add(args, i)), ' ')) {
      i = add(i, 1);
    }
    start = i;
    while(and(neq(ri8(add(args, i)), 0), neq(ri8(add(args, i)), ','))) {
      i = add(i, 1);
    }
    end = i;
    while(and(gt(end, start), eq(ri8(add(args, sub(end, 1))), ' '))) {
      end = sub(end, 1);
    }
    if(gt(count, 0)) {
      dbputs(", ");
    }
    dbputs("int ");
    while(lt(start, end)) {
      dbputc(ri8(add(args, start)));
      start = add(start, 1);
    }
    count = add(count, 1);
    if(eq(ri8(add(args, i)), ',')) {
      i = add(i, 1);
    }
  }
}

int process_global_var(int l){
  oputs("int ");
  oputs_converted(add(l, 4));
  oputs("\n");
}

int process_function(int l){
  int p;
  int name;
  int name_end;
  int args;
  p=l;
  l = add(l, 9);
  name=l;
  while(neq(ri8(p), '(')){
    p = add(p, 1);
  }
  name_end = p;
  wi8(p, 0);
  p = add(p, 1);
  args = p;
  while(neq(ri8(p), ')')){
    p = add(p, 1);
  }
  wi8(p, 0);
  p = add(p, 1);

  dbputs("int ");
  emit_decl_name(name, name_end);
  dbputs("(");
  emit_decl_typed_args(args);
  dbputs(");\n");

  oputs("int ");
  emit_name(name, name_end);
  oputs("(");
  emit_typed_args(args);
  oputs(")");
  oputs_converted(p);
  oputs("\n");
}

int process_line(int l) {
  if(eq(memcmp("var ",l,4), 0)){
    process_global_var(l);
    return;
  }
  if(eq(memcmp("function ",l,9), 0)){
    process_function(l);
    return;
  }
  oputs_converted(l);
  oputs("\n");
}

int process_file(int name){
  int f;
  int c;
  int lb;
  int lo;
  lb=malloc(l_size);
  f=fopen(name,"rb");
  lo=0;
  while(neq((c=fgetc(f)), sub(0, 1))) {
    wu8(add(lb, lo),c);
    if(eq(c, '\n')){
      wu8(add(lb, lo),0);
      lo = 0;
      process_line(lb);
    } else {
      lo = add(lo, 1);
    }
  }
  free(lb);
  oputs("\n");
  fclose(f);
  return 0;
}

int print_primitives(){
  fputs("int ADD(int a, int b); int SUB(int a, int b); int MUL(int a, int b);\n",outfd);
  fputs("int DIV(int a, int b); int MOD(int a, int b); int NEG(int a);\n",outfd);
  fputs("int NOT(int a); int EQ(int a, int b); int NE(int a, int b);\n",outfd);
  fputs("int LT(int a, int b); int LE(int a, int b); int GT(int a, int b);\n",outfd);
  fputs("int GE(int a, int b); int AND(int a, int b); int OR(int a, int b);\n",outfd);
  fputs("int XOR(int a, int b); int SHL(int a, int b); int SHR(int a, int b);\n",outfd);
  fputs("int ri8(int p); int wi8(int p, int v); int ri32(int p); int wi32(int p, int v);\n",outfd);
  fputs("int malloc(int n); int open(int p, int f, int m); int read(int fd, int buf, int len);\n",outfd);
  fputs("int write(int fd, int buf, int len); int close(int fd); int exit(int status);\n",outfd);
  fputs("int mks(int s); int mkC(int s);\n",outfd);
}

int print_fn_decls(){
  fputs(db, outfd);
}

int print_converted() {
  fwrite(ob,1,obo,outfd);
}

int print_wrapper() {
  fputs("int main(int envp, int argv) {\n", outfd);
  fputs("    int argc;\n", outfd);
  fputs("    argc = 0;\n", outfd);
  fputs("    while (NE(ri32(ADD(argv, MUL(argc, 4))), 0)) {\n", outfd);
  fputs("        argc = ADD(argc, 1);\n", outfd);
  fputs("    }\n", outfd);
  fputs("    return mawkcc_main(argc, argv);\n", outfd);
  fputs("}\n", outfd);
}

int main(int argc, int argv) {
  init_c();
  init_globals();
  if(eq(argc, 3)){
    infile = ri32(add(argv, 4));
    outfd=fopen(ri32(add(argv, 8)),"wb");
  } else {
    exit(1);
  }
  process_file(infile);

  print_primitives();
  print_fn_decls();
  print_converted();
  print_wrapper();

  return 0;
}
