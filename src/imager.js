module.exports = {
	binaryScale: require('./BinaryScale/build/Release/imager.node').binaryScale,
	grayScale: require('./GrayScale/build/Release/imager.node').grayScale,
	negative: require('./Negative/build/Release/imager.node').negative,
	captureScreen: require('./ScreenCapture/build/Release/imager.node').captureScreen
}