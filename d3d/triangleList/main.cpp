#include <ctime>
#include "jcd3d.h"

using namespace jcd3d;

CONST INT WIN_WIDTH = 800;
CONST INT WIN_HEIGHT = 600;
CONST INT CORNOR_AMOUNT = 50;
CONST INT LAYER_AMOUNT = 40;
CONST FLOAT LAYER_HEIGHT = 0.1f;
CONST FLOAT RADIUS_MAX = 2.5f;

LPDIRECT3DDEVICE9 lpd3dd = NULL;
LPDIRECT3DVERTEXBUFFER9 lpvb = NULL;
LPDIRECT3DINDEXBUFFER9 lpib = NULL;

BOOL jcd3d::jcd3d_setup()
{
	srand((UINT)time(NULL));

	lpd3dd->CreateVertexBuffer(
		CORNOR_AMOUNT * (LAYER_AMOUNT + 1) * sizeof(JCD3D_Vertex_xyz_diffuse), 
		D3DUSAGE_WRITEONLY, JCD3D_Vertex_xyz_diffuse::FVF, D3DPOOL_MANAGED, &lpvb, NULL
	);
	// i5----i6----i7----i8
	//  |   /|    / |   /|
	//  |  / |   /  |  / |
	//  | /  |  /   | /  |
	//  |/   | /    |/   |
	// i1----i2----i3----i4
	JCD3D_Vertex_xyz_diffuse* vb;
	FLOAT degreeInc = D3DX_PI * 2 / CORNOR_AMOUNT;
	FLOAT radiusDegree = 0;
	FLOAT radiusDegreeInc = D3DX_PI * 2 / LAYER_AMOUNT;
	lpvb->Lock(0, 0, (VOID**)&vb, 0);
	for(INT layer = 0; layer < LAYER_AMOUNT + 1; layer++)
	{
		for(INT corner = 0; corner < CORNOR_AMOUNT; corner++)
		{
			vb[layer * CORNOR_AMOUNT + corner] = JCD3D_Vertex_xyz_diffuse(
				cosf(corner * degreeInc) * RADIUS_MAX * cosf(radiusDegree), 
				layer * LAYER_HEIGHT, 
				sinf(corner * degreeInc) * RADIUS_MAX * cosf(radiusDegree), 
				rand() * 200 % 0xFFFFFF
			);
		}
		radiusDegree += radiusDegreeInc;
	}
	lpvb->Unlock();

	lpd3dd->CreateIndexBuffer(
		6 * CORNOR_AMOUNT * LAYER_AMOUNT * sizeof(WORD), 
		D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &lpib, NULL
	);
	WORD* ib;
	INT ibIndex = 0;
	lpib->Lock(0, 0, (VOID**)&ib, 0);
	for (INT layer = 0; layer < LAYER_AMOUNT; layer++)
	{
		for(INT corner = 0; corner < CORNOR_AMOUNT; corner++)
		{
			// i1----i2
			//  |   /|
			//  |  / |
			//  | /  |
			//  |/   |
			// i3----i4
			WORD index1 = layer * CORNOR_AMOUNT + corner;
			WORD index2 = corner == CORNOR_AMOUNT - 1 ? layer * CORNOR_AMOUNT + 0 : layer * CORNOR_AMOUNT + corner + 1;
			WORD index3 = (layer + 1) * CORNOR_AMOUNT + corner;
			WORD index4 = corner == CORNOR_AMOUNT - 1 ? (layer + 1) * CORNOR_AMOUNT + 0 : (layer + 1) * CORNOR_AMOUNT + corner + 1;
			ib[ibIndex++] = index1;
			ib[ibIndex++] = index2;
			ib[ibIndex++] = index3;
			ib[ibIndex++] = index2;
			ib[ibIndex++] = index4;
			ib[ibIndex++] = index3;
		}
	}
	lpib->Unlock();

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI * 0.5f, (FLOAT)WIN_WIDTH / (FLOAT)WIN_HEIGHT, 1.0f, 1000.0f);
	lpd3dd->SetTransform(D3DTS_PROJECTION, &proj);

	if(!jcd3d_initRenderState(lpd3dd, D3DCULL_NONE, FALSE, TRUE, D3DSHADE_GOURAUD, D3DFILL_WIREFRAME, FALSE))
	{
		return FALSE;
	}

	return TRUE;
}

VOID jcd3d::jcd3d_display(DWORD timeDelta)
{
	if(lpd3dd != NULL)
	{
		static FLOAT eyeRadius = 6.0f;
		static FLOAT eyeDegree = 0.0f;
		static FLOAT eyeHeight = 0.0f;
		if(jcd3d_keyDown('A'))
		{
			eyeDegree += 0.05f;
		}
		if(jcd3d_keyDown('D'))
		{
			eyeDegree -= 0.05f;
		}
		if(jcd3d_keyDown('W'))
		{
			eyeHeight += 0.05f;
		}
		if(jcd3d_keyDown('S'))
		{
			eyeHeight -= 0.05f;
		}
		D3DXMATRIX view;
		D3DXVECTOR3 eye(cosf(eyeDegree) * eyeRadius, eyeHeight, sinf(eyeDegree) * eyeRadius);
		D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
		D3DXMatrixLookAtLH(&view, &eye, &target, &up);
		lpd3dd->SetTransform(D3DTS_VIEW, &view);

		lpd3dd->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
		lpd3dd->BeginScene();

		lpd3dd->SetStreamSource(0, lpvb, 0, sizeof(JCD3D_Vertex_xyz_diffuse));
		lpd3dd->SetFVF(JCD3D_Vertex_xyz_diffuse::FVF);
		lpd3dd->SetIndices(lpib);
		lpd3dd->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, CORNOR_AMOUNT * (LAYER_AMOUNT + 1), 0, CORNOR_AMOUNT * 2 * LAYER_AMOUNT);

		lpd3dd->EndScene();
		lpd3dd->Present(NULL, NULL, NULL, NULL);
	}
}

VOID jcd3d::jcd3d_release()
{
	jcd3d_releaseHandle(&lpvb);
	jcd3d_releaseHandle(&lpib);
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