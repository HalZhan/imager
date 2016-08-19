// hello.node
const addon = require('./build/Release/adder.node');

const A = 11, B = 13;

console.log(A + ' + ' + B + ' =', addon.add(A, B));