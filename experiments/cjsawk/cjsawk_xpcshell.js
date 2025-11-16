init = new Function([],' \
var { ctypes } = ChromeUtils.importESModule(  \
  "resource://gre/modules/ctypes.sys.mjs"  \
); return ctypes;');

ctypes = init();

print(ctypes);
print(puts);
// load("cjsawk_nodejs.js");
