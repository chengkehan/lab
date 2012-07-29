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

BitmapDataColorIterator32Bit::BitmapDataColorIterator32Bit(BitmapData * bmpd):numColors(0), index(0), colors(NULL)
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
}

BOOL BitmapDataColorIterator32Bit::hasNext()
{
	return index < numColors;
}

BitmapDataXRGB* BitmapDataColorIterator32Bit::next()
{
	return colors == NULL ? NULL : &(colors[index++]);
}