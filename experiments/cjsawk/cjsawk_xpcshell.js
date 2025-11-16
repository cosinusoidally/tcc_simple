init = new Function([],' \
var { ctypes } = ChromeUtils.importESModule(  \
  "resource://gre/modules/ctypes.sys.mjs"  \
); return ctypes;');

ctypes = init();

libc = ctypes.open("libc.so.6");

puts = libc.declare("puts", ctypes.default_abi, ctypes.int32_t, ctypes.char.ptr);

print(ctypes);
print(puts);
puts("hello world from ctypes");
// load("cjsawk_nodejs.js");
