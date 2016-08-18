#include "Bmp.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>

using namespace std;

int main(int argc, char **argv) {
	char readPath[] = "snail.bmp"; // 读取路径
	char writePath[] = "snail_binary.bmp"; // 写入路径

	BMP bmp;
	bmp.readBmp(readPath);

	// 输出图像信息
	printf("width=%d, height=%d, biBitCount=%d\n", bmp.bmpWidth, bmp.bmpHeight, bmp.biBitCount);

	// 灰度化
	bmp.binaryScale();
	// 将图像数据存盘
	bmp.saveBmp(writePath);

	system("pause");
	return 0;
}