#define far
#define TRUE true
#define FALSE false
#define FAR far
#define BI_RGB        0L
#define BI_RLE8       1L
#define BI_RLE4       2L
#define BI_BITFIELDS  3L
#define BI_JPEG       4L
#define BI_PNG        5L

typedef unsigned long long ULONGLONG;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef unsigned char BYTE;
typedef unsigned int UINT;
typedef long LONG;
typedef bool BOOL;


// 位图文件头BITMAPFILEHEADER是一个结构体，长度为14字节
typedef struct tagBITMAPFILEHEADER
{
	WORD bfType; //文件类型，必须是0x4D42，即字符串"BM"
	DWORD bfSize; //文件大小，包括BITMAPFILEHEADER的14个字节
	WORD bfReserved1; //保留字
	WORD bfReserved2; //保留字
	DWORD bfOffBits; //从文件头到实际的位图数据的偏移字节数
} BITMAPFILEHEADER, FAR *LPBITMAPFILEHEADER, *PBITMAPFILEHEADER;

// 位图信息头BITMAPINFOHEADER也是一个结构体，长度为40字节
typedef struct tagBITMAPINFOHEADER
{
	DWORD biSize; //本结构的长度，为40
	DWORD biWidth; //图像的宽度，单位是像素
	DWORD biHeight; //图像的高度，单位是像素
	WORD biPlanes; //目标设备位平面数，必须是1
	//表示颜色时要用到的位数，1(单色), 4(16色), 8(256色), 24(真彩色)
	WORD biBitCount;
	//指定位图是否压缩，有效的值为BI_RGB，BI_RLE8，BI_RLE4，BI_BITFIELDS等，BI_RGB表示不压缩
	DWORD biCompression;
	//实际的位图数据占用的字节数，即 biSizeImage=biWidth × biHeight，biWidth是biWidth按照4的整倍数调整后的结果
	DWORD biSizeImage;
	DWORD biXPelsPerMeter; //目标设备的水平分辨率，单位是每米的像素个数
	DWORD biYPelsPerMeter; //目标设备的垂直分辨率，单位是每米的像素个数
	//位图实际用到的颜色数，8位图，biClrUsed和biClrImportant两个字段为256，对于16位、24位或32位图像来说，则将它们置为零
	DWORD biClrUsed;
	DWORD biClrImportant; //位图中重要的颜色数，0表示所有颜色都重要
} BITMAPINFOHEADER, FAR *LPBITMAPINFOHEADER, *PBITMAPINFOHEADER;

/*
*  调色板Palette针对的是需要调色板的位图，即单色、16色和256色位图。
*  对于不以调色板方式存储的位图，则无此项信息。
*  调色板是一个数组，共有biClrUsed个元素(如果该值为0，则有2biBitCount个元素)。
*  数组中每个元素是一个RGBQUAD结构体，长度为4个字节
*/
typedef struct tagRGBQUAD
{
	BYTE rgbBlue; //蓝色分量
	BYTE rgbGreen; //绿色分量
	BYTE rgbRed; //红色分量
	BYTE rgbReserved; //保留值
} RGBQUAD;

/*
*  对于用到调色板的位图，实际的图像数据ImageDate为该像素的颜色在调色板中的索引值；
*  对于真彩色图，图像数据则为实际的R、G、B值：
*  a.单色位图：用1bit就可以表示像素的颜色索引值；
*  b.16色位图：用4bit可以表示像素的颜色索引值；
*  c.256色位图：1个字节表示1个像素的颜色索引值；
*  d.真彩色：3个字节表示1个像素的颜色R，G，B值。
*  此外，位图数据每一行的字节数必须为4的整倍数，如果不是，则需要补齐。
*  位图文件中的数据是从下到上（而不是从上到下）、从左到右方式存储的。
*/

/*
*  位图信息段BITMAPINFO包括两部分数组
*  位图信息头指针 & 调色板信息指针（真彩色图片没有调色板信息）
*/
typedef struct tagBITMAPINFO {
	BITMAPINFOHEADER bmiHeader;
	RGBQUAD bmiColors[1];
} BITMAPINFO, FAR *LPBITMAPINFO, *PBITMAPINFO;