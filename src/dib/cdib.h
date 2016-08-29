// CDIB.h
#ifndef __CDIB_H__
#define __CDIB_H__
#include <Windows.h>
#include <wingdi.h>
#define BINARY_THRESHOLD 128 // 二值化灰度阈值
class CDib
{
public:
	// 构造函数
	CDib();
	// 析构函数
	~CDib();
	// 载入位图资源，传入文件路径名
	BOOL Load(const char *);
	// 存储位图资源，传入文件路径名
	BOOL Save(const char *);
	// 存储位图资源，传入文件路径名，像素颜色数组指针，宽度，高度，颜色类型，颜色表
	BOOL Save(const char *, BYTE *, LONG, LONG, LONG, RGBQUAD *);
	// 灰度化图像
	BOOL GrayScale();
	// 二值化图像
	BOOL BinaryScale();
	// 图像负片
	BOOL Negative();
	// 打印文件头
	void PrintFileHead();
	// 打印信息头
	void PrintInfoHead();
private:
	// 实际图像像素数组指针
	BYTE *m_pDibBytes;
	// DIB文件头信息指针
	BITMAPFILEHEADER *m_pBFH;
	// DIB位图信息指针
	BITMAPINFO *m_pDibInfo;
	// 返回像素数据指针
	BYTE *GetpDibBytes();
	// 返回图像宽度
	DWORD GetWidth();
	// 返回图像高度
	DWORD GetHeight();
	// 获得每个像素所占位数
	WORD GetBitsPerPixel();
	// 返回DIB位图像素数组大小
	DWORD GetBodySize();
	// 返回每行扫描线所需字节数
	DWORD GetLineBytes();
	// 返回每行扫描线所需字节数，传入宽度与每像素字节数
	DWORD GetLineBytes(LONG, LONG);
};
#endif