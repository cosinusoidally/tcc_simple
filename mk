./mk_clean

echo "build with tcc"
tcc tcc_simple.c -o artifacts/tcc_simple_tcc.exe -ldl
./artifacts/tcc_simple_tcc.exe -nostdinc -c M2_simple_asm.c -o artifacts/M2_simple_asm_tcc.o

echo "build with gcc"
gcc -O0 -m32 -nostdinc tcc_simple.c -o artifacts/tcc_simple_gcc.exe -ldl
./artifacts/tcc_simple_gcc.exe -nostdinc -c M2_simple_asm.c -o artifacts/M2_simple_asm_gcc.o
sha256sum -c sums
