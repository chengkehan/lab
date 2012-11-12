#include "jcstring.h"

BOOL jcstring::jcstring_wcsequ(LPCWCHAR str1, LPCWCHAR str2)
{
	if((str1 == NULL && str2 != NULL) || (str1 != NULL && str2 == NULL))
	{
		return FALSE;
	}
	else if(str1 == NULL && str2 == NULL)
	{
		return TRUE;
	}
	else
	{
		return wcscmp(str1, str2) == 0;
	}
}

SIZE_T jcstring::jcstring_wcslen(LPCWCHAR str)
{
	if(str == NULL)
	{
		return 0;
	}
	else
	{
		return wcslen(str);
	}
}