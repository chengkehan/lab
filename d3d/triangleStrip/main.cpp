#include <ctime>
#include "jcd3d.h"

using namespace jcd3d;

const INT WIN_WIDTH = 800;
const INT WIN_HEIGHT = 600;

const INT CORNER_AMOUNT = 80;
const INT LAYER_AMOUNT = 6;
const FLOAT LAYER_HEIGHT = 0.5f;
const FLOAT RADIUS = 3.0f;

IDirect3DDevice9* lpd3dd = NULL;
IDirect3DVertexBuffer9* lpvb = NULL;

struct Vertex
{
	FLOAT x, y, z;
	D3DCOLOR color;
	Vertex(FLOAT px, FLOAT py, FLOAT pz, D3DCOLOR pcolor)
	{
		x = px; y = py; z = pz;
		color = pcolor;
	}
	static const DWORD FVF;
};
const DWORD Vertex::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;

BOOL jcd3d::jcd3d_setup()
{
	srand((UINT)time(NULL));

	lpd3dd->CreateVertexBuffer((CORNER_AMOUNT + 2) * LAYER_AMOUNT * sizeof(Vertex) * 2, D3DUSAGE_WRITEONLY, Vertex::FVF, D3DPOOL_MANAGED, &lpvb, NULL);
	Vertex* vb = NULL;
	lpvb->Lock(0, 0, (VOID**)&vb, 0);
	
	FLOAT layerHeight = 0;
	FLOAT degree = 0;
	FLOAT degreeInc = D3DX_PI * 2.0f / (FLOAT)CORNER_AMOUNT;
	INT index = 0;
	for (INT layer = 0; layer < LAYER_AMOUNT; layer++)
	{
		degree = 0;
		for (INT corner = 0; corner < CORNER_AMOUNT + 1; corner++)
		{
			FLOAT p1x = cosf(degree) * RADIUS;
			FLOAT p1z = sinf(degree) * RADIUS;
			FLOAT p1y = layerHeight;

			FLOAT p2x = p1x;
			FLOAT p2z = p1z;
			FLOAT p2y = layerHeight + LAYER_HEIGHT;

			vb[index++] = Vertex(p1x, p1y, p1z, rand() % 0xFFFFFF);
			vb[index++] = Vertex(p2x, p2y, p2z, rand() % 0xFFFFFF);

			degree += degreeInc;
		}
		layerHeight += LAYER_HEIGHT;
	}

	lpvb->Unlock();

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI * 0.5f, (FLOAT)WIN_WIDTH / (FLOAT)WIN_HEIGHT, 1.0f, 1000.0f);
	lpd3dd->SetTransform(D3DTS_PROJECTION, &proj);

	lpd3dd->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	lpd3dd->SetRenderState(D3DRS_LIGHTING, FALSE);
	lpd3dd->SetRenderState(D3DRS_ZENABLE, TRUE);
	lpd3dd->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
	lpd3dd->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	lpd3dd->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	lpd3dd->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	return TRUE;
}

VOID jcd3d::jcd3d_display(DWORD timeDelta)
{
	if(lpd3dd != NULL)
	{
		static FLOAT eyeY = 0.0f;
		static FLOAT viewAngle = 0.0f;
		static FLOAT viewRadius = 6.0f;

		if(jcd3d_keyDown('W'))
		{
			eyeY += 0.1f;
		}
		if(jcd3d_keyDown('S'))
		{
			eyeY -= 0.1f;
		}
		if(!jcd3d_keyDown('W') && !jcd3d_keyDown('S'))
		{
			eyeY *= 0.9f;
		}

		if(jcd3d_keyDown('A'))
		{
			viewAngle += 0.02f;
		}
		if(jcd3d_keyDown('D'))
		{
			viewAngle -= 0.02f;
		}
		if(!jcd3d_keyDown('A') && !jcd3d_keyDown('D'))
		{
			viewAngle *= 0.9f;
		}

		D3DXMATRIX view;
		D3DXVECTOR3 eye(cosf(viewAngle) * viewRadius, eyeY, sinf(viewAngle) * viewRadius);
		D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
		D3DXMatrixLookAtLH(&view, &eye, &target, &up);
		lpd3dd->SetTransform(D3DTS_VIEW, &view);

		lpd3dd->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
		lpd3dd->BeginScene();

		lpd3dd->SetStreamSource(0, lpvb, 0, sizeof(Vertex));
		lpd3dd->SetFVF(Vertex::FVF);
		for (INT layer = 0; layer < LAYER_AMOUNT; layer++)
		{
			lpd3dd->DrawPrimitive(D3DPT_TRIANGLESTRIP, (CORNER_AMOUNT * 2 + 2) * layer, CORNER_AMOUNT * 2);
		}

		lpd3dd->EndScene();
		lpd3dd->Present(NULL, NULL, NULL, NULL);
	}
}

VOID jcd3d::jcd3d_release()
{
	jcd3d_releaseHandle(&lpvb);
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