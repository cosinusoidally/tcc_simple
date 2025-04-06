#ifdef TEST_1
/* syntax error */

int foo(x, y) {
  return 1;
#endif

#ifdef TEST_2
// identifier not found
int blah(x) {
  int i;
  for(i0;i<100;i++) {
  }
}

#endif

#ifdef TEST_3

#warning "This is a warning"
#error "This is an error"

#endif
