#include "jcd3d.h"

using namespace jcd3d;

const INT WINDOW_WIDTH = 800;
const INT WINDOW_HEIGHT = 600;

LPDIRECT3DDEVICE9 lpd3dd = NULL;

LPDIRECT3DVERTEXBUFFER9 lpbgvb = NULL;
LPDIRECT3DVERTEXBUFFER9 lpboxvb = NULL;

LPDIRECT3DTEXTURE9 lptreetexture = NULL;

BOOL jcd3d::jcd3d_setup()
{
	if(FAILED(D3DXCreateTextureFromFile(lpd3dd, L"tree.tga", &lptreetexture)))
	{
		MessageBox(NULL, L"Cannot load texture file", L"Error", MB_OK);
		return FALSE;
	}

	lpd3dd->CreateVertexBuffer(4 * sizeof(JCD3D_Vertex_xyz_diffuse), D3DUSAGE_WRITEONLY, JCD3D_Vertex_xyz_diffuse::FVF, D3DPOOL_MANAGED, &lpbgvb, NULL);
	JCD3D_Vertex_xyz_diffuse bgvbList[] = {
		JCD3D_Vertex_xyz_diffuse(-3.0f, -3.0f, 0.0f, 0xFF222222), 
		JCD3D_Vertex_xyz_diffuse(-3.0f, 3.0f, 0.0f, 0xFF222222), 
		JCD3D_Vertex_xyz_diffuse(3.0f, -3.0f, 0.0f, 0xFF222222), 
		JCD3D_Vertex_xyz_diffuse(3.0f, 3.0f, 0.0f, 0xFF222222)
	};
	VOID* bgvb;
	lpbgvb->Lock(0, 4 * sizeof(JCD3D_Vertex_xyz_diffuse), (VOID**)&bgvb, 0);
	memcpy(bgvb, bgvbList, 4 * sizeof(JCD3D_Vertex_xyz_diffuse));
	lpbgvb->Unlock();

	lpd3dd->CreateVertexBuffer(4 * sizeof(JCD3D_Vertex_xyz_diffuse_texture), D3DUSAGE_WRITEONLY, JCD3D_Vertex_xyz_diffuse_texture::FVF, D3DPOOL_MANAGED, &lpboxvb, NULL);
	JCD3D_Vertex_xyz_diffuse_texture boxvbList[] = {
		JCD3D_Vertex_xyz_diffuse_texture(-2.0f, -2.0f, -1.0f, 0xFF00FF00, 0.0f, 1.0f), 
		JCD3D_Vertex_xyz_diffuse_texture(-2.0f, 2.0f, -1.0f, 0xFF0000FF, 0.0f, 0.0f), 
		JCD3D_Vertex_xyz_diffuse_texture(2.0f, -2.0f, -1.0f, 0xFFFF0000, 1.0f, 1.0f), 
		JCD3D_Vertex_xyz_diffuse_texture(2.0f, 2.0f, -1.0f, 0xFF000000, 1.0f, 0.0f)
	};
	VOID* boxvb;
	lpboxvb->Lock(0, 4 * sizeof(JCD3D_Vertex_xyz_diffuse_texture), (VOID**)&boxvb, 0);
	memcpy(boxvb, boxvbList, 4 * sizeof(JCD3D_Vertex_xyz_diffuse_texture));
	lpboxvb->Unlock();

	D3DXVECTOR3 eye(0.0f, 0.0f, -4.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	jcd3d_setViewTransform(lpd3dd, &eye, &target, &up);

	jcd3d_setProjectionPerspectiveTransform(lpd3dd, WINDOW_WIDTH, WINDOW_HEIGHT);

	jcd3d_initRenderState(lpd3dd, D3DCULL_CCW, FALSE, TRUE, D3DSHADE_GOURAUD, D3DFILL_SOLID, TRUE);

	return TRUE;
}

VOID jcd3d::jcd3d_display(DWORD timeDelta)
{
	if(lpd3dd != NULL)
	{
		lpd3dd->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
		lpd3dd->BeginScene();

		lpd3dd->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		lpd3dd->SetTexture(0, NULL);
		lpd3dd->SetStreamSource(0, lpbgvb, 0, sizeof(JCD3D_Vertex_xyz_diffuse));
		lpd3dd->SetFVF(JCD3D_Vertex_xyz_diffuse::FVF);
		lpd3dd->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		lpd3dd->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		lpd3dd->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		lpd3dd->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		lpd3dd->SetTexture(0, lptreetexture);
		lpd3dd->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_ADD);
		lpd3dd->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		lpd3dd->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

		lpd3dd->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		lpd3dd->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		lpd3dd->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

		lpd3dd->SetStreamSource(0, lpboxvb, 0, sizeof(JCD3D_Vertex_xyz_diffuse_texture));
		lpd3dd->SetFVF(JCD3D_Vertex_xyz_diffuse_texture::FVF);
		lpd3dd->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		lpd3dd->EndScene();
		lpd3dd->Present(NULL, NULL, NULL, NULL);
	}
}

VOID jcd3d::jcd3d_release()
{
	jcd3d_releaseHandle(&lpbgvb);
	jcd3d_releaseHandle(&lpboxvb);
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