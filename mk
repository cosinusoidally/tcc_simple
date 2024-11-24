./mk_clean
tcc tcc_simple.c -o artifacts/tcc_simple_tcc.exe -ldl
./artifacts/tcc_simple_tcc.exe -c M2_simple_asm.c -o artifacts/M2_simple_asm_tcc.o
sha256sum -c sums
