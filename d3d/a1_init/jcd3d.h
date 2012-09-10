#ifndef __JCD3D_H__
#define __JCD3D_H__

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>

namespace jcd3d
{
	BOOL jcd3d_init(HINSTANCE hInstance, INT width, INT height, BOOL windowed, D3DDEVTYPE deviceType, LPDIRECT3DDEVICE9* device);

	LRESULT CALLBACK jcd3d_wndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	VOID jcd3d_loop(VOID (*display)(DWORD timeDelta));
}

#endif