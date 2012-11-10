#ifndef __JCCOMMON_H__
#define __JCCOMMON_H__

#include <Windows.h>
#include <cassert>

#define jccommon_assertM(c) { assert(c); }

#define jccommon_releaseComM(lp) { if(lp != NULL){ lp->Release(); lp = NULL; } }
#define jccommon_deleteM(lp) { if(lp != NULL){ delete lp; lp = NULL; } }
#define jccommon_deleteArrayM(lp) { if(lp != NULL){ delete[] lp; lp = NULL; } }

namespace jccommon
{
	template<class T>
	VOID jccommon_releaseCom(T lplp)
	{
		if((*lplp) != NULL)
		{
			(*lplp)->Release();
			(*lplp) = NULL;
		}
	}

	template<class T>
	VOID jccommon_delete(T lplp)
	{
		if((*lplp) != NULL)
		{
			delete (*lplp);
			(*lplp) = NULL;
		}
	}

	template<class T>
	VOID jccommon_deleteArray(T lplp)
	{
		if((*lplp) != NULL)
		{
			delete[] (*lplp);
			(*lplp) = NULL;
		}
	}
}

#endif