int eq(int a, int b){
        asm(
                "lea_eax,[ebp+DWORD] %-4"
                "mov_eax,[eax]"
                "push_eax"
                "lea_eax,[ebp+DWORD] %-8"
                "mov_eax,[eax]"
                "pop_ebx"
                "cmp"
                "sete_al"
                "movzx_eax,al"
                "ret"
        );
}

int neq(int a, int b){
        asm(
                "lea_eax,[ebp+DWORD] %-4"
                "mov_eax,[eax]"
                "push_eax"
                "lea_eax,[ebp+DWORD] %-8"
                "mov_eax,[eax]"
                "pop_ebx"
                "cmp"
                "setne_al"
                "movzx_eax,al"
                "ret"
        );
}

int lt(int a, int b){
        asm(
                "lea_eax,[ebp+DWORD] %-4"
                "mov_eax,[eax]"
                "push_eax"
                "lea_eax,[ebp+DWORD] %-8"
                "mov_eax,[eax]"
                "pop_ebx"
                "cmp"
                "setl_al"
                "movzx_eax,al"
                "ret"
        );
}

int gt(int a, int b){
        asm(
                "lea_eax,[ebp+DWORD] %-4"
                "mov_eax,[eax]"
                "push_eax"
                "lea_eax,[ebp+DWORD] %-8"
                "mov_eax,[eax]"
                "pop_ebx"
                "cmp"
                "setg_al"
                "movzx_eax,al"
                "ret"
        );
}

int gte(int a, int b){
        asm(
                "lea_eax,[ebp+DWORD] %-4"
                "mov_eax,[eax]"
                "push_eax"
                "lea_eax,[ebp+DWORD] %-8"
                "mov_eax,[eax]"
                "pop_ebx"
                "cmp"
                "setge_al"
                "movzx_eax,al"
                "ret"
        );
}

int ri8(int o) {
        asm(
                "mov_eax, %0"
                "push_eax"
                "lea_eax,[ebp+DWORD] %-12"
                "mov_eax,[eax]"
                "push_eax"
                "lea_eax,[ebp+DWORD] %-4"
                "mov_eax,[eax]"
                "pop_ebx"
                "add_eax,ebx"
                "movsx_eax,BYTE_PTR_[eax]"
                "push_eax"
                "mov_eax, %0xFF"
                "pop_ebx"
                "and_eax,ebx"
                "pop_ebx"
                "ret"
        );
}

int wi8(int o,int v) {
        asm(
                "mov_eax, %0"
                "push_eax"
                "lea_eax,[ebp+DWORD] %-16"
                "mov_eax,[eax]"
                "push_eax"
                "lea_eax,[ebp+DWORD] %-4"
                "mov_eax,[eax]"
                "pop_ebx"
                "add_eax,ebx"
                "push_eax"
                "lea_eax,[ebp+DWORD] %-8"
                "mov_eax,[eax]"
                "pop_ebx"
                "mov_[ebx],al"
                "pop_ebx"
                "ret"
        );
}

int add(int a, int b){
        asm(
                "lea_eax,[ebp+DWORD] %-4"
                "mov_eax,[eax]"
                "push_eax"
                "lea_eax,[ebp+DWORD] %-8"
                "mov_eax,[eax]"
                "pop_ebx"
                "add_eax,ebx"
                "ret"
        );
}

int sub(int a, int b){
        asm(
                "lea_eax,[ebp+DWORD] %-4"
                "mov_eax,[eax]"
                "push_eax"
                "lea_eax,[ebp+DWORD] %-8"
                "mov_eax,[eax]"
                "pop_ebx"
                "sub_ebx,eax"
                "mov_eax,ebx"
                "ret"
        );
}

int mul(int a, int b){
        asm(
                "lea_eax,[ebp+DWORD] %-4"
                "mov_eax,[eax]"
                "push_eax"
                "lea_eax,[ebp+DWORD] %-8"
                "mov_eax,[eax]"
                "pop_ebx"
                "imul_ebx"
                "ret"
        );
}

int div(int a, int b){
        asm(
                "lea_eax,[ebp+DWORD] %-4"
                "mov_eax,[eax]"
                "push_eax"
                "lea_eax,[ebp+DWORD] %-8"
                "mov_eax,[eax]"
                "pop_ebx"
                "xchg_ebx,eax"
                "cdq"
                "idiv_ebx"
                "ret"
        );
}

int mod(int a, int b){
        asm(
                "lea_eax,[ebp+DWORD] %-4"
                "mov_eax,[eax]"
                "push_eax"
                "lea_eax,[ebp+DWORD] %-8"
                "mov_eax,[eax]"
                "pop_ebx"
                "xchg_ebx,eax"
                "cdq"
                "idiv_ebx"
                "mov_eax,edx"
                "ret"
        );
}

int and(int a, int b){
        asm(
                "lea_eax,[ebp+DWORD] %-4"
                "mov_eax,[eax]"
                "push_eax"
                "lea_eax,[ebp+DWORD] %-8"
                "mov_eax,[eax]"
                "pop_ebx"
                "and_eax,ebx"
                "ret"
        );
}

int or(int a, int b){
        asm(
                "lea_eax,[ebp+DWORD] %-4"
                "mov_eax,[eax]"
                "push_eax"
                "lea_eax,[ebp+DWORD] %-8"
                "mov_eax,[eax]"
                "pop_ebx"
                "or_eax,ebx"
                "ret"
        );
}

int shl(int a, int b){
        asm(
                "lea_eax,[ebp+DWORD] %-4"
                "mov_eax,[eax]"
                "push_eax"
                "lea_eax,[ebp+DWORD] %-8"
                "mov_eax,[eax]"
                "pop_ebx"
                "mov_ecx,eax"
                "mov_eax,ebx"
                "sal_eax,cl"
                "ret"
        );
}

int shr(int a, int b){
        asm(
                "lea_eax,[ebp+DWORD] %-4"
                "mov_eax,[eax]"
                "push_eax"
                "lea_eax,[ebp+DWORD] %-8"
                "mov_eax,[eax]"
                "pop_ebx"
                "mov_ecx,eax"
                "mov_eax,ebx"
                "sar_eax,cl"
                "ret"
        );
}

int ri32(int o) {
        return or(or(and(ri8(o), 255),
                shl(and(ri8(add(o, 1)), 255), 8)),
                or(shl(and(ri8(add(o, 2)), 255), 16),
                shl(and(ri8(add(o, 3)), 255), 24)));
}

int wi32(int o, int v) {
  wi8(o, and(v, 0xFF));
  v = shr(v, 8);
  wi8(add(o, 1), and(v, 0xFF));
  v = shr(v, 8);
  wi8(add(o, 2), and(v, 0xFF));
  v = shr(v, 8);
  wi8(add(o, 3), and(v, 0xFF));
}

int open(int name, int flag, int mode)
{
        asm("lea_ebx,[esp+DWORD] %12"
            "mov_ebx,[ebx]"
            "lea_ecx,[esp+DWORD] %8"
            "mov_ecx,[ecx]"
            "lea_edx,[esp+DWORD] %4"
            "mov_edx,[edx]"
            "mov_eax, %5"
            "int !0x80");
}

int close(int fd)
{
        asm("lea_ebx,[esp+DWORD] %4"
            "mov_ebx,[ebx]"
            "mov_eax, %6"
            "int !0x80");
}

int read(int fd, int buf, int len)
{
        asm("lea_ebx,[esp+DWORD] %12"
            "mov_ebx,[ebx]"
            "lea_ecx,[esp+DWORD] %8"
            "mov_ecx,[ecx]"
            "lea_edx,[esp+DWORD] %4"
            "mov_edx,[edx]"
            "mov_eax, %3"
            "int !0x80");
}

int write(int fd, int buf, int len)
{
        asm("lea_ebx,[esp+DWORD] %12"
            "mov_ebx,[ebx]"
            "lea_ecx,[esp+DWORD] %8"
            "mov_ecx,[ecx]"
            "lea_edx,[esp+DWORD] %4"
            "mov_edx,[edx]"
            "mov_eax, %4"
            "int !0x80");
}

int brk(int addr)
{
        asm("mov_eax,[esp+DWORD] %4"
            "push_eax"
            "mov_eax, %45"
            "pop_ebx"
            "int !0x80");
}

int NULL;
int _malloc_ptr;
int _brk_ptr;

int malloc(int size)
{
        int old_malloc;
        if(eq(NULL, _brk_ptr))
        {
                _brk_ptr = brk(0);
                _malloc_ptr = _brk_ptr;
        }

        if(lt(_brk_ptr, add(_malloc_ptr, size)))
        {
                _brk_ptr = brk(add(_malloc_ptr, size));
                if(eq(sub(0,1), _brk_ptr)) return 0;
        }

        old_malloc = _malloc_ptr;
        _malloc_ptr = add(_malloc_ptr, size);
        return old_malloc;
}

int exit(int value)
{
        asm("pop_ebx"
            "pop_ebx"
            "mov_eax, %1"
            "int !0x80");
}

int mks(int s) {
  return s;
}

int mkC(int s) {
  return ri8(s);
}

int ADD(int a, int b) { return add(a, b); }
int SUB(int a, int b) { return sub(a, b); }
int MUL(int a, int b) { return mul(a, b); }
int DIV(int a, int b) { return div(a, b); }
int MOD(int a, int b) { return mod(a, b); }
int EQ(int a, int b) { return eq(a, b); }
int NE(int a, int b) { return neq(a, b); }
int LT(int a, int b) { return lt(a, b); }
int LE(int a, int b) { return or(lt(a, b), eq(a, b)); }
int GT(int a, int b) { return gt(a, b); }
int GE(int a, int b) { return gte(a, b); }
int AND(int a, int b) { return and(a, b); }
int OR(int a, int b) { return or(a, b); }
int SHL(int a, int b) { return shl(a, b); }
int SHR(int a, int b) { return shr(a, b); }
int NEG(int a) { return sub(0, a); }
int NOT(int a) { return eq(a, 0); }

int XOR(int a, int b) {
        asm(
                "lea_eax,[ebp+DWORD] %-4"
                "mov_eax,[eax]"
                "push_eax"
                "lea_eax,[ebp+DWORD] %-8"
                "mov_eax,[eax]"
                "pop_ebx"
                "xor_eax,ebx"
                "ret"
        );
}
