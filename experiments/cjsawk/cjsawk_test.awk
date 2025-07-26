BEGIN {
  print "testing cjsawk"
  init_runtime();
  if(!my_fname) {
    my_fname = "./artifacts/cjsawk_full.c";
  }

  if(!my_fout) {
    my_fout = "./artifacts/out_awk.M1";
  }

  mk_args("./artifacts/cjsawk.exe " my_fname " " my_fout)
  print "argc: " argc;
  print "argv: " argv;

  main(argc, argv);
  exit 0
}
