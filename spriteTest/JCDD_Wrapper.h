#ifndef __JCDD_WRAPPER_H__
#define __JCDD_WRAPPER_H__

#include "JCDD.h"

namespace JCDD_NS
{
	typedef class JCDD_Wrapper
	{
	public:
		JCDD_Wrapper(LPJCDD lpjcdd);
		~JCDD_Wrapper();

	public:
		BOOL loadBitmapDataFromFile(INT surfaceID, LPWCH filePath, UINT colorKey);
		BOOL drawBitmapData(INT surfaceID, LPRECT srcRect, LPRECT destRect);
		LPJCDD getJCDD();

	private:
		LPJCDD lpjcdd;
	} JCDD_Wrapper, *LPJCDD_Wrapper;
};

#endif