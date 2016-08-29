// DIB.CPP
#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include "cdib.h"

using namespace std;

CDib::CDib() {
	m_pDibBytes = NULL;
	m_pDibInfo = NULL;
	m_pBFH = NULL;
}

CDib::~CDib() {
	// 如果位图已经被加载，释放之
	if (m_pDibBytes) {
		delete[]m_pDibBytes;
	}
	// 如果位图文件结构已被加载，释放之
	if (m_pDibInfo) {
		delete[]m_pDibInfo;
	}
	// 如果位图文件头结构已被加载，释放之
	if (m_pBFH) {
		delete[]m_pBFH;
	}
}

BYTE* CDib::GetpDibBytes() {
	return m_pDibBytes;
}

DWORD CDib::GetWidth() {
	return m_pDibInfo->bmiHeader.biWidth;
}

DWORD CDib::GetHeight() {
	return m_pDibInfo->bmiHeader.biHeight;
}

WORD CDib::GetBitsPerPixel() {
	if (!m_pDibInfo) {
		return 0;
	}
	return m_pDibInfo->bmiHeader.biBitCount;
}

DWORD CDib::GetLineBytes() {
	return((((m_pDibInfo->bmiHeader.biWidth * GetBitsPerPixel()) + 31) / 32) * 4);
}

DWORD CDib::GetLineBytes(LONG biWidth, LONG biBitCount) {
	return ((((biWidth * biBitCount) + 31) / 32) * 4);
}

DWORD CDib::GetBodySize() {
	return GetLineBytes() * m_pDibInfo->bmiHeader.biHeight;
}

BOOL CDib::Load(const char *filePath) {
	if (!strlen(filePath)) {
		cout << "Invalid Or Empty File Path!" << endl;
		return FALSE;
	}
	// 二进制读方式打开指定图像文件
	FILE *fp = fopen(filePath, "rb");
	if (!fp) {
		cout << "Loaded Image File Unsuccessfully!" << endl;
		return FALSE;
	}

	// 分配内存空间
	m_pBFH = (BITMAPFILEHEADER*)new BYTE[sizeof(BITMAPFILEHEADER)];
	
	// 读取位图文件头进入内存
	fread(m_pBFH, sizeof(BITMAPFILEHEADER), 1, fp);


	// 如果文件类型标头不是“0x4d42”，表示该文件不是BMP类型文件
	if (m_pBFH->bfType != 0x4d42) {
		cout << "Image File Is Not A Bitmap!" << endl;
		return FALSE;
	}

	// 读取位图信息头进入内存
	BITMAPINFOHEADER bih;
	fread(&bih, sizeof(BITMAPINFOHEADER), 1, fp);

	// 定义调色板大小
	int paletteSize = 0;
	switch (bih.biBitCount) {
		case 1:
			paletteSize = 2;
			break;
		case 4:
			paletteSize = 16;
			break;
		case 8:
			paletteSize = 256;
			break;
	}

	m_pDibInfo = (BITMAPINFO*)new BYTE[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*paletteSize];
	m_pDibInfo->bmiHeader = bih;

	if (paletteSize) {
		// 申请颜色表所需空间，读颜色表进内存
		fread(m_pDibInfo->bmiColors, sizeof(RGBQUAD), paletteSize, fp);
	}

	// 申请位图数据所需空间，读位图数据进内存
	DWORD bodySize = GetBodySize();
	m_pDibBytes = (BYTE*)new BYTE[bodySize];
	// 把文件指针移动到DIB像素数组
	fseek(fp, m_pBFH->bfOffBits, 0);
	fread(m_pDibBytes, sizeof(BYTE) ,bodySize, fp);

	// 关闭文件
	fclose(fp);
	cout << "Loaded Image File: \"" << filePath << "\" Successfully!" << endl;

	return TRUE;
}

BOOL CDib::Save(const char *filePath) {
	if (!strlen(filePath)) {
		cout << "Invalid Saved Path!" << endl;
		return FALSE;
	}
	return Save(filePath, m_pDibBytes, m_pDibInfo->bmiHeader.biWidth, m_pDibInfo->bmiHeader.biHeight, m_pDibInfo->bmiHeader.biBitCount, m_pDibInfo->bmiColors);
}

BOOL CDib::Save(const char *filePath, BYTE *m_pDibBytes, LONG m_width, LONG m_height, LONG m_biBitCount, RGBQUAD *m_pPalette) {
	// 如果位图数组指针为空，函数返回
	if (!strlen(filePath)) {
		cout << "Invalid Saved Path!" << endl;
		return FALSE;
	}

	// 像素数据空指针
	if (!m_pDibBytes) {
		cout << "Invalid Image File Data!" << endl;
		return FALSE;
	}

	// 颜色表大小，以字节为单位，灰度图像颜色为1024字节，彩色图像颜色表大小为0
	int colorTableSize = 0;
	if (m_biBitCount == 8) {
		colorTableSize = 1024;
	}

	// 待存储图像数据格式为每行字节数为4的倍数
	int lineBytes = GetLineBytes(m_width, m_biBitCount);

	// 以二进制写的方式打开文件
	FILE *fp = fopen(filePath, "wb");
	if (!fp) {
		cout << "Writing Image File Unsuccessfully!" << endl;
		return FALSE;
	}

	// 申请位图文件头结构变量，填写文件头信息
	BITMAPFILEHEADER fileHead;
	fileHead.bfType = 0x4D42; // 位图始终为此类型
	// bfSize是图像文件4个组成部分之和
	fileHead.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + colorTableSize + lineBytes*m_height;
	fileHead.bfReserved1 = 0;
	fileHead.bfReserved2 = 0;

	// bfOffBits 是图像文件前3个部分所需空间之和
	fileHead.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + colorTableSize;

	// 写文件头进入文件
	try
	{
		fwrite(&fileHead, sizeof(BITMAPFILEHEADER), 1, fp);
	}
	catch (const std::exception& e)
	{
		cout << "Writed File Header Unsuccessfully!" << endl;
		cout << e.what() << endl;
		exit(-1);
	}

	// 申请位图信息头结构变量，填写信息头信息
	BITMAPINFOHEADER infoHead;
	infoHead.biBitCount = m_biBitCount;
	if (m_biBitCount == 8) {
		infoHead.biClrImportant = infoHead.biClrUsed = 256;
	}
	else {
		infoHead.biClrImportant = infoHead.biClrUsed = 0;
	}
	infoHead.biCompression = BI_RGB;
	infoHead.biHeight = m_height;
	infoHead.biPlanes = 1;
	infoHead.biSize = 40;
	infoHead.biSizeImage = lineBytes * m_height;
	infoHead.biWidth = m_width;
	infoHead.biXPelsPerMeter = 0;
	infoHead.biYPelsPerMeter = 0;

	// 写位图头信息进文件
	try
	{
		fwrite(&infoHead, sizeof(BITMAPINFOHEADER), 1, fp);
	}
	catch (const std::exception& e)
	{
		cout << "Writed Info Header Unsuccessfully!" << endl;
		cout << e.what() << endl;
		exit(-1);
	}

	// 如果为灰度图像，有颜色表，写入文件
	if (m_biBitCount == 8) {
		fwrite(m_pPalette, sizeof(RGBQUAD), 256, fp);
	}

	// 写位图数据进文件
	try
	{
		fwrite(m_pDibBytes, m_height * lineBytes, 1, fp);
	}
	catch (const std::exception& e)
	{
		cout << "Writed Image Data Unsuccessfully!" << endl;
		cout << e.what() << endl;
		exit(-1);
	}

	fclose(fp);
	cout << "Image File \"" << filePath << "\" Stored Successfully!" << endl;
	return TRUE;
}

void CDib::PrintFileHead() {
	if (m_pBFH) {
		cout << "bfType: " << m_pBFH->bfType
			<<" bfSize: "<< m_pBFH->bfSize
			<<" bfReserved1: "<< m_pBFH->bfReserved1
			<< " bfReserved2: " << m_pBFH->bfReserved2
			<< " bfOffBits: " << m_pBFH->bfOffBits
			<< endl;
	}
	else {
		cout << "Empty File Header!" << endl;
	}
}

void CDib::PrintInfoHead() {
	if (m_pDibInfo) {
		cout << "width: " << m_pDibInfo->bmiHeader.biWidth
			<< " height: " << m_pDibInfo->bmiHeader.biHeight
			<< " biBitCount: " << m_pDibInfo->bmiHeader.biBitCount
			<< endl;
	}
	else {
		cout << "Empty Info Header!" << endl;
	}
}

BOOL CDib::GrayScale() {
	if (!m_pDibBytes) {
		cout << "Failed To Gray Scale Image File!" << endl
			<< "DIB Data Is Empty!" << endl;
		return FALSE;
	}
	// 循环变量，图像横纵坐标
	DWORD i, j;
	// 每行字节数
	DWORD lineBytes = GetLineBytes();
	LONG width = m_pDibInfo->bmiHeader.biWidth;
	LONG height = m_pDibInfo->bmiHeader.biHeight;
	LONG bitCount = m_pDibInfo->bmiHeader.biBitCount;
	if (bitCount == 24) {
		BYTE r, g, b;
		int k;
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				b = *(m_pDibBytes + i*lineBytes + j * 3 + 0);
				g = *(m_pDibBytes + i*lineBytes + j * 3 + 1);
				r = *(m_pDibBytes + i*lineBytes + j * 3 + 2);
				int val = (30 * r + 59 * g + 11 * b) / 100;
				val = val > 255 ? 255 : val;
				for (k = 0; k < 3; k++) {
					*(m_pDibBytes + i*lineBytes + j * 3 + k) = val;
				}
			}
		}
	}
	cout << "Image Has Gray Scaled Successfully!" << endl;
	return TRUE;
}

BOOL CDib::BinaryScale() {
	if (!m_pDibBytes) {
		cout << "Failed To Binary Scale Image File!" << endl
			<< "DIB Data Is Empty!" << endl;
		return FALSE;
	}
	// 循环变量，图像横纵坐标
	DWORD i, j;
	// 每行字节数
	DWORD lineBytes = GetLineBytes();
	LONG width = m_pDibInfo->bmiHeader.biWidth;
	LONG height = m_pDibInfo->bmiHeader.biHeight;
	LONG bitCount = m_pDibInfo->bmiHeader.biBitCount;
	if (bitCount == 24) {
		BYTE r, g, b;
		int k;
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				b = *(m_pDibBytes + i*lineBytes + j * 3 + 0);
				g = *(m_pDibBytes + i*lineBytes + j * 3 + 1);
				r = *(m_pDibBytes + i*lineBytes + j * 3 + 2);
				int val = (30 * r + 59 * g + 11 * b) / 100;
				val = val >= BINARY_THRESHOLD ? 255 : 0;
				for (k = 0; k < 3; k++) {
					*(m_pDibBytes + i*lineBytes + j * 3 + k) = val;
				}
			}
		}
	}
	// 如果为灰度图
	else if (bitCount == 8) {
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				BYTE val = *(m_pDibBytes + i*lineBytes + j);
				val = val >= BINARY_THRESHOLD ? 255 : 0;
				*(m_pDibBytes + i*lineBytes + j) = val;
			}
		}
	}
	cout << "Image Has Binary Scaled Successfully!" << endl;
	return TRUE;
}

BOOL CDib::Negative() {
	if (!m_pDibBytes) {
		cout << "Failed To Negative Image File!" << endl
			<< "DIB Data Is Empty!" << endl;
		return FALSE;
	}
	// 循环变量，图像横纵坐标
	DWORD i, j;
	// 每行字节数
	DWORD lineBytes = GetLineBytes();
	LONG width = m_pDibInfo->bmiHeader.biWidth;
	LONG height = m_pDibInfo->bmiHeader.biHeight;
	LONG bitCount = m_pDibInfo->bmiHeader.biBitCount;
	BYTE maxVal = 255;
	if (bitCount == 24) {
		BYTE r, g, b;
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				b = *(m_pDibBytes + i*lineBytes + j * 3 + 0);
				*(m_pDibBytes + i*lineBytes + j * 3 + 0) = maxVal - b;
				g = *(m_pDibBytes + i*lineBytes + j * 3 + 1);
				*(m_pDibBytes + i*lineBytes + j * 3 + 1) = maxVal - g;
				r = *(m_pDibBytes + i*lineBytes + j * 3 + 2);
				*(m_pDibBytes + i*lineBytes + j * 3 + 2) = maxVal - r;
			}
		}
	}
	else if (bitCount == 8) {
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				BYTE val = *(m_pDibBytes + i*lineBytes + j);
				*(m_pDibBytes + i*lineBytes + j) = maxVal - val;
			}
		}
	}
	cout << "Image Has Negatived Successfully!" << endl;
	return TRUE;
}