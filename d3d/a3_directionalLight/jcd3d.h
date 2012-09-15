#ifndef __JCD3D_H__
#define __JCD3D_H__

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>

namespace jcd3d
{
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

	template<class T>
	VOID jcd3d_delete(T handle)
	{
		if((*handle) != NULL)
		{
			delete (*handle);
			(*handle) = NULL;
		}
	}

	D3DLIGHT9* jcd3d_initDirectionalLight(D3DLIGHT9* lpLight, D3DXVECTOR3* direction, D3DXCOLOR* color);

	D3DMATERIAL9* jcd3d_initMaterial(D3DMATERIAL9* lpMaterial, D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, FLOAT p);
}

#endif