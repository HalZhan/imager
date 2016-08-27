#include "Bmp.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>

using namespace std;

int main(int argc, char **argv) {
	char *readPath;
	char defaultWritePath[] = "_deal_.bmp"; // 写入路径
	char *writePath;
	if (argc >= 2) {
		readPath = argv[1];
	}
	else {
		cout << "Please input image's path!" << endl;
		return -1;
	}

	if (argc >= 3) {
		writePath = argv[2];
	}
	else {
		writePath = defaultWritePath;
	}

	BMP bmp;
	bmp.readBmp(readPath);

	// 输出图像信息
	printf("width=%d, height=%d, biBitCount=%d\n", bmp.bmpWidth, bmp.bmpHeight, bmp.biBitCount);

	// 灰度化
	// bmp.grayScale();
	// 二值化
	bmp.binaryScale();
	// 将图像数据存盘
	bmp.saveBmp(writePath);

	system("pause");
	return 0;
}