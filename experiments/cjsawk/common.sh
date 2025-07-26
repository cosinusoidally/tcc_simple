export PATH=$PWD/artifacts/deps/js_to_c/:$PWD/artifacts/deps/:$PATH

function compile() {
  $1 $2 $3.M1
  cat ../../../../m2min_v3/simple_asm_defs.M1 ../../../../m2min_v3/x86_defs.M1 ../../../../m2min_v3/libc-core.M1 $3.M1 > $3-0.M1
  M0 $3-0.M1 $3.hex2
  cat ../../../../m2min_v3/ELF-i386.hex2 $3.hex2 > $3-0.hex2
  hex2-0 $3-0.hex2 $3
}

function assemble_link() {
  cat ../../../../m2min_v3/simple_asm_defs.M1 ../../../../m2min_v3/x86_defs.M1 ../../../../m2min_v3/libc-core.M1 $1.M1 > $1-0.M1
  M0 $1-0.M1 $1.hex2
  cat ../../../../m2min_v3/ELF-i386.hex2 $1.hex2 > $1-0.hex2
  hex2-0 $1-0.hex2 $1
}
