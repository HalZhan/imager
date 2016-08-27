#include "cdib.h"
#include <iostream>
#include <cstdlib>

int main(int argc, char **argv) {
	char readPath[] = "airplane2.bmp";
	char savePath[] = "airplane3.bmp";
	CDib cdib;
	try
	{
		cdib.Load(readPath);
		cdib.Negative();
		cdib.Save(savePath);
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	system("pause");
	return 0;
}