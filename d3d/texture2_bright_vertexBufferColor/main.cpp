#include "jcd3d.h"

using namespace jcd3d;

const INT WINDOW_WIDTH = 800;
const INT WINDOW_HEIGHT = 600;

IDirect3DVertexBuffer9* lpVbWall = NULL;
IDirect3DTexture9* lpTextureWall = NULL;

BOOL jcd3d::jcd3d_setup()
{
	if(FAILED(D3DXCreateTextureFromFile(jcd3d_lpd3dd, L"wall.bmp", & lpTextureWall)))
	{
		MessageBox(NULL, L"Load wall texture failed", L"Error", MB_OK);
		return FALSE;
	}

	JCD3D_Vertex_xyz_diffuse_texture vbList[] = {
		JCD3D_Vertex_xyz_diffuse_texture(-2.0f, -2.0f, 0.0f, 0xFF888888, 0.0f, 1.0f), 
		JCD3D_Vertex_xyz_diffuse_texture(-2.0f, 2.0f, 0.0f, 0xFF888888, 0.0f, 0.0f), 
		JCD3D_Vertex_xyz_diffuse_texture(2.0f, -2.0f, 0.0f, 0xFF888888, 1.0f, 1.0f), 
		JCD3D_Vertex_xyz_diffuse_texture(2.0f, 2.0f, 0.0f, 0xFF888888, 1.0f, 0.0f)
	};

	jcd3d_lpd3dd->CreateVertexBuffer(
		4 * sizeof(JCD3D_Vertex_xyz_diffuse_texture), D3DUSAGE_WRITEONLY, JCD3D_Vertex_xyz_diffuse_texture::FVF, D3DPOOL_MANAGED, &lpVbWall, NULL
	);
	VOID* wallVb = NULL;
	lpVbWall->Lock(0, 4 * sizeof(JCD3D_Vertex_xyz_diffuse_texture), &wallVb, 0);
	memcpy(wallVb, vbList, 4 * sizeof(JCD3D_Vertex_xyz_diffuse_texture));
	lpVbWall->Unlock();

	jcd3d_setViewTransform(jcd3d_lpd3dd, 0.0f, 0.0f, -4.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	jcd3d_setProjectionPerspectiveTransform(jcd3d_lpd3dd, WINDOW_WIDTH, WINDOW_HEIGHT);
	jcd3d_initRenderState(jcd3d_lpd3dd, D3DCULL_CCW, FALSE, TRUE, D3DSHADE_GOURAUD, D3DFILL_SOLID, FALSE);

	return TRUE;
}

VOID jcd3d::jcd3d_display(DWORD timeDelta)
{
	jcd3d_lpd3dd->SetTexture(0, lpTextureWall);
	jcd3d_lpd3dd->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	jcd3d_lpd3dd->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	jcd3d_lpd3dd->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	jcd3d_lpd3dd->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_ADD);

	jcd3d_lpd3dd->SetStreamSource(0, lpVbWall, 0, sizeof(JCD3D_Vertex_xyz_diffuse_texture));
	jcd3d_lpd3dd->SetFVF(JCD3D_Vertex_xyz_diffuse_texture::FVF);
	jcd3d_lpd3dd->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

VOID jcd3d::jcd3d_release()
{
	jcd3d_releaseHandle(&lpTextureWall);
	jcd3d_releaseHandle(&lpVbWall);
}

INT WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
{
	if(!jcd3d_init(hInstance, WINDOW_WIDTH, WINDOW_HEIGHT, TRUE, D3DDEVTYPE_HAL, 1))
	{
		return 0;
	}

	jcd3d_loop();

	return 0;
}