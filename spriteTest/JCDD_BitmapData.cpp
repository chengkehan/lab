#include "JCDD_BitmapData.h"

namespace JCDD_NS
{
	JCDD_BitmapData::JCDD_BitmapData():palette(NULL), colors(NULL)
	{

	}

	JCDD_BitmapData::~JCDD_BitmapData()
	{
		destroy();
	}

	BOOL JCDD_BitmapData::create(CHAR* buffer)
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
				INT paletteNumColors = bmpInfoHeader.biClrUsed == 0 ? JCDD_BitmapData::PALETTE_COLORS_MAX : bmpInfoHeader.biClrUsed;

				if(paletteNumColors > JCDD_BitmapData::PALETTE_COLORS_MAX)
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
				colors = (UCHAR*)malloc(bmpInfoHeader.biSizeImage);
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

	VOID JCDD_BitmapData::destroy()
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

	JCDD_BitmapDataColorIterator32Bit::JCDD_BitmapDataColorIterator32Bit(JCDD_BitmapData* bmpd):numColors(0), index(0), colors(NULL)
	{
		this->bmpd = bmpd;
		if(bmpd != NULL)
		{
			numColors = bmpd->bmpInfoHeader.biWidth * bmpd->bmpInfoHeader.biHeight;
			colors = (JCDD_BitmapDataXRGB*)bmpd->colors;
		}
	}

	JCDD_BitmapDataColorIterator32Bit::~JCDD_BitmapDataColorIterator32Bit()
	{
		bmpd = NULL;
		colors = NULL;
	}

	BOOL JCDD_BitmapDataColorIterator32Bit::hasNext()
	{
		return index < numColors;
	}

	JCDD_BitmapDataXRGB* JCDD_BitmapDataColorIterator32Bit::next()
	{
		JCDD_BitmapDataXRGB* color = colors + index;
		++index;

		return color;
	}

	JCDD_BitmapDataColorIterator24Bit::JCDD_BitmapDataColorIterator24Bit(JCDD_BitmapData* bmpd):numColors(0), index(0), colors(NULL), rowBytesCount(0), colsCount(0)
	{
		this->bmpd = bmpd;
		if(bmpd != NULL)
		{
			numColors = bmpd->bmpInfoHeader.biWidth * bmpd->bmpInfoHeader.biHeight;
			colors = (JCDD_BitmapDataRGB*)bmpd->colors;
		}
	}

	JCDD_BitmapDataColorIterator24Bit::~JCDD_BitmapDataColorIterator24Bit()
	{
		bmpd = NULL;
		colors = NULL;
	}

	BOOL JCDD_BitmapDataColorIterator24Bit::hasNext()
	{
		return index < numColors;
	}

	JCDD_BitmapDataRGB* JCDD_BitmapDataColorIterator24Bit::next()
	{
		if(colors == NULL)
		{
			return NULL;
		}
		else
		{
			JCDD_BitmapDataRGB* color = colors + index;
			++index;
			rowBytesCount += sizeof(JCDD_BitmapDataRGB);
			if(++colsCount == bmpd->bmpInfoHeader.biWidth)
			{
				if(rowBytesCount % 4 != 0)
				{
					INT bytesOff = (((INT)(rowBytesCount / 4)) + 1) * 4 - rowBytesCount;
					CHAR* colorsTmp = (CHAR*)colors;
					colorsTmp += bytesOff;
					colors = (JCDD_BitmapDataRGB*)colorsTmp;
				}
				rowBytesCount = 0;
				colsCount = 0;
			}

			return color;
		}
	}

	JCDD_BitmapDataColorIterator8Bit::JCDD_BitmapDataColorIterator8Bit(JCDD_BitmapData* bmpd):numColors(0), index(0), colors(NULL), rowBytesCount(0), colsCount(0)
	{
		this->bmpd = bmpd;
		if(bmpd != NULL)
		{
			colors = bmpd->colors;
			numColors = 
				bmpd->bmpInfoHeader.biWidth % 4 == 0 ? 
				bmpd->bmpInfoHeader.biWidth * bmpd->bmpInfoHeader.biHeight : 
			(INT(bmpd->bmpInfoHeader.biWidth / 4) + 1) * 4 * bmpd->bmpInfoHeader.biHeight;
		}
	}

	JCDD_BitmapDataColorIterator8Bit::~JCDD_BitmapDataColorIterator8Bit()
	{
		bmpd = NULL;
		colors = NULL;
	}

	BOOL JCDD_BitmapDataColorIterator8Bit::hasNext()
	{
		return index < numColors;
	}

	UCHAR JCDD_BitmapDataColorIterator8Bit::next()
	{
		if(colors == NULL)
		{
			return NULL;
		}
		else
		{
			UCHAR color = colors[index];
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
};