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
	BitmapDataBitCount_24 = 24, 
	BitmapDataBitCount_32 = 32
};

class BitmapDataColorIterator32Bit
{
public:
	BitmapDataColorIterator32Bit(BitmapData* bmpd);
	~BitmapDataColorIterator32Bit();

public:
	BOOL hasNext();
	BitmapDataXRGB* next();

private:
	BitmapDataColorIterator32Bit();

private:
	BitmapData* bmpd;
	BitmapDataXRGB* colors;
	INT numColors;
	INT index;
};

class BitmapDataColorIterator24Bit
{
public:
	BitmapDataColorIterator24Bit(BitmapData* bmpd);
	~BitmapDataColorIterator24Bit();

public:
	BOOL hasNext();
	BitmapDataRGB* next();

private:
	BitmapDataColorIterator24Bit();

private:
	BitmapData* bmpd;
	BitmapDataRGB* colors;
	INT numColors;
	INT index;
	INT rowBytesCount;
	INT colsCount;
};

class BitmapDataColorIterator8Bit
{
public:
	BitmapDataColorIterator8Bit(BitmapData* bmpd);
	~BitmapDataColorIterator8Bit();

public:
	BOOL hasNext();
	PALETTEENTRY* next();

private:
	BitmapDataColorIterator8Bit();

private:
	BitmapData* bmpd;
	PALETTEENTRY* palette;
	CHAR* colors;
	INT numColors;
	INT index;
	INT rowBytesCount;
	INT colsCount;
};

#endif