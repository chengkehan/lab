#ifndef __JCD3D_H__
#define __JCD3D_H__

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>

namespace jcd3d
{
	// Util
	inline BOOL jcd3d_keyDown(INT vkCode)
	{
		return GetAsyncKeyState(vkCode) & 0x8000 ? TRUE : FALSE;
	}

	inline BOOL jcd3d_keyUp(INT vkCode)
	{
		return GetAsyncKeyState(vkCode) & 0x8000 ? FALSE : TRUE;
	}

	// D3D
	BOOL jcd3d_init(HINSTANCE hInstance, INT width, INT height, BOOL windowed, D3DDEVTYPE deviceType, LPDIRECT3DDEVICE9* device);
	BOOL jcd3d_setup();
	VOID jcd3d_release();
	VOID jcd3d_display(DWORD timeDelta);

	LRESULT CALLBACK jcd3d_wndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	VOID jcd3d_loop(VOID (*display)(DWORD timeDelta));

	template<class T>
	VOID jcd3d_releaseHandle(T handle)
	{
		if((*handle) != NULL)
		{
			(*handle)->Release();
			(*handle) = NULL;
		}
	}

	// Color
	const D3DXCOLOR JCD3D_WHITE(D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));
	const D3DXCOLOR JCD3D_BLACK(D3DCOLOR_XRGB(0x00, 0x00, 0x00));
	const D3DXCOLOR JCD3D_RED(D3DCOLOR_XRGB(0xFF, 0, 0));
	const D3DXCOLOR JCD3D_GREEN(D3DCOLOR_XRGB(0, 0xFF, 0));
	const D3DXCOLOR JCD3D_BLUE(D3DCOLOR_XRGB(0, 0, 0xFF));
	const D3DXCOLOR JCD3D_YELLOW(D3DCOLOR_XRGB(0xFF, 0xFF, 0));
	const D3DXCOLOR JCD3D_CYAN(D3DCOLOR_XRGB(0, 0xFF, 0xFF));
	const D3DXCOLOR JCD3D_MAGENTA(D3DCOLOR_XRGB(0xFF, 0, 0xFF));

	// Material
	D3DMATERIAL9 jcd3d_initMaterial(D3DXCOLOR ambient, D3DXCOLOR diffuse, D3DXCOLOR specular, D3DXCOLOR emissive, FLOAT power);
	const D3DMATERIAL9 JCD3D_WHITE_MATERIAL = jcd3d_initMaterial(JCD3D_WHITE, JCD3D_WHITE, JCD3D_WHITE, JCD3D_BLACK, 2.0f);
	const D3DMATERIAL9 JCD3D_RED_MATERIAL = jcd3d_initMaterial(JCD3D_RED, JCD3D_RED, JCD3D_RED, JCD3D_BLACK, 2.0f);
	const D3DMATERIAL9 JCD3D_GREEN_MATERIAL = jcd3d_initMaterial(JCD3D_GREEN, JCD3D_GREEN, JCD3D_GREEN, JCD3D_BLACK, 2.0f);
	const D3DMATERIAL9 JCD3D_BLUE_MATERIAL = jcd3d_initMaterial(JCD3D_BLUE, JCD3D_BLUE, JCD3D_BLUE, JCD3D_BLACK, 2.0f);
	const D3DMATERIAL9 JCD3D_YELLOW_MATERIAL = jcd3d_initMaterial(JCD3D_YELLOW, JCD3D_YELLOW, JCD3D_YELLOW, JCD3D_BLACK, 2.0f);

	// Light
	D3DLIGHT9 jcd3d_initDirectionalLight(D3DXVECTOR3* direction, D3DXCOLOR* color);
	D3DLIGHT9 jcd3d_initPointLight(D3DXVECTOR3* position, D3DXCOLOR* color);
	D3DLIGHT9 jcd3d_initSpotLight(D3DXVECTOR3* position, D3DXVECTOR3* direction, D3DXCOLOR* color);
}

#endif