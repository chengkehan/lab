#include "jcd3d.h"

using namespace jcd3d;

const INT WIN_WIDTH = 800;
const INT WIN_HEIGHT = 600;

LPDIRECT3DDEVICE9 lpd3dd = NULL;
LPDIRECT3DVERTEXBUFFER9 lpd3dvb = NULL;
D3DMATERIAL9* lpmtrl = NULL;

BOOL jcd3d::jcd3d_setup()
{
	lpd3dd->CreateVertexBuffer(4 * sizeof(JCD3D_Vertex_xyz_normal), D3DUSAGE_WRITEONLY, JCD3D_Vertex_xyz_normal::FVF, D3DPOOL_MANAGED, &lpd3dvb, NULL);
	JCD3D_Vertex_xyz_normal* vb;
	lpd3dvb->Lock(0, 4 * sizeof(JCD3D_Vertex_xyz_normal), (VOID**)&vb, 0);
	vb[0] = JCD3D_Vertex_xyz_normal(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f);
	vb[1] = JCD3D_Vertex_xyz_normal(-1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f);
	vb[2] = JCD3D_Vertex_xyz_normal(1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f);
	vb[3] = JCD3D_Vertex_xyz_normal(1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f);
	lpd3dvb->Unlock();

	D3DXMATRIX view;
	D3DXVECTOR3 eye(0.0f, 0.0f, -3.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&view, &eye, &target, &up);
	lpd3dd->SetTransform(D3DTS_VIEW, &view);

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI * 0.5f, (FLOAT)WIN_WIDTH / (FLOAT)WIN_HEIGHT, 1.0f, 1000.0f);
	lpd3dd->SetTransform(D3DTS_PROJECTION, &proj);
	
	lpmtrl = const_cast<D3DMATERIAL9*>(&JCD3D_WHITE_MATERIAL);

	jcd3d_initRenderState(lpd3dd, D3DCULL_NONE, TRUE, TRUE, D3DSHADE_GOURAUD, D3DFILL_SOLID, FALSE);

	lpd3dd->SetRenderState(D3DRS_AMBIENT, 0x0099FF);

	return TRUE;
}

VOID jcd3d::jcd3d_display(DWORD timeDelta)
{
	if(lpd3dd != NULL)
	{
		lpd3dd->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
		lpd3dd->BeginScene();

		lpd3dd->SetStreamSource(0, lpd3dvb, 0, sizeof(JCD3D_Vertex_xyz_normal));
		lpd3dd->SetFVF(JCD3D_Vertex_xyz_normal::FVF);
		lpd3dd->SetMaterial(lpmtrl);
		lpd3dd->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		lpd3dd->EndScene();
		lpd3dd->Present(NULL, NULL, NULL, NULL);
	}
}

VOID jcd3d::jcd3d_release()
{
	lpmtrl = NULL;
	jcd3d_releaseHandle(&lpd3dvb);
	jcd3d_releaseHandle(&lpd3dd);
}

INT WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
{
	if(!jcd3d_init(hInstance, WIN_WIDTH, WIN_HEIGHT, TRUE, D3DDEVTYPE_HAL, &lpd3dd))
	{
		return 0;
	}

	jcd3d_loop(jcd3d_display);
	jcd3d_release();

	return 0;
}