int memcpy(int s1, int s2, int n);
int process_file(int name);

int l_size;
int ob;
int obo;

int gib;
int gibo;

int gdb;
int gdbo;

int db;
int dbo;

int infile;
int outfd;

int wu8(int o, int v) {
  wi8(o, v);
}

int init_globals(void){
  l_size=4096;
  ob=calloc(mul(1024, 1024),1);
  obo=0;
  gib=calloc(mul(1024, 1024),1);
  gibo=0;
  gdb=calloc(mul(1024, 1024),1);
  gdbo=0;
  db=calloc(mul(1024, 1024),1);
  dbo=0;
}

int oputs(int s){
  int l;
  l=strlen(s);
  memcpy(add(ob, obo),s,l);
  obo = add(obo, l);
}

int giputs(int s){
  int l;
  l=strlen(s);
  memcpy(add(gib, gibo),s,l);
  gibo = add(gibo, l);
}

int gdputs(int s){
  int l;
  l=strlen(s);
  memcpy(add(gdb, gdbo),s,l);
  gdbo = add(gdbo, l);
}

int dbputs(int s){
  int l;
  l=strlen(s);
  memcpy(add(db, dbo),s,l);
  dbo = add(dbo, l);
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
/* FIXME ljw not quite right */
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


int process_global_var(int l){
  int p;
  int c;
  l=add(l, 4);
  p=l;
  while(neq((c=ri8(p)), 0)){
    if(eq(c,'=')){
      giputs("    ");
      giputs(l);
      giputs("\n");
      wi8(p, ';');
      wi8(add(p, 1), 0);
      break;
    }
    p=p+1;
  }
  gdputs("int ");
  gdputs(l);
  gdputs("\n");
}

int process_local_var(int l,int n){
  int i;
  i = 0;
  while(lt(i, n)){
    oputs(" ");
    i = add(i, 1);
  }
  oputs("int ");
  oputs(l);
  oputs("\n");
}

int process_load(int l){
  int p;
  p=l;

  while(neq(ri8(p), '"')){
    p = add(p, 1);
  }
  p=p+1;
  l=p;
  while(neq(ri8(p), '"')){
    p = add(p, 1);
  }
  wi8(p, 0);
  if(eq(memcmp(l,"support.js",10), 0)){
    return;
  }
  oputs("/* load: ");
  oputs(l);
  oputs(" */");
  oputs("\n");
  process_file(l);
}

int eat_whitespace(int l){
  int o;
  o=l;
  while(eq(memcmp(" ",o,1), 0)){
    o = add(o, 1);
  }
  return o;
}

int num_whitespace(int l){
  int o;
  o=l;
  while(eq(memcmp(" ",o,1), 0)){
    o = add(o, 1);
  }
  return sub(o, l);
}

int process_function(int l){
  char *p;
  int *args;
  int args_n;
  int c;
  int i;
  int name;
  args_n=0;
  int decl;
  p=l;
  /* FIXME should be dynamic */
  args=malloc(mul(20, 4));
  oputs("/* ");
  oputs(l);
  oputs(" */");
  oputs("\n");
  l = add(l, 9);
  decl = add(db, dbo);
  dbputs("int ");
  name=l;
  while(neq(ri8(p), '(')){
    p = add(p, 1);
  }
  wi8(p, 0);
  p = add(p, 1);
  wi32(add(args, 4), p);
  while(neq(ri8(p), ')')){
    c = ri8(p);
    if(eq(args_n, 0)){
      args_n=1;
    }
    if(eq(c, ',')){
      args_n = add(args_n, 1);
      wi8(p, 0);
      p = add(p, 1);
      wi32(add(args, mul(args_n, 4)), p);
    } else {
      p = add(p, 1);
    }
  }
  wi8(p, 0);
  p = add(p, 1);
  dbputs(name);
  dbputs("(");
  if(eq(args_n, 0)){
/* breaks m2min_v3.js */
/*    dbputs("void"); */
  } else {
    dbputs("int ");
    dbputs(ri32(add(args, 4)));
  }
  i = 1;
  while(lt(i, args_n)) {
    dbputs(", int ");
    dbputs(ri32(add(args, mul(add(i, 1), 4))));
    i = add(i, 1);
  }
  dbputs(")");
  oputs(decl);
  oputs(" {\n");
  dbputs(";\n");
  free(args);
}

int strip_comments(int l) {
  int i;
  char *s;
  s = l;
  i = 0;
  while(1) {
    if(eq(ri8(add(s, i)), 0)) {
      return;
    }
    if(and(eq(ri8(add(s, i)), 47), eq(ri8(add(s, add(i,1))), 47))) {
      wi8(add(s, i), 0);
      return;
    }
    i = add(i, 1);
  }
}

int process_line(int l) {
  int t;
  int n;
  strip_comments(l);
  if(eq(memcmp("var ",l,4), 0)){
    process_global_var(l);
    return;
  }
  if(eq(memcmp("load(",l,4), 0)){
    process_load(l);
    return;
  }
  if(eq(memcmp("function ",l,9), 0)){
    process_function(l);
    return;
  }
  t=eat_whitespace(l);
  n=num_whitespace(l);
  if(eq(memcmp("// ",t,3), 0)){
    return;
  }
  if(eq(memcmp("print(",t,6), 0)){
    return;
  }
  if(eq(memcmp("var ",t,4), 0)){
    process_local_var(add(t, 4),n);
    return;
  }
  oputs(l);
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
      wu8(lb+lo,0);
      lo=0;
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

int print_fn_decls(void){
  fputs("\n",outfd);
  fputs("/* fn decls */",outfd);
  fputs("\n",outfd);
  fputs(db, outfd);
}

int print_declare_globals(void){
  fputs("/* declare globals */\n",outfd);
  fputs(gdb, outfd);
}

int print_converted() {
  fwrite(ob,1,obo,outfd);
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

  print_fn_decls();
  print_declare_globals();
  print_converted();
  
  return 0;
}
