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

	BOOL JCDD_Wrapper::loadBitmapDataFromFile(INT id, LPWCH filePath, UINT colorKey)
	{
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
		
		if(!lpjcdd->createOffscreenSurface(id, bmpd.bmpInfoHeader.biWidth, bmpd.bmpInfoHeader.biHeight, colorKey))
		{
			return FALSE;
		}



		return FALSE;
	}

	LPJCDD JCDD_Wrapper::getJCDD()
	{
		return lpjcdd;
	}
};