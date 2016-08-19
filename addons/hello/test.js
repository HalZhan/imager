// hello.node
const addon = require('./build/Release/hello.node');

console.log('hello', addon.hello());