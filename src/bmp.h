#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <Windows.h>

#define BITCOUNT_GRAY 8 // 灰度图每像素位数
#define BITCOUNT_COLOR 24 // 彩色图每像素位数
#define BINARY_THRESHOLD 128 // 二值化灰度阈值(小于此阈值皆为0，大于此阈值皆为255)

using namespace std;

class BMP
{
public:
	BMP();
	~BMP();
	// 读取图片
	bool readBmp(char *bmpName);
	// 存储图片
	bool saveBmp(char *bmpName);
	// 灰度化
	bool grayScale();
	// 二值化
	bool binaryScale();
	unsigned char *pBmpBuf;  // 读入图像数据的指针
	int bmpWidth; // 图像的宽
	int bmpHeight; // 图像的高
	RGBQUAD *pColorTable; // 颜色表指针
	int biBitCount; // 图像类型，每像素位数

private:
	// 存储图片
	bool _saveBmp(char *bmpName, unsigned char *imgBuf, int width, int height, int biBitCount, RGBQUAD *pColorTable);
};

BMP::BMP()
{
}

BMP::~BMP()
{
	delete pBmpBuf;
	delete pColorTable;
}

// 读取图片
bool BMP::readBmp(char *bmpName) {
	// 二进制读方式打开指定图像文件
	FILE *fp = fopen(bmpName, "rb");
	if (fp == 0) {
		return 0;
	}
	// 跳过位图文件头结构 BITMAPFILEHEADER
	fseek(fp, sizeof(BITMAPFILEHEADER), 0);

	// 定义位图信息头结构变量，读取位图信息头进内存，存放在变量head中
	BITMAPINFOHEADER head;
	fread(&head, sizeof(BITMAPINFOHEADER), 1, fp);

	// 获取图像宽、高、每像素所占位数等信息
	bmpWidth = head.biWidth;
	bmpHeight = head.biHeight;
	biBitCount = head.biBitCount;

	// 定义变量，计算图像每行像素所占字节数（必须是4的倍数）
	int lineByte = (bmpWidth * biBitCount / 8 + 3) / 4 * 4;

	// 灰度图像有颜色表，且颜色表表象为256
	if (biBitCount == 8) {
		// 申请颜色表所需要的空间，读颜色表进内存
		pColorTable = new RGBQUAD[256];
		fread(pColorTable, sizeof(RGBQUAD), 256, fp);
	}

	// 申请位图数据所需空间，读位图数据进内存
	pBmpBuf = new unsigned char[lineByte * bmpHeight];
	fread(pBmpBuf, 1, lineByte * bmpHeight, fp);

	// 关闭文件
	fclose(fp);

	return 1;
}
// 存储图片
bool BMP::saveBmp(char *bmpName) {
	return _saveBmp(bmpName, pBmpBuf, bmpWidth, bmpHeight, biBitCount, pColorTable);
}

// 灰度化
bool BMP::grayScale() {
	// 循环变量，图像坐标
	int i, j;
	// 每行字节数
	int lineByte = (bmpWidth * biBitCount / 8 + 3) / 4 * 4;
	// 循环变量，针对彩色图像，遍历每像素三个分量
	int k;
	if (biBitCount == BITCOUNT_COLOR) { // 彩色图像
		for (i = 0; i < bmpHeight; i++) {
			for (j = 0; j < bmpWidth; j++) {
				// 加权平均数法
				// f(i,j) = 0.30R(i,j) + 0.59G(i,j) + 0.11B(i,j)
				char b = *(pBmpBuf + i*lineByte + j * 3 + 0);
				char g = *(pBmpBuf + i*lineByte + j * 3 + 1);
				char r = *(pBmpBuf + i*lineByte + j * 3 + 2);
				int val = (30 * r + 59 * g + 11 * b) / 100;
				val = val > 255 ? 255 : val;
				for (k = 0; k < 3; k++) { //  每像素RGB三个分量分别置为相等的灰度值
					*(pBmpBuf + i*lineByte + j * 3 + k) = val;
				}
			}
		}
	}
	return true;
}

// 二值化
bool BMP::binaryScale() {
	char low_val = 0, high_val = 255;
	int i, j, k;
	// 定义变量，计算图像每行像素所占字节数（必须是4的倍数）
	int lineByte = (bmpWidth * biBitCount / 8 + 3) / 4 * 4;
	if (biBitCount == BITCOUNT_GRAY) {
		for (i = 0; i < bmpHeight; i++) {
			for (j = 0; j < bmpWidth; j++) {
				char val = *(pBmpBuf + i*lineByte + j);
				if (val <= BINARY_THRESHOLD) {
					val = low_val;
				}
				else {
					val = high_val;
				}
				*(pBmpBuf + i*lineByte + j) = val;
			}
		}
	}
	else if(biBitCount == BITCOUNT_COLOR){
		for (i = 0; i < bmpHeight; i++) {
			for (j = 0; j < bmpWidth; j++) {
				char b = *(pBmpBuf + i*lineByte + j * 3 + 0);
				char g = *(pBmpBuf + i*lineByte + j * 3 + 1);
				char r = *(pBmpBuf + i*lineByte + j * 3 + 2);
				int val = (30 * r + 59 * g + 11 * b) / 100;
				if (val <= BINARY_THRESHOLD) {
					val = low_val;
				}
				else {
					val = high_val;
				}
				for (k = 0; k < 3; k++) {
					*(pBmpBuf + i*lineByte + j * 3 + k) = val;
				}
			}
		}
	}

	return 1;
}

// 存储图片(内部使用)
bool BMP::_saveBmp(char *bmpName, unsigned char *imgBuf, int width, int height, int biBitCount, RGBQUAD *pColorTable) {
	// 如果位图数据指针为0， 则没有数据传入，函数返回
	if (!imgBuf) {
		return 0;
	}
	// 颜色表大小，以字节为单位，灰度图像颜色表为1024字节，彩色图像颜色表大小为0
	int colorTablesize = 0;
	if (biBitCount == BITCOUNT_GRAY) {
		colorTablesize = 1024;
	}
	// 待存储图像数据每行字节数为4的倍数
	int lineByte = (width * biBitCount / 8 + 3) / 4 * 4;
	// 以二进制写得方式打开文件
	FILE *fp = fopen(bmpName, "wb");
	if (fp == 0) {
		return 0;
	}

	// 申请位图文件头结构变量，填写文件头信息
	BITMAPFILEHEADER fileHead;
	fileHead.bfType = 0x4D42; // bmp类型

							  // bfSize是图像文件4个组成部分之和
	fileHead.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + colorTablesize + lineByte * height;
	fileHead.bfReserved1 = 0;
	fileHead.bfReserved2 = 0;

	// bfOffBits 是图像文件前3个部分所需空间之和
	fileHead.bfOffBits = 54 + colorTablesize;

	// 写文件头进文件
	fwrite(&fileHead, sizeof(BITMAPFILEHEADER), 1, fp);

	// 申请位图信息头结构变量，填写信息头信息
	BITMAPINFOHEADER head;
	head.biBitCount = biBitCount;
	head.biClrImportant = 0;
	head.biClrUsed = 0;
	head.biCompression = 0;
	head.biHeight = height;
	head.biPlanes = 1;
	head.biSize = 40;
	head.biSizeImage = lineByte * height;
	head.biWidth = width;
	head.biXPelsPerMeter = 0;
	head.biYPelsPerMeter = 0;

	// 写位图头信息进内存
	fwrite(&head, sizeof(BITMAPINFOHEADER), 1, fp);

	// 如果灰度图像，有颜色表，写入文件
	if (biBitCount == 8) {
		fwrite(pColorTable, sizeof(RGBQUAD), 256, fp);
	}

	// 写位图数据进文件
	fwrite(imgBuf, height * lineByte, 1, fp);

	fclose(fp);

	return 1;
}