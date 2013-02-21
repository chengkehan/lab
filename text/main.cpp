#include "jcd3d.h"

using namespace jcd3d;
using namespace jcwin32;

ID3DXFont* lpFont = NULL;

BOOL jcd3d::jcd3d_setup()
{
	D3DXFONT_DESCW fontDesc;
	ZeroMemory(&fontDesc, sizeof(D3DXFONT_DESCA));
	fontDesc.Width = 12;
	fontDesc.Height = 25;
	fontDesc.Weight = 500;
	fontDesc.MipLevels = D3DX_DEFAULT;
	fontDesc.Italic = FALSE;
	fontDesc.CharSet = DEFAULT_CHARSET;
	fontDesc.OutputPrecision = 0;
	fontDesc.Quality = 0;
	fontDesc.PitchAndFamily = 0;
	
	wcscpy(fontDesc.FaceName, L"楷体");
	D3DXCreateFontIndirect(jcd3d_lpd3dd, &fontDesc, &lpFont);

	return TRUE;
}

VOID jcd3d::jcd3d_display(DWORD timeDelta)
{
	if(jcd3d_keyDown(VK_ESCAPE))
	{
		jcwin32_exit();
		return;
	}

	RECT rect;
	rect.left = 100;
	rect.right = 130;
	rect.top = 100;
	rect.bottom = 200;
	const wchar_t* lpStr = L"游戏开发";
	lpFont->DrawText(NULL, lpStr, -1, &rect, DT_CENTER | DT_VCENTER, 0xFFFF0000);
	rect.right = 400;
	rect.bottom = 400;
	lpFont->DrawText(NULL, lpStr, -1, &rect, DT_CENTER | DT_VCENTER, 0xFF00FF00);
}

VOID jcd3d::jcd3d_release()
{
	jccommon_releaseComM(lpFont);
}

INT WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
{
	if(!jcd3d_init(hInstance))
	{
		return 0;
	}
	jcd3d_loop();

	return 0;
}