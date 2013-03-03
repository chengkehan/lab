#include "jcd3d.h"
#include "jcxcommon.h"

using namespace jcd3d;
using namespace jcwin32;

ID3DXFont* lpFont = NULL;
ID3DXSprite* lpSprite = NULL;
IDirect3DVertexBuffer9* lpvb = NULL;
IDirect3DIndexBuffer9* lpib = NULL;
IDirect3DTexture9* lpTexture = NULL;

BOOL jcd3d::jcd3d_setup()
{
	jcd3d_setProjectionPerspectiveTransform(jcd3d_lpd3dd, 800, 600);
	jcd3d_setViewTransform(jcd3d_lpd3dd, 0.0f, 0.0f, -2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	jcd3d_initRenderState(jcd3d_lpd3dd, D3DCULL_CCW, FALSE, FALSE, D3DSHADE_GOURAUD, D3DFILL_SOLID, FALSE);

	xVerifyFailedIf(D3DXCreateTextureFromFileA(jcd3d_lpd3dd, "Tulips.jpg", &lpTexture))xVerifyFailedEndIf

	xVerifyFailedIf(jcd3d_lpd3dd->CreateIndexBuffer(3 * sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &lpib, NULL))xVerifyFailedEndIf
		WORD* lpibData = NULL;
	xVerifyFailedIf(lpib->Lock(0, 3 * sizeof(WORD), (VOID**)&lpibData, NULL))xVerifyFailedEndIf
		lpibData[0] = 0; lpibData[1] = 1; lpibData[2] = 2;
	xVerifyFailedIf(lpib->Unlock())xVerifyFailedEndIf

	xVerifyFailedIf(jcd3d_lpd3dd->CreateVertexBuffer(3 * sizeof(JCD3D_Vertex_xyzrhw_diffuse_texture), D3DUSAGE_WRITEONLY, JCD3D_Vertex_xyzrhw_diffuse_texture::FVF, D3DPOOL_MANAGED, &lpvb, NULL))xVerifyFailedEndIf
		JCD3D_Vertex_xyzrhw_diffuse_texture* lpvbData = NULL;
	xVerifyFailedIf(lpvb->Lock(0, 3 * sizeof(JCD3D_Vertex_xyzrhw_diffuse_texture), (VOID**)&lpvbData, 0))xVerifyFailedEndIf
	lpvbData[0].x = 0.0f; lpvbData[0].y = 0.0f, lpvbData[0].z = 0.0f; lpvbData[0].color = 0xFF0000; lpvbData[0].rhw = 1.0f; lpvbData[0].u = 0.0f; lpvbData[0].v = 0.0f;
	lpvbData[1].x = 400.0f; lpvbData[1].y = 400.0f, lpvbData[1].z = 0.0f; lpvbData[1].color = 0x00FF00; lpvbData[1].rhw = 1.0f; lpvbData[1].u = 1.0f; lpvbData[1].v = 1.0f;
	lpvbData[2].x = 0.0f; lpvbData[2].y = 400.0f, lpvbData[2].z = 0.0f; lpvbData[2].color = 0x0000FF; lpvbData[2].rhw = 1.0f; lpvbData[2].u = 0.0f; lpvbData[2].v = 1.0f;
	xVerifyFailedIf(lpvb->Unlock())xVerifyFailedEndIf

	D3DXCreateSprite(jcd3d_lpd3dd, &lpSprite);

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

	wcscpy(fontDesc.FaceName, L"¿¬Ìå");
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
	rect.left = 0;
	rect.right = 1;
	rect.top = 0;
	rect.bottom = 1;
	const wchar_t* lpStr = L"gamedev";

	jcd3d_lpd3dd->SetTexture(0, lpTexture);
	jcd3d_lpd3dd->SetStreamSource(0, lpvb, 0, sizeof(JCD3D_Vertex_xyzrhw_diffuse_texture));
	jcd3d_lpd3dd->SetFVF(JCD3D_Vertex_xyzrhw_diffuse_texture::FVF);
	jcd3d_lpd3dd->SetIndices(lpib);
	jcd3d_lpd3dd->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 3, 0, 1);

	D3DXMATRIX mat;
	D3DXMatrixTranslation(&mat, 0.0f, 0.0f, 0.0f);
	lpSprite->SetTransform(&mat);
	lpSprite->Begin(D3DXSPRITE_ALPHABLEND);
	lpFont->DrawText(lpSprite, lpStr, -1, &rect, DT_TOP | DT_LEFT | DT_NOCLIP, 0xFFFF0000);
	lpSprite->End();
}

VOID jcd3d::jcd3d_release()
{
	jccommon_releaseComM(lpFont);
	jccommon_releaseComM(lpSprite);
	jccommon_releaseComM(lpvb);
	jccommon_releaseComM(lpib);
	jccommon_releaseComM(lpTexture);
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