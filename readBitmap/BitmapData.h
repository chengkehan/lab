#ifndef __BITMAPDATA_H__
#define __BITMAPDATA_H__

#include <Windows.h>
#include <WinGDI.h>

class BitmapData
{	
public:
	static CONST INT PALETTE_COLORS_MAX = 256;

public:
	BITMAPFILEHEADER bmpFileHeader;
	BITMAPINFOHEADER bmpInfoHeader;
	PALETTEENTRY *palette;
	CHAR *colors;

public:
	BitmapData();
	~BitmapData();

public:
	BOOL create(CHAR* buffer);

private:
	VOID destroy();
};

struct BitmapDataXRGB
{
	BYTE b;
	BYTE g;
	BYTE r;
	BYTE x;
};

struct BitmapDataRGB
{
	BYTE b;
	BYTE g;
	BYTE r;
};

enum BitmapDataBitCount
{
	BitmapDataBitCount_8 = 8, 
	BitmapDataBitCount_16 = 16, 
	BitmapDataBitCount_24 = 24, 
	BitmapDataBitCount_32 = 32
};

#endif