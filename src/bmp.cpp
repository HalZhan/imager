#include "Bmp.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>

using namespace std;

int main(int argc, char **argv) {
	char readPath[] = "snail.bmp"; // ��ȡ·��
	char writePath[] = "snail_binary.bmp"; // д��·��

	BMP bmp;
	bmp.readBmp(readPath);

	// ���ͼ����Ϣ
	printf("width=%d, height=%d, biBitCount=%d\n", bmp.bmpWidth, bmp.bmpHeight, bmp.biBitCount);

	// �ҶȻ�
	bmp.binaryScale();
	// ��ͼ�����ݴ���
	bmp.saveBmp(writePath);

	system("pause");
	return 0;
}