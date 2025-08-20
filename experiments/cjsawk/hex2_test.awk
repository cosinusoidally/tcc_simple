function chmod(a,b) {
# dummy for now
}

function fputc(a,b) {
  v_fputc(a, b);
}

BEGIN {
  print "testing hex2 awk"
  init_runtime();
  if(!my_fname) {
    exit 1
  }

  if(!my_fout) {
    exit 1
  }

  mk_args("./artifacts/hex2.exe " my_fname " " my_fout)
  print "argc: " argc;
  print "argv: " argv;

  main(argc, argv);
  exit 0
}
