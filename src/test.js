const imager = require('./build/Release/imager.node');

const readPath = 'snail.bmp';
const writePath = 'gray.bmp';

imager.grayScale(readPath);