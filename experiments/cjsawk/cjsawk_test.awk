BEGIN {
  print "testing cjsawk"
  if(!my_fname) {
    my_fname = "test.c"
  }

  mk_args("./artifacts/cjsawk.exe " + fname+ " artifacts/out_awk.M1")
  print "argc: " argc;
  print "argv: " argv;

  main(argc, argv);
  exit 0
}
