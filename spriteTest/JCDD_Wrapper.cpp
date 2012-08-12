#include "JCDD_Wrapper.h"
#include "JCDD_IO.h"
#include "JCDD_BitmapData.h"

namespace JCDD_NS
{
	JCDD_Wrapper::JCDD_Wrapper(LPJCDD lpjcdd)
	{
		this->lpjcdd = lpjcdd;
	}

	JCDD_Wrapper::~JCDD_Wrapper()
	{
		lpjcdd = NULL;
	}

	BOOL JCDD_Wrapper::loadBitmapDataFromFile(INT surfaceID, LPWCH filePath, UINT colorKey)
	{
		if(lpjcdd->containsTheOffscreenSurface(surfaceID))
		{
			return FALSE;
		}

		JCDD_File file;
		if(!file.loadData(filePath))
		{
			return FALSE;
		}

		JCDD_BitmapData bmpd;
		if(!bmpd.create(file.getData()))
		{
			return FALSE;
		}
		BOOL reverseRow = bmpd.bmpInfoHeader.biHeight < 0;
		INT biWidth = bmpd.bmpInfoHeader.biWidth;
		INT biHeight = reverseRow ? -bmpd.bmpInfoHeader.biHeight : bmpd.bmpInfoHeader.biHeight;
		
		if(bmpd.bmpInfoHeader.biBitCount != BitmapDataBitCount_8 && 
			bmpd.bmpInfoHeader.biBitCount != BitmapDataBitCount_24 && 
			bmpd.bmpInfoHeader.biBitCount != BitmapDataBitCount_32)
		{
			return FALSE;
		}

		if(!lpjcdd->createOffscreenSurface(surfaceID, bmpd.bmpInfoHeader.biWidth, biHeight, colorKey))
		{
			return FALSE;
		}

		LPJCDD_Surface surface = lpjcdd->getOffscreenSurface(surfaceID);
		LPDIRECTDRAWSURFACE7 lpdds = surface->getSurface();
		DDSURFACEDESC2 ddsd;
		jcdd_initStruct(&ddsd);
		if(FAILED(lpdds->Lock(NULL, &ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL)))
		{
			lpjcdd->deleteOffscreenSurface(surfaceID);
			return FALSE;
		}

		UINT* buffer = (UINT*)ddsd.lpSurface;
		INT pixelWidth = bmpd.bmpInfoHeader.biWidth;
		INT pixelHeight = biHeight;
		INT pixelWidthCount = 0;
		INT pixelHeightCount = biHeight - 1;
		INT pitch = ddsd.lPitch >> 2;

		if(bmpd.bmpInfoHeader.biBitCount == BitmapDataBitCount_8)
		{
			JCDD_BitmapDataColorIterator8Bit it(&bmpd);
			while(it.hasNext())
			{
				PALETTEENTRY color = bmpd.palette[it.next()];
				buffer[pixelWidthCount + pixelHeightCount * pitch] = jcdd_rgb32Bit(0x00, color.peRed, color.peGreen, color.peBlue);
				if(++pixelWidthCount == pixelWidth)
				{
					pixelWidthCount = 0;
					--pixelHeightCount;
				}
			}
		}
		else if(bmpd.bmpInfoHeader.biBitCount == BitmapDataBitCount_24)
		{
			JCDD_BitmapDataColorIterator24Bit it(&bmpd);
			while(it.hasNext())
			{
				JCDD_BitmapDataRGB* color = it.next();
				buffer[pixelWidthCount + pixelHeightCount * pitch] = jcdd_rgb32Bit(0x00, color->r, color->g, color->b);
				if(++pixelWidthCount == pixelWidth)
				{
					pixelWidthCount = 0;
					--pixelHeightCount;
				}
			}
		}
		else if(bmpd.bmpInfoHeader.biBitCount == BitmapDataBitCount_32)
		{
			JCDD_BitmapDataColorIterator32Bit it(&bmpd);
			while(it.hasNext())
			{
				JCDD_BitmapDataXRGB* color = it.next();
				buffer[pixelWidthCount + pixelHeightCount * pitch] = jcdd_rgb32Bit(0x00, color->r, color->g, color->b);
				if(++pixelWidthCount == pixelWidth)
				{
					pixelWidthCount = 0;
					--pixelHeightCount;
				}
			}
		}

		if(FAILED(lpdds->Unlock(NULL)))
		{
			lpjcdd->deleteOffscreenSurface(surfaceID);
			return FALSE;
		}

		return TRUE;
	}

	BOOL JCDD_Wrapper::drawBitmapData(INT surfaceID, LPRECT srcRect, LPRECT destRect)
	{
		if(!lpjcdd->containsTheOffscreenSurface(surfaceID))
		{
			return FALSE;
		}

		LPDIRECTDRAWSURFACE7 dest = lpjcdd->getBackBufferSurface();
		LPDIRECTDRAWSURFACE7 src = lpjcdd->getOffscreenSurface(surfaceID)->getSurface();
		if(FAILED(dest->Blt(destRect, src, srcRect, DDBLT_WAIT, NULL)))
		{
			return FALSE;
		}

		return TRUE;
	}

	LPJCDD JCDD_Wrapper::getJCDD()
	{
		return lpjcdd;
	}
};