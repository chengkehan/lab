#include <Windows.h>
#include <cstdio>
#include <iostream>
#include <fstream>
#include "BitmapData.h"

using namespace std;

int main()
{
	ifstream istream;
	istream.open(L"E:\\lab_git\\readBitmap\\imageSmall.bmp", ifstream::in | ifstream::binary);
	cout << "file open" << endl;
	if(istream.good())
	{
		istream.seekg(0, ios::end);
		INT bmpFileNumBytes = (INT)istream.tellg();
		istream.seekg(0, ios::beg);
		CHAR* bmpFileBytes = (CHAR*)malloc(bmpFileNumBytes);
		istream.read(bmpFileBytes, bmpFileNumBytes);

		BitmapData bmp;
		bmp.create(bmpFileBytes);
		int m = ((bmp.bmpFileHeader.bfType & 0xFF00) >> 8);
		int b = (bmp.bmpFileHeader.bfType & 0x00FF);
		printf("%x %c\n", b, b);
		printf("%x %c\n", m, m);
		printf("bytes %d\n", bmp.bmpFileHeader.bfSize);
		printf("reserved1 %d\n", bmp.bmpFileHeader.bfReserved1);
		printf("reserved2 %d\n", bmp.bmpFileHeader.bfReserved2);
		printf("bytesOff %d\n", bmp.bmpFileHeader.bfOffBits);

		printf("biSize %d\n", bmp.bmpInfoHeader.biSize);
		printf("biWidth %d\n", bmp.bmpInfoHeader.biWidth);
		printf("biHeight %d\n", bmp.bmpInfoHeader.biHeight);
		printf("biPlanes %d\n", bmp.bmpInfoHeader.biPlanes);
		printf("biBitCount %d\n", bmp.bmpInfoHeader.biBitCount);
		printf("biCompression %d\n", bmp.bmpInfoHeader.biCompression);
		printf("biSizeImage %d\n", bmp.bmpInfoHeader.biSizeImage);
		printf("xPelsPerMeter %d\n", bmp.bmpInfoHeader.biXPelsPerMeter);
		printf("yPelsPerMeter %d\n", bmp.bmpInfoHeader.biYPelsPerMeter);
		printf("biClrused %d\n", bmp.bmpInfoHeader.biClrUsed);
		printf("biClrImportant %d\n", bmp.bmpInfoHeader.biClrImportant);

		/*BitmapDataXRGB* colors = (BitmapDataXRGB*)bmp.colors;
		INT numColors = bmp.bmpInfoHeader.biWidth * bmp.bmpInfoHeader.biHeight;
		for (INT i = 0; i < numColors; ++i)
		{
			printf("R:%x G:%x B:%x\n", colors[i].r, colors[i].g, colors[i].b);
		}*/
		BitmapDataColorIterator32Bit i32(&bmp);
		while(i32.hasNext())
		{
			BitmapDataXRGB* i32Color = i32.next();
			printf("R:%x G:%x B:%x\n", i32Color->r, i32Color->g, i32Color->b);
		}

		/*BitmapDataRGB* colors = (BitmapDataRGB*)bmp.colors;
		INT numColors = bmp.bmpInfoHeader.biWidth * bmp.bmpInfoHeader.biHeight;
		INT cols = 0;
		INT bytesCount = 0;
		for (INT i = 0; i < numColors; ++i)
		{
			printf("R:%x G:%x B:%x\n", colors[i].r, colors[i].g, colors[i].b);
			bytesCount += sizeof(colors[i].r);
			bytesCount += sizeof(colors[i].g);
			bytesCount += sizeof(colors[i].b);

			if(++cols == bmp.bmpInfoHeader.biWidth)
			{
				if(bytesCount % 4 != 0)
				{
					INT bytesOff = (((INT)(bytesCount / 4)) + 1) * 4 - bytesCount;
					CHAR* colorsTmp = (CHAR*)colors;
					colorsTmp += bytesOff;
					colors = (BitmapDataRGB*)colorsTmp;
				}
				bytesCount = 0;
				cols = 0;
			}
		}*/

		/*CHAR* colors = bmp.colors;
		INT cols = 0;
		INT bytesCount = 0;
		INT biSizeImage = bmp.bmpInfoHeader.biWidth % 4 == 0 ? bmp.bmpInfoHeader.biWidth * bmp.bmpInfoHeader.biHeight : (INT(bmp.bmpInfoHeader.biWidth / 4) + 1) * 4 * bmp.bmpInfoHeader.biHeight;
		for (INT i = 0; i < biSizeImage; ++i)
		{
		PALETTEENTRY color = bmp.palette[colors[i]];
		printf("R:%x G:%x B:%x\n", color.peRed, color.peGreen, color.peBlue);
		++bytesCount;

		if(++cols == bmp.bmpInfoHeader.biWidth)
		{
		if(bytesCount % 4 != 0)
		{
		INT bytesOff = (INT(bytesCount / 4) + 1) * 4 - bytesCount;
		i += bytesOff;
		}
		bytesCount = 0;
		cols = 0;
		}
		}*/

		free(bmpFileBytes);
	}

	istream.close();
	cout << "file close" << endl;

	getchar();
	return 0;
}