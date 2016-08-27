const imager = require('../src/imager');

const readPath = '../resources/snail.bmp';

// imager.binaryScale(readPath, './output/_binary_pic.bmp');
// imager.grayScale(readPath);
// imager.negative(readPath, './output/_negative_pic.bmp');
imager.captureScreen('./output/captureScreen.bmp');