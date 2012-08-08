#include "JCDD_Util.h"

namespace JCDD_NS
{
	BOOL jcdd_createOffscreenSurface(
		LPDIRECTDRAW7 lpdd, LPDIRECTDRAWSURFACE7* lplpdds, INT width, INT height, UINT colorKey)
	{
		if(lpdd == NULL)
		{
			return FALSE;
		}

		DDSURFACEDESC2 ddsd;
		jcdd_initStruct(&ddsd);
		ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
		ddsd.dwWidth = width;
		ddsd.dwHeight = height;
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
		if(FAILED(lpdd->CreateSurface(&ddsd, lplpdds, NULL)))
		{
			return FALSE;
		}

		DDCOLORKEY ck;
		ck.dwColorSpaceHighValue = colorKey;
		ck.dwColorSpaceLowValue = colorKey;
		if(FAILED((*lplpdds)->SetColorKey(DDCKEY_SRCBLT, &ck)))
		{
			return FALSE;
		}

		return TRUE;
	}

	VOID jcdd_deleteOffscreenSurface(LPDIRECTDRAWSURFACE7* lplpdds)
	{
		jcdd_release(lplpdds);
	}

	BOOL jcdd_createClipper(LPDIRECTDRAW7 lpdd, LPDIRECTDRAWSURFACE7 lpdds, LPDIRECTDRAWCLIPPER* lplpddc, INT numRect, LPRECT rectList)
	{
		if(lpdd == NULL)
		{
			return FALSE;
		}

		if(lpdds == NULL)
		{
			return FALSE;
		}

		if(FAILED(lpdd->CreateClipper(0, lplpddc, NULL)))
		{
			return FALSE;
		}

		LPRGNDATA regionData;
		regionData = (LPRGNDATA)malloc(sizeof(RGNDATAHEADER) + numRect * sizeof(RECT));
		memcpy(regionData->Buffer, rectList, numRect * sizeof(RECT));

		regionData->rdh.dwSize = sizeof(RGNDATAHEADER);
		regionData->rdh.iType = RDH_RECTANGLES;
		regionData->rdh.nCount = numRect;
		regionData->rdh.nRgnSize = sizeof(RECT) * numRect;
		regionData->rdh.rcBound.left = LONG_MAX;
		regionData->rdh.rcBound.top = LONG_MAX;
		regionData->rdh.rcBound.right = LONG_MIN;
		regionData->rdh.rcBound.bottom = LONG_MIN;

		for (INT i = 0; i < numRect; ++i)
		{
			if(rectList[i].left < regionData->rdh.rcBound.left)
			{
				regionData->rdh.rcBound.left = rectList[i].left;
			}
			if(rectList[i].top < regionData->rdh.rcBound.top)
			{
				regionData->rdh.rcBound.top = rectList[i].top;
			}
			if(rectList[i].right > regionData->rdh.rcBound.right)
			{
				regionData->rdh.rcBound.right = rectList[i].right;
			}
			if(rectList[i].bottom > regionData->rdh.rcBound.bottom)
			{
				regionData->rdh.rcBound.bottom = rectList[i].bottom;
			}
		}

		if(FAILED((*lplpddc)->SetClipList(regionData, 0)))
		{
			free(regionData);
			return NULL;
		}
		if(FAILED(lpdds->SetClipper(*lplpddc)))
		{
			free(regionData);
			return NULL;
		}
		free(regionData);

		return TRUE;
	}

	VOID jcdd_deleteClipper(LPDIRECTDRAWCLIPPER* lplpddc)
	{
		jcdd_release(lplpddc);
	}

	BOOL jcdd_colorFillSurface(LPDIRECTDRAWSURFACE7 lpdds, INT width, INT height, UINT color)
	{
		if(lpdds == NULL)
		{
			return FALSE;
		}
		RECT destRect = {0, 0, width, height};
		DDBLTFX ddbltfx;
		jcdd_initStruct(&ddbltfx);
		ddbltfx.dwFillColor = color;
		if(FAILED(lpdds->Blt(&destRect, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddbltfx)))
		{
			return FALSE;
		}

		return TRUE;
	}
};