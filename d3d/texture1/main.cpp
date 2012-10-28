#include "jcd3d.h"

using namespace jcd3d;

IDirect3DVertexBuffer9* lpVbCanvas = NULL;
IDirect3DTexture9* lpTextureWall = NULL;
IDirect3DTexture9* lpTextureLight = NULL;

const INT WINDOW_WIDTH = 800;
const INT WINDOW_HEIGHT = 600;

BOOL jcd3d::jcd3d_setup()
{
	// Canvas
	jcd3d_lpd3dd->CreateVertexBuffer(
		4 * sizeof(JCD3D_Vertex_xyz_diffuse_texture2), D3DUSAGE_WRITEONLY, JCD3D_Vertex_xyz_diffuse_texture2::FVF, D3DPOOL_MANAGED, &lpVbCanvas, NULL
	);
	JCD3D_Vertex_xyz_diffuse_texture2 canvasVbList[] = {
		JCD3D_Vertex_xyz_diffuse_texture2(-3.0f, -3.0f, 0.0f, 0xFF0099FF, 0.0f, 1.0f, 0.0f, 1.0f), 
		JCD3D_Vertex_xyz_diffuse_texture2(-3.0f, 3.0f, 0.0f, 0xFF0099FF, 0.0f, 0.0f, 0.0f, 0.0f), 
		JCD3D_Vertex_xyz_diffuse_texture2(3.0f, -3.0f, 0.0f, 0xFF0099FF, 1.0f, 1.0f, 1.0f, 1.0f), 
		JCD3D_Vertex_xyz_diffuse_texture2(3.0f, 3.0f, 0.0f, 0xFF0099FF, 0.0f, 1.0f, 0.0f, 1.0f), 
	};
	VOID* canvasVb;
	lpVbCanvas->Lock(0, 4 * sizeof(JCD3D_Vertex_xyz_diffuse_texture2), &canvasVb, 0);
	memcpy(canvasVb, canvasVbList, 4 * sizeof(JCD3D_Vertex_xyz_diffuse_texture2));
	lpVbCanvas->Unlock();

	// Texture
	if(FAILED(D3DXCreateTextureFromFile(jcd3d_lpd3dd, L"wall.bmp", &lpTextureWall)))
	{
		MessageBox(NULL, L"Load wall texture failed", L"Error", MB_OK);
		return FALSE;
	}
	if(FAILED(D3DXCreateTextureFromFile(jcd3d_lpd3dd, L"light.jpg", &lpTextureLight)))
	{
		MessageBox(NULL, L"Load light texture failed", L"Error", MB_OK);
		return FALSE;
	}

	jcd3d_setViewTransform(jcd3d_lpd3dd, 0.0f, 0.0f, -4.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	jcd3d_setProjectionPerspectiveTransform(jcd3d_lpd3dd, WINDOW_WIDTH, WINDOW_HEIGHT);
	jcd3d_initRenderState(jcd3d_lpd3dd, D3DCULL_CCW, FALSE, TRUE, D3DSHADE_GOURAUD, D3DFILL_SOLID, FALSE);

	return TRUE;
}

VOID jcd3d::jcd3d_display(DWORD timeDelta)
{
	jcd3d_lpd3dd->SetTexture(0, lpTextureWall);
	jcd3d_lpd3dd->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	jcd3d_lpd3dd->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	jcd3d_lpd3dd->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	
	jcd3d_lpd3dd->SetTexture(1, lpTextureLight);
	jcd3d_lpd3dd->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
	jcd3d_lpd3dd->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_ADD);
	jcd3d_lpd3dd->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	jcd3d_lpd3dd->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT); 
	jcd3d_lpd3dd->SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS,    D3DTTFF_COUNT2);

	jcd3d_lpd3dd->SetStreamSource(0, lpVbCanvas, 0, sizeof(JCD3D_Vertex_xyz_diffuse_texture2));
	jcd3d_lpd3dd->SetFVF(JCD3D_Vertex_xyz_diffuse_texture2::FVF);
	jcd3d_lpd3dd->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

VOID jcd3d::jcd3d_release()
{
	jcd3d_releaseHandle(&lpVbCanvas);
	jcd3d_releaseHandle(&lpTextureLight);
	jcd3d_releaseHandle(&lpTextureWall);
}

INT WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
{
	if(!jcd3d_init(hInstance, WINDOW_WIDTH, WINDOW_HEIGHT, TRUE, D3DDEVTYPE_HAL, 2))
	{
		return 0;
	}

	jcd3d_loop();

	return 0;
}