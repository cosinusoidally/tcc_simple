BEGIN {
  print "testing cjsawk"
  init_runtime();
  if(!my_fname) {
    my_fname = "test.c"
  }

  mk_args("./artifacts/cjsawk.exe " my_fname " artifacts/out_awk.M1")
  print "argc: " argc;
  print "argv: " argv;

  main(argc, argv);
  exit 0
}
