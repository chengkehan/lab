#include "JCDD_Util.h"

namespace JCDD_NS
{
	BOOL createOffscreenSurface(
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

	VOID deleteOffscreenSurface(LPDIRECTDRAWSURFACE7* lplpdds)
	{
		jcdd_release(lplpdds);
	}
};