const imager = require('../src/imager');

const readPath = 'snail.bmp';
const writePath = 'output/gray.bmp';
const savePath = 'output/capture.bmp'

imager.grayScale(readPath, writePath);
imager.captureScreen(savePath);