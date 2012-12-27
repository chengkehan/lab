#ifndef __JC_STRING_H__
#define __JC_STRING_H__

#include <Windows.h>

namespace jcstring
{
	BOOL jcstring_wcsequ(LPCWCHAR str1, LPCWCHAR str2);
	SIZE_T jcstring_wcslen(LPCWCHAR str);
	BOOL jcstring_csequ(LPCSTR str1, LPCSTR str2);
	SIZE_T jcstring_cslen(LPCSTR str);
}

#endif