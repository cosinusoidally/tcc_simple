function add(a, b){
  a = a | 0;
  b = b | 0;
  return a + b;
}

function sub(a, b){
  a = a | 0;
  b = b | 0;
  return a - b;
}

/* wrapper for sub since sub is a builtin word in AWK */
function SUB(a, b) {
  return sub(a, b);
}

/* wrapper for and since sub is a builtin word in AWK */
function AND(a, b) {
  return and(a, b);
}

function mul(a, b){
  a = a | 0;
  b = b | 0;
  return a * b;
}

function shl(a, b){
  a = a | 0;
  b = b | 0;
  return (a << b) | 0;
}

function shr(a, b){
  a = a | 0;
  b = b | 0;
  return a >> b;
}

function lt(a, b){
  a = a | 0;
  b = b | 0;
  return (a < b) | 0;
}

function gt(a, b){
  a = a | 0;
  b = b | 0;
  return (a > b) | 0;
}

function lte(a, b){
  a = a | 0;
  b = b | 0;
  return (a <= b) | 0;
}

function gte(a, b){
  a = a | 0;
  b = b | 0;
  return (a >= b) | 0;
}

function and(a, b){
  a = a | 0;
  b = b | 0;
  return a & b;
}

function or(a, b){
  a = a | 0;
  b = b | 0;
  return a | b;
}

function eq(a, b){
  a = a | 0;
  b = b | 0;
  return (a == b) | 0;
}

function neq(a, b){
  a = a | 0;
  b = b | 0;
  return (a != b) | 0;
}

function xor(a, b){
  a = a | 0;
  b = b | 0;
  return a ^ b;
}

function div(a, b){
  a = a | 0;
  b = b | 0;
  return (a / b) | 0;
}

function mod(a, b){
  a = a | 0;
  b = b | 0;
  return a % b;
}
