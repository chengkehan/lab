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

BOOL jcstring::jcstring_csequ(LPCSTR str1, LPCSTR str2)
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
		return strcmp(str1, str2) == 0;
	}
}

SIZE_T jcstring::jcstring_cslen(LPCSTR str)
{
	if(str == NULL)
	{
		return 0;
	}
	else
	{
		return strlen(str);
	}
}