#include "BitmapData.h"

BitmapData::BitmapData():palette(NULL), colors(NULL)
{

}

BitmapData::~BitmapData()
{
	destroy();
}

BOOL BitmapData::create(CHAR* buffer)
{
	if(buffer == NULL)
	{
		return FALSE;
	}
	else
	{
		destroy();

		CHAR* input = buffer;

		memcpy(&bmpFileHeader, input, sizeof(BITMAPFILEHEADER));
		input += sizeof(BITMAPFILEHEADER);
		memcpy(&bmpInfoHeader, input, sizeof(BITMAPINFOHEADER));
		input += sizeof(BITMAPINFOHEADER);

		if(bmpInfoHeader.biBitCount == BitmapDataBitCount_8)
		{
			INT paletteNumColors = bmpInfoHeader.biClrUsed == 0 ? BitmapData::PALETTE_COLORS_MAX : bmpInfoHeader.biClrUsed;
			
			if(paletteNumColors > BitmapData::PALETTE_COLORS_MAX)
			{
				return FALSE;
			}

			palette = (PALETTEENTRY*)malloc(sizeof(PALETTEENTRY) * paletteNumColors);
			if(palette == NULL)
			{
				return FALSE;
			}

			memcpy(palette, input, sizeof(PALETTEENTRY) * paletteNumColors);

			for (INT i = 0; i < paletteNumColors; ++i)
			{
				INT tempColor = palette[i].peRed;
				palette[i].peRed = palette[i].peBlue;
				palette[i].peBlue = tempColor;
				palette[i].peFlags = PC_NOCOLLAPSE;
			}
		}

		if(bmpInfoHeader.biBitCount == BitmapDataBitCount_8 || bmpInfoHeader.biBitCount == BitmapDataBitCount_24 || bmpInfoHeader.biBitCount == BitmapDataBitCount_32)
		{
			input = buffer + bmpFileHeader.bfOffBits;
			colors = (CHAR*)malloc(bmpInfoHeader.biSizeImage);
			if(colors == NULL)
			{
				destroy();
				return FALSE;
			}

			memcpy(colors, input, bmpInfoHeader.biSizeImage);

			return TRUE;
		}
		else
		{
			destroy();
			return FALSE;
		}
	}
}

VOID BitmapData::destroy()
{
	if(palette != NULL)
	{
		free(palette);
		palette = NULL;
	}
	if(colors != NULL)
	{
		free(colors);
		colors = NULL;
	}
}

BitmapDataColorIterator32Bit::BitmapDataColorIterator32Bit(BitmapData* bmpd):numColors(0), index(0), colors(NULL)
{
	this->bmpd = bmpd;
	if(bmpd != NULL)
	{
		numColors = bmpd->bmpInfoHeader.biWidth * bmpd->bmpInfoHeader.biHeight;
		colors = (BitmapDataXRGB*)bmpd->colors;
	}
}

BitmapDataColorIterator32Bit::~BitmapDataColorIterator32Bit()
{
	bmpd = NULL;
	colors = NULL;
}

BOOL BitmapDataColorIterator32Bit::hasNext()
{
	return index < numColors;
}

BitmapDataXRGB* BitmapDataColorIterator32Bit::next()
{
	BitmapDataXRGB* color = colors + index;
	++index;

	return color;
}

BitmapDataColorIterator24Bit::BitmapDataColorIterator24Bit(BitmapData* bmpd):numColors(0), index(0), colors(NULL), rowBytesCount(0), colsCount(0)
{
	this->bmpd = bmpd;
	if(bmpd != NULL)
	{
		numColors = bmpd->bmpInfoHeader.biWidth * bmpd->bmpInfoHeader.biHeight;
		colors = (BitmapDataRGB*)bmpd->colors;
	}
}

BitmapDataColorIterator24Bit::~BitmapDataColorIterator24Bit()
{
	bmpd = NULL;
	colors = NULL;
}

BOOL BitmapDataColorIterator24Bit::hasNext()
{
	return index < numColors;
}

BitmapDataRGB* BitmapDataColorIterator24Bit::next()
{
	if(colors == NULL)
	{
		return NULL;
	}
	else
	{
		BitmapDataRGB* color = colors + index;
		++index;
		rowBytesCount += sizeof(BitmapDataRGB);
		if(++colsCount == bmpd->bmpInfoHeader.biWidth)
		{
			if(rowBytesCount % 4 != 0)
			{
				INT bytesOff = (((INT)(rowBytesCount / 4)) + 1) * 4 - rowBytesCount;
				CHAR* colorsTmp = (CHAR*)colors;
				colorsTmp += bytesOff;
				colors = (BitmapDataRGB*)colorsTmp;
			}
			rowBytesCount = 0;
			colsCount = 0;
		}

		return color;
	}
}

BitmapDataColorIterator8Bit::BitmapDataColorIterator8Bit(BitmapData* bmpd):numColors(0), index(0), colors(NULL), rowBytesCount(0), colsCount(0), palette(NULL)
{
	this->bmpd = bmpd;
	if(bmpd != NULL)
	{
		colors = bmpd->colors;
		palette = bmpd->palette;
		numColors = 
			bmpd->bmpInfoHeader.biWidth % 4 == 0 ? 
			bmpd->bmpInfoHeader.biWidth * bmpd->bmpInfoHeader.biHeight : 
			(INT(bmpd->bmpInfoHeader.biWidth / 4) + 1) * 4 * bmpd->bmpInfoHeader.biHeight;
	}
}

BitmapDataColorIterator8Bit::~BitmapDataColorIterator8Bit()
{
	bmpd = NULL;
	colors = NULL;
	palette = NULL;
}

BOOL BitmapDataColorIterator8Bit::hasNext()
{
	return index < numColors;
}

PALETTEENTRY* BitmapDataColorIterator8Bit::next()
{
	if(colors == NULL)
	{
		return NULL;
	}
	else
	{
		PALETTEENTRY* color = &(palette[colors[index]]);
		++index;
		++rowBytesCount;
		if(++colsCount == bmpd->bmpInfoHeader.biWidth)
		{
			if(rowBytesCount % 4 != 0)
			{
				INT bytesOff = (INT(rowBytesCount / 4) + 1) * 4 - rowBytesCount;
				index += bytesOff;
			}
			rowBytesCount = 0;
			colsCount = 0;
		}

		return color;
	}
}