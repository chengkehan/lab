#ifndef __BITMAPDATA_H__
#define __BITMAPDATA_H__

#include <Windows.h>
#include <WinGDI.h>

namespace JCDD_NS
{
	class JCDD_BitmapData
	{	
	public:
		static CONST INT PALETTE_COLORS_MAX = 256;

	public:
		BITMAPFILEHEADER bmpFileHeader;
		BITMAPINFOHEADER bmpInfoHeader;
		PALETTEENTRY *palette;
		UCHAR *colors;

	public:
		JCDD_BitmapData();
		~JCDD_BitmapData();

	public:
		BOOL create(CHAR* buffer);

	private:
		VOID destroy();
	};

	struct JCDD_BitmapDataXRGB
	{
		BYTE x;
		BYTE b;
		BYTE g;
		BYTE r;
	};

	struct JCDD_BitmapDataRGB
	{
		BYTE b;
		BYTE g;
		BYTE r;
	};

	enum JCDD_BitmapDataBitCount
	{
		BitmapDataBitCount_8 = 8, 
		BitmapDataBitCount_24 = 24, 
		BitmapDataBitCount_32 = 32
	};

	class JCDD_BitmapDataColorIterator32Bit
	{
	public:
		JCDD_BitmapDataColorIterator32Bit(JCDD_BitmapData* bmpd);
		~JCDD_BitmapDataColorIterator32Bit();

	public:
		BOOL hasNext();
		JCDD_BitmapDataXRGB* next();

	private:
		JCDD_BitmapDataColorIterator32Bit();

	private:
		JCDD_BitmapData* bmpd;
		JCDD_BitmapDataXRGB* colors;
		INT numColors;
		INT index;
	};

	class JCDD_BitmapDataColorIterator24Bit
	{
	public:
		JCDD_BitmapDataColorIterator24Bit(JCDD_BitmapData* bmpd);
		~JCDD_BitmapDataColorIterator24Bit();

	public:
		BOOL hasNext();
		JCDD_BitmapDataRGB* next();

	private:
		JCDD_BitmapDataColorIterator24Bit();

	private:
		JCDD_BitmapData* bmpd;
		JCDD_BitmapDataRGB* colors;
		INT numColors;
		INT index;
		INT rowBytesCount;
		INT colsCount;
	};

	class JCDD_BitmapDataColorIterator8Bit
	{
	public:
		JCDD_BitmapDataColorIterator8Bit(JCDD_BitmapData* bmpd);
		~JCDD_BitmapDataColorIterator8Bit();

	public:
		BOOL hasNext();
		UCHAR next();

	private:
		JCDD_BitmapDataColorIterator8Bit();

	private:
		JCDD_BitmapData* bmpd;
		UCHAR* colors;
		INT numColors;
		INT index;
		INT rowBytesCount;
		INT colsCount;
	};
};

#endif