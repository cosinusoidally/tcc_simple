init = new Function([],' \
var { ctypes } = ChromeUtils.importESModule(  \
  "resource://gre/modules/ctypes.sys.mjs"  \
); return ctypes;');

ctypes = init();

libc = ctypes.open("libc.so.6");

puts = libc.declare("puts", ctypes.default_abi, ctypes.int32_t, ctypes.char.ptr);

(function() {
// FILE * fopen ( const char * filename, const char * mode );
var fopen=libc.declare("fopen",
                        ctypes.default_abi, /* call ABI */
                        ctypes.voidptr_t,         /* return type */
                        ctypes.char.ptr,  /* argument type */
                        ctypes.char.ptr);   /* argument type */
// int fseek(FILE *stream, long offset, int whence);
var fseek=libc.declare("fseek",
                        ctypes.default_abi, /* call ABI */
                        ctypes.int,         /* return type */
                        ctypes.voidptr_t,  /* argument type */
                        ctypes.long,   /* argument type */
                        ctypes.int);   /* argument type */
// void rewind(FILE *stream);
var rewind=libc.declare("rewind",
                        ctypes.default_abi, /* call ABI */
                        ctypes.void_t,
                        ctypes.voidptr_t);
var SEEK_END=2;
// int fclose ( FILE * stream );
var fclose=libc.declare("fclose",
                        ctypes.default_abi, /* call ABI */
                        ctypes.int,
                        ctypes.voidptr_t);

// long int ftell ( FILE * stream );
var ftell=libc.declare("ftell",
                        ctypes.default_abi, /* call ABI */
                        ctypes.long,
                        ctypes.voidptr_t);

// size_t fread(void *ptr, size_t size, size_t nmemb, FILE * stream );
var fread=libc.declare("fread",
                        ctypes.default_abi, /* call ABI */
                        ctypes.int,
                        ctypes.voidptr_t,
                        ctypes.int,
                        ctypes.int,
                        ctypes.voidptr_t);

read = function(n,t) {
  var f=fopen(n,"rb");

  // FIXME this makes no sense, but if I don't do it I get a segfault
  // FIXME check if this is still needed
  var ptr = ctypes.cast( f, ctypes.ArrayType( ctypes.int, 1 ).ptr );
  ptr.contents;

  fseek(f,0,SEEK_END);
  var l=ftell(f);
  rewind(f);
  l=parseInt(l.toString(),10);
  var b=new Uint8Array(l);
  fread(b,1,l,f);
  fclose(f);
  if(t==="binary"){
    return b;
  };

  var o=[];
  for(var i=0;i<l;i++){
    o.push(String.fromCharCode(b[i]));
  }
  o=o.join("");
  return o;
}
})();

print(ctypes);
print(puts);
// print(fopen("artifacts/deps/cjsawk_full.c", "rb"));
puts("hello world from ctypes");


fname="./artifacts/deps/cjsawk_full.c";
load("cjsawk_nodejs.js");
