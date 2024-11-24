./mk_clean

echo "build with tcc24"
tcc ../tcc_bootstrap_alt/tcc_24/tcc.o ../tcc_bootstrap_alt/tcc_24/libtcc1.o -o artifacts/tcc24 -ldl
./artifacts/tcc24 -c tcc_simple.c -o artifacts/tcc_simple_tcc24.o
tcc artifacts/tcc_simple_tcc24.o ../tcc_bootstrap_alt/tcc_24/libtcc1.o -o artifacts/tcc_simple_tcc24.exe -ldl
./artifacts/tcc_simple_tcc24.exe -nostdinc -c M2_simple_asm.c -o artifacts/M2_simple_asm_tcc24.o

echo "build with tcc"
tcc tcc_simple.c -o artifacts/tcc_simple_tcc.exe -ldl
./artifacts/tcc_simple_tcc.exe -nostdinc -c M2_simple_asm.c -o artifacts/M2_simple_asm_tcc.o

echo "build with gcc"
gcc -O0 -m32 -nostdinc tcc_simple.c -o artifacts/tcc_simple_gcc.exe -ldl
./artifacts/tcc_simple_gcc.exe -nostdinc -c M2_simple_asm.c -o artifacts/M2_simple_asm_gcc.o
wc tcc_simple.c
sha256sum -c sums
