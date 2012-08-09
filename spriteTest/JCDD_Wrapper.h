#ifndef __JCDD_WRAPPER_H__
#define __JCDD_WRAPPER_H__

#include "JCDD.h"

namespace JCDD_NS
{
	class JCDD_Wrapper
	{
	public:
		JCDD_Wrapper(LPJCDD lpjcdd);
		~JCDD_Wrapper();

	public:
		BOOL loadBitmapDataFromFile(INT id, LPWCH filePath, UINT colorKey);

	public:
		LPJCDD getJCDD();

	private:
		LPJCDD lpjcdd;
	};
};

#endif