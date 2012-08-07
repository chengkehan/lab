#ifndef __JCDD_UTIL_H__
#define __JCDD_UTIL_H__

#include <Windows.h>
#include <WindowsX.h>
#include <ddraw.h>

namespace JCDD_NS
{
	enum JCDD_SCREEN_BIT_COUNT
	{
		JCDD_SCREEN_BIT_COUNT_32 = 32
	};

	enum JCDD_ERROR_ID
	{
		JCDD_ERROR_ID_NO_ERROR = 0, 
		JCDD_ERROR_ID_INITIALIZE_REPEATING = 1,
		JCDD_ERROR_ID_REGISTER_WND_CLASS = 2, 
		JCDD_ERROR_ID_CREATE_WND = 3, 
		JCDD_ERROR_ID_UPDATE_WND = 4, 
		JCDD_ERROR_ID_ADJUST_WND = 5, 
		JCDD_ERROR_ID_MOVE_WND = 6,
		JCDD_ERROR_ID_CREATE_DDRAW = 7, 
		JCDD_ERROR_ID_COOPERATIVE_LEVEL = 8, 
		JCDD_ERROR_ID_DISPLAY_MODE = 9, 
		JCDD_ERROR_ID_PRIMARY_SURFACE = 10, 
		JCDD_ERROR_ID_PRIMARY_SURFACE_COLOR_KEY = 11, 
		JCDD_ERROR_ID_BACKBUFFER_SURFACE = 12, 
		JCDD_ERROR_ID_BACKBUFFER_SURFACE_COLOR_KEY = 13, 
		JCDD_ERROR_ID_PIXEL_FORMAT = 14, 
		JCDD_ERROR_ID_ILLEGAL_PIXEL_FORMAT = 15, 
		JCDD_ERROR_ID_NULL_INPUT_VALUE = 16
	};

	inline BOOL jcdd_keyDown(INT vkCode)
	{
		return GetAsyncKeyState(vkCode) & 0x8000 ? TRUE : FALSE;
	}

	inline BOOL jcdd_keyUp(INT vkCode)
	{
		return GetAsyncKeyState(vkCode) & 0x8000 ? FALSE : TRUE;
	}

	inline UINT jcdd_rgb32Bit(INT a, INT r, INT g, INT b)
	{
		return (a << 24) + (r << 16) + (g << 8) + b;
	}

	template<class T>
	inline VOID jcdd_free(T** pptr)
	{
		if(*pptr != NULL)
		{
			free (*pptr);
			*pptr = NULL;
		}
	}

	template<class T>
	inline VOID jcdd_delete(T** pptr)
	{
		if(*pptr != NULL)
		{
			delete (*pptr);
			*pptr = NULL;
		}
	}

	template<class T>
	inline VOID jcdd_release(T** pptr)
	{
		if(*pptr != NULL)
		{
			(*pptr)->Release();
			*pptr = NULL;
		}
	}

	template<class T>
	inline VOID jcdd_initStruct(T* ptrStruct)
	{
		memset(ptrStruct, 0, sizeof(*ptrStruct));
		ptrStruct->dwSize = sizeof(*ptrStruct);
	}

	inline BOOL jcdd_success(enum JCDD_ERROR_ID errorID)
	{
		return errorID == JCDD_ERROR_ID_NO_ERROR;
	}

	inline BOOL jcdd_failed(enum JCDD_ERROR_ID errorID)
	{
		return errorID != JCDD_ERROR_ID_NO_ERROR;
	}

	BOOL jcdd_createOffscreenSurface(
		LPDIRECTDRAW7 lpdd, LPDIRECTDRAWSURFACE7* lplpdds, INT width, INT height, UINT colorKey);

	VOID jcdd_deleteOffscreenSurface(LPDIRECTDRAWSURFACE7* lplpdds);
};

#endif