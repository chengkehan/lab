// Read 32_color_bit Bitmap File

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <Windows.h>
#include <WinGDI.h>

using namespace std;

int main()
{
	ifstream istream;
	istream.open(L"E:\\lab_git\\readBitmap\\imageSmall.bmp", ifstream::in | ifstream::binary);
	cout << "file open" << endl;
	if(istream.good())
	{
		cout << "good" << endl;

		BITMAPFILEHEADER bmpFileHead;
		istream.read(reinterpret_cast<char*>(&bmpFileHead), sizeof(BITMAPFILEHEADER));
		int m = ((bmpFileHead.bfType & 0xFF00) >> 8);
		int b = (bmpFileHead.bfType & 0x00FF);
		printf("structSize %d\n", sizeof(BITMAPFILEHEADER));
		printf("%x %c\n", b, b);
		printf("%x %c\n", m, m);
		printf("bytes %d\n", bmpFileHead.bfSize);
		printf("reserved1 %d\n", bmpFileHead.bfReserved1);
		printf("reserved2 %d\n", bmpFileHead.bfReserved2);
		printf("bytesOff %d\n", bmpFileHead.bfOffBits);

		BITMAPINFOHEADER bmpInfoHead;
		istream.read(reinterpret_cast<char*>(&bmpInfoHead), sizeof(BITMAPINFOHEADER));
		printf("biSize %d\n", bmpInfoHead.biSize);
		printf("biWidth %d\n", bmpInfoHead.biWidth);
		printf("biHeight %d\n", bmpInfoHead.biHeight);
		printf("biPlanes %d\n", bmpInfoHead.biPlanes);
		printf("biBitCounr %d\n", bmpInfoHead.biBitCount);
		printf("biCompression %d\n", bmpInfoHead.biCompression);
		printf("biSizeImage %d\n", bmpInfoHead.biSizeImage);
		printf("xPelsPerMeter %d\n", bmpInfoHead.biXPelsPerMeter);
		printf("yPelsPerMeter %d\n", bmpInfoHead.biYPelsPerMeter);
		printf("biClrused %d\n", bmpInfoHead.biClrUsed);
		printf("biClrImportant %d\n", bmpInfoHead.biClrImportant);

		RGBQUAD apixel;
		int apixelSize = sizeof(RGBQUAD);
		int numPixels = bmpInfoHead.biWidth * bmpInfoHead.biHeight;
		for (int pixelIndex = 0; pixelIndex < numPixels; ++pixelIndex)
		{
			istream.read(reinterpret_cast<char*>(&apixel), apixelSize);
			printf("R:%x G:%x B:%x\n", apixel.rgbRed, apixel.rgbGreen, apixel.rgbBlue);
		}

		if(istream.good())
		{
			printf("End of File\n");
		}
	}
	istream.close();
	cout << "file close" << endl;

	cin.get();
	return 0;
}