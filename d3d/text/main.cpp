#include "jcd3d.h"

using namespace jcd3d;

const INT WINDOW_WIDTH = 800;
const INT WINDOW_HEIGHT = 600;

IDirect3DDevice9* lpd3dd = NULL;
ID3DXFont* lpfont = NULL;

RECT g_clientRect;

BOOL jcd3d::jcd3d_setup()
{
	D3DXCreateFont(lpd3dd, 50, 20, 20, 0, FALSE, DEFAULT_CHARSET, 0, 0, 0, L"Arial", &lpfont);
	GetClientRect(jcd3d_hwnd, &g_clientRect);
	return TRUE;
}

VOID jcd3d::jcd3d_display(DWORD timeDelta)
{
	if(lpd3dd != NULL)
	{
		lpd3dd->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
		lpd3dd->BeginScene();

		WCHAR text[] = L"ABCD";
		lpfont->DrawText(NULL, text, wcslen(text), &g_clientRect, DT_SINGLELINE | DT_NOCLIP | DT_CENTER | DT_VCENTER, 0xFFFF0000);

		lpd3dd->EndScene();
		lpd3dd->Present(NULL, NULL, NULL, NULL);
	}
}

VOID jcd3d::jcd3d_release()
{
	jcd3d_releaseHandle(&lpfont);
	jcd3d_releaseHandle(&lpd3dd);
}

INT WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
{
	if(!jcd3d_init(hInstance, WINDOW_WIDTH, WINDOW_HEIGHT, TRUE, D3DDEVTYPE_HAL, &lpd3dd))
	{
		return 0;
	}

	jcd3d_loop(jcd3d_display);
	jcd3d_release();

	return 0;
}