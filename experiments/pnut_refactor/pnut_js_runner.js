var readFile = os.file.readFile;
var writeTypedArrayToFile = os.file.writeTypedArrayToFile;
var sourcePath = scriptArgs[0];
var outputPath = scriptArgs[1];

var heap = new Uint8Array(1048576);
var heapTop = 1;
var files = {};
var nextFd = 3;
var stdoutBytes = [];
var stringCache = {};
var trace = os.getenv("PNUT_JS_TRACE");

function trace_msg(s) {
    if (trace) {
        printErr(s);
    }
}

function ensure_heap(needed) {
    var next;
    var old;
    if (needed <= heap.length) {
        return 0;
    }
    next = heap.length;
    while (next < needed) {
        next = next * 2;
    }
    old = heap;
    heap = new Uint8Array(next);
    heap.set(old);
    return 0;
}

function cstr(ptr) {
    var chars = [];
    var i = ptr;
    while (heap[i] !== 0) {
        chars.push(String.fromCharCode(heap[i]));
        i = i + 1;
    }
    return chars.join("");
}

function brk(n) {
    var p = heapTop;
    heapTop = heapTop + n;
    ensure_heap(heapTop + 1);
    return p;
}

function ri8(p) {
    return heap[p] | 0;
}

function wi8(p, v) {
    ensure_heap(p + 1);
    heap[p] = v & 255;
    return v & 255;
}

function ri32(p) {
    return (heap[p] | (heap[p + 1] << 8) | (heap[p + 2] << 16) | (heap[p + 3] << 24)) | 0;
}

function wi32(p, v) {
    ensure_heap(p + 4);
    heap[p] = v & 255;
    heap[p + 1] = (v >>> 8) & 255;
    heap[p + 2] = (v >>> 16) & 255;
    heap[p + 3] = (v >>> 24) & 255;
    return v | 0;
}

function mks(s) {
    if (Object.prototype.hasOwnProperty.call(stringCache, s)) {
        return stringCache[s];
    }
    var p = brk(s.length + 1);
    var i = 0;
    while (i < s.length) {
        wi8(p + i, s.charCodeAt(i));
        i = i + 1;
    }
    wi8(p + s.length, 0);
    stringCache[s] = p;
    return p;
}

function mkC(s) {
    return s.charCodeAt(0) | 0;
}

function open(path, flags, mode) {
    var name = cstr(path);
    trace_msg("open " + name);
    var data = readFile(name, "binary");
    if (data.length === undefined) {
        trace_msg("binary read type=" + Object.prototype.toString.call(data) + " byteLength=" + data.byteLength);
        data = new Uint8Array(data);
    }
    var fd = nextFd;
    nextFd = nextFd + 1;
    files[fd] = { data: data, pos: 0 };
    trace_msg("open fd=" + fd + " len=" + data.length);
    return fd;
}

function read(fd, buf, len) {
    var file = files[fd];
    var n;
    var i;
    if (!file) {
        return -1;
    }
    n = len;
    if (n > file.data.length - file.pos) {
        n = file.data.length - file.pos;
    }
    trace_msg("read fd=" + fd + " len=" + len + " n=" + n + " pos=" + file.pos);
    i = 0;
    while (i < n) {
        wi8(buf + i, file.data[file.pos + i]);
        i = i + 1;
    }
    file.pos = file.pos + n;
    return n;
}

function write(fd, buf, len) {
    var i = 0;
    if (fd === 1) {
        while (i < len) {
            stdoutBytes.push(ri8(buf + i));
            i = i + 1;
        }
        return len;
    }
    if (fd === 2) {
        var chars = [];
        while (i < len) {
            chars.push(String.fromCharCode(ri8(buf + i)));
            i = i + 1;
        }
        printErr(chars.join(""));
        return len;
    }
    return -1;
}

function close(fd) {
    delete files[fd];
    return 0;
}

function exit(status) {
    quit(status);
}

function add(a, b) { return (a + b) | 0; }
function ADD(a, b) { return (a + b) | 0; }
function sub(a, b) { return (a - b) | 0; }
function SUB(a, b) { return (a - b) | 0; }
function mul(a, b) { return Math.imul(a, b) | 0; }
function MUL(a, b) { return Math.imul(a, b) | 0; }
function div(a, b) { return (a / b) | 0; }
function DIV(a, b) { return (a / b) | 0; }
function mod(a, b) { return (a % b) | 0; }
function MOD(a, b) { return (a % b) | 0; }
function neg(a) { return (-a) | 0; }
function NEG(a) { return (-a) | 0; }
function not(a) { return a ? 0 : 1; }
function NOT(a) { return a ? 0 : 1; }
function eq(a, b) { return a === b ? 1 : 0; }
function EQ(a, b) { return a === b ? 1 : 0; }
function ne(a, b) { return a !== b ? 1 : 0; }
function NE(a, b) { return a !== b ? 1 : 0; }
function lt(a, b) { return a < b ? 1 : 0; }
function LT(a, b) { return a < b ? 1 : 0; }
function le(a, b) { return a <= b ? 1 : 0; }
function LE(a, b) { return a <= b ? 1 : 0; }
function gt(a, b) { return a > b ? 1 : 0; }
function GT(a, b) { return a > b ? 1 : 0; }
function ge(a, b) { return a >= b ? 1 : 0; }
function GE(a, b) { return a >= b ? 1 : 0; }
function and(a, b) { return (a & b) | 0; }
function AND(a, b) { return (a & b) | 0; }
function or(a, b) { return (a | b) | 0; }
function OR(a, b) { return (a | b) | 0; }
function xor(a, b) { return (a ^ b) | 0; }
function XOR(a, b) { return (a ^ b) | 0; }
function shl(a, b) { return (a << b) | 0; }
function SHL(a, b) { return (a << b) | 0; }
function shr(a, b) { return a >> b; }
function SHR(a, b) { return a >> b; }

load("mawkcc_self.c");

if (trace) {
    var old_read_source = read_source;
    read_source = function(path) {
        trace_msg("read_source");
        var r = old_read_source(path);
        trace_msg("read_source done src_len=" + src_len);
        return r;
    };
    var old_init_lexer = init_lexer;
    init_lexer = function() {
        trace_msg("init_lexer");
        return old_init_lexer();
    };
    var old_code_reset = code_reset;
    code_reset = function() {
        trace_msg("code_reset");
        return old_code_reset();
    };
    var old_next_tok = next_tok;
    next_tok = function() {
        var r = old_next_tok();
        trace_msg("next_tok tok=" + tok + " idx=" + idx_pos);
        return r;
    };
    var old_parse_program = parse_program;
    parse_program = function() {
        trace_msg("parse_program");
        var r = old_parse_program();
        trace_msg("parse_program done");
        return r;
    };
    var old_patch_calls = patch_calls;
    patch_calls = function() {
        trace_msg("patch_calls");
        return old_patch_calls();
    };
    var old_build_binary = build_binary;
    build_binary = function() {
        trace_msg("build_binary");
        var r = old_build_binary();
        trace_msg("build_binary done bin_len=" + bin_len);
        return r;
    };
    var old_emit_binary = emit_binary;
    emit_binary = function() {
        trace_msg("emit_binary");
        return old_emit_binary();
    };
}

var argv = brk(8);
wi32(argv, mks("pnut.exe"));
wi32(argv + 4, mks(sourcePath));

trace_msg("calling main");
var status = main(2, argv);
trace_msg("main returned " + status);
if (status !== 0) {
    quit(status);
}

trace_msg("writing " + stdoutBytes.length + " bytes");
writeTypedArrayToFile(outputPath, new Uint8Array(stdoutBytes));
