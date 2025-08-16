BEGIN {
  print "testing m0 awk"
  init_runtime();
  if(!my_fname) {
    exit 1
  }

  if(!my_fout) {
    exit 1
  }

  mk_args("./artifacts/m0.exe " my_fname " " my_fout)
  print "argc: " argc;
  print "argv: " argv;

  main(argc, argv);
  exit 0
}
