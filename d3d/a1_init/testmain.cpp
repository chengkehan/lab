#include <Windows.h>
#include "jcd3d.h"

LPDIRECT3DDEVICE9 lpd3dd = NULL;

LRESULT CALLBACK jcd3d::jcd3d_wndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}
	case WM_KEYDOWN:
		{
			if(wparam == VK_ESCAPE)
			{
				DestroyWindow(hwnd);
			}
			break;
		}
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

BOOL jcd3d_setup()
{
	return TRUE;
}

VOID jcd3d_release()
{
	if(lpd3dd)
	{
		lpd3dd->Release();
		lpd3dd = NULL;
	}
}

VOID jcd3d_display(DWORD timeDelta)
{
	if(lpd3dd)
	{
		lpd3dd->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);

		lpd3dd->Present(NULL, NULL, NULL, NULL);
	}
}

INT WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
{
	if(!jcd3d::jcd3d_init(hInstance, 800, 600, true, D3DDEVTYPE_HAL, &lpd3dd))
	{
		return 0;
	}

	if(!jcd3d_setup())
	{
		return 0;
	}

	jcd3d::jcd3d_loop(jcd3d_display);

	jcd3d_release();

	return 0;
}