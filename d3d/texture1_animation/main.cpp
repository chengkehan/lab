#include "jcd3d.h"

using namespace jcd3d;

const INT WINDOW_WIDTH = 800;
const INT WINDOW_HEIGHT = 600;

IDirect3DVertexBuffer9* lpVbWall = NULL;
IDirect3DTexture9* lpTextureWall = NULL;
IDirect3DTexture9* lpTextureLight = NULL;

BOOL jcd3d::jcd3d_setup()
{
	jcd3d_lpd3dd->CreateVertexBuffer(
		4 * sizeof(JCD3D_Vertex_xyz_diffuse_texture2), D3DUSAGE_WRITEONLY, JCD3D_Vertex_xyz_diffuse_texture2::FVF, D3DPOOL_MANAGED, &lpVbWall, NULL
	);
	JCD3D_Vertex_xyz_diffuse_texture2 wallVbList[] = {
		JCD3D_Vertex_xyz_diffuse_texture2(-2.0f, -2.0f, 0.0f, 0xFF0099FF, 0.0f, 1.0f, 0.0f, 1.0f), 
		JCD3D_Vertex_xyz_diffuse_texture2(-2.0f, 2.0f, 0.0f, 0xFF0099FF, 0.0f, 0.0f, 0.0f, 0.0f), 
		JCD3D_Vertex_xyz_diffuse_texture2(2.0f, -2.0f, 0.0f, 0xFF0099FF, 1.0f, 1.0f, 1.0f, 1.0f), 
		JCD3D_Vertex_xyz_diffuse_texture2(2.0f, 2.0f, 0.0f, 0xFF0099FF, 1.0f, 0.0f, 1.0f, 0.0f)
	};
	VOID* wallVb;
	lpVbWall->Lock(0, 4 * sizeof(JCD3D_Vertex_xyz_diffuse_texture2), &wallVb, 0);
	memcpy(wallVb, wallVbList, 4 * sizeof(JCD3D_Vertex_xyz_diffuse_texture2));
	lpVbWall->Unlock();

	if(FAILED(D3DXCreateTextureFromFile(jcd3d_lpd3dd, L"wall.bmp", &lpTextureWall)))
	{
		MessageBox(NULL, L"Load texture wall failed", L"Error", MB_OK);
		return FALSE;
	}
	if(FAILED(D3DXCreateTextureFromFile(jcd3d_lpd3dd, L"light.jpg", &lpTextureLight)))
	{
		MessageBox(NULL, L"Load texture light failed", L"Error", MB_OK);
		return FALSE;
	}

	jcd3d_setViewTransform(jcd3d_lpd3dd, 0.0f, 0.0f, -4.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	jcd3d_setProjectionPerspectiveTransform(jcd3d_lpd3dd, WINDOW_WIDTH, WINDOW_HEIGHT);
	jcd3d_initRenderState(jcd3d_lpd3dd, D3DCULL_CCW, FALSE, TRUE, D3DSHADE_GOURAUD, D3DFILL_SOLID, FALSE);

	return TRUE;
}

VOID jcd3d::jcd3d_display(DWORD timeDelta)
{
	static INT count = 1;
	static FLOAT speed = 0.005f;

	JCD3D_Vertex_xyz_diffuse_texture2* wallVb;
	lpVbWall->Lock(0, 4 * sizeof(JCD3D_Vertex_xyz_diffuse_texture2), (VOID**)&wallVb, 0);
	wallVb[0].u2 += speed; wallVb[0].v2 -= speed;
	wallVb[1].u2 += speed; wallVb[1].v2 += speed;
	wallVb[2].u2 -= speed; wallVb[2].v2 -= speed;
	wallVb[3].u2 -= speed; wallVb[3].v2 += speed;
	lpVbWall->Unlock();
	if(++count >= 50)
	{
		count = 0;
		speed *= -1.0f;
	}

	jcd3d_lpd3dd->SetTexture(0, lpTextureWall);
	jcd3d_lpd3dd->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	jcd3d_lpd3dd->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	jcd3d_lpd3dd->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);

	jcd3d_lpd3dd->SetTexture(1, lpTextureLight);
	jcd3d_lpd3dd->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
	jcd3d_lpd3dd->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	jcd3d_lpd3dd->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
	jcd3d_lpd3dd->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);

	jcd3d_lpd3dd->SetStreamSource(0, lpVbWall, 0, sizeof(JCD3D_Vertex_xyz_diffuse_texture2));
	jcd3d_lpd3dd->SetFVF(JCD3D_Vertex_xyz_diffuse_texture2::FVF);
	jcd3d_lpd3dd->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

VOID jcd3d::jcd3d_release()
{
	jcd3d_releaseHandle(&lpTextureWall);
	jcd3d_releaseHandle(&lpTextureLight);
	jcd3d_releaseHandle(&lpVbWall);
}

INT WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
{
	if(!jcd3d_init(hInstance, WINDOW_WIDTH, WINDOW_HEIGHT, TRUE, D3DDEVTYPE_HAL, 2))
	{
		return FALSE;
	}

	jcd3d_loop();

	return 0;
}