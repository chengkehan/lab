#include "jcd3d.h"

using namespace jcd3d;

const INT WIN_WIDTH = 800;
const INT WIN_HEIGHT = 600;

const INT VB_AMOUNT = 4;
const INT IB_AMOUNT = 6;
const INT TRIANGLE_AMOUNT = 2;

IDirect3DDevice9* lpd3dd = NULL;

IDirect3DVertexBuffer9* lpRect1VB = NULL;
IDirect3DIndexBuffer9* lpRect1IB = NULL;

IDirect3DVertexBuffer9* lpRect2VB = NULL;
IDirect3DIndexBuffer9* lpRect2IB = NULL;

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

VOID createVBIB(IDirect3DVertexBuffer9** lplpRectVB, IDirect3DIndexBuffer9** lplpRectIB, Vertex* vertexes, WORD* indexes)
{
	lpd3dd->CreateVertexBuffer(VB_AMOUNT * sizeof(Vertex), D3DUSAGE_WRITEONLY, Vertex::FVF, D3DPOOL_MANAGED, lplpRectVB, NULL);
	Vertex* vb;
	(*lplpRectVB)->Lock(0, 0, (VOID**)&vb, 0);
	for (INT i = 0; i < VB_AMOUNT; i++)
	{
		vb[i] = vertexes[i];
	}
	(*lplpRectVB)->Unlock();

	lpd3dd->CreateIndexBuffer(IB_AMOUNT * sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, lplpRectIB, NULL);
	WORD* ib;
	(*lplpRectIB)->Lock(0, 0, (VOID**)&ib, 0);
	for (INT i = 0; i < IB_AMOUNT; i++)
	{
		ib[i] = indexes[i];
	}
	(*lplpRectIB)->Unlock();
}

VOID drawRect(IDirect3DVertexBuffer9* lpRectVB, IDirect3DIndexBuffer9* lpRectIB)
{
	lpd3dd->SetStreamSource(0, lpRectVB, 0, sizeof(Vertex));
	lpd3dd->SetIndices(lpRectIB);
	lpd3dd->SetFVF(Vertex::FVF);
	lpd3dd->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, VB_AMOUNT, 0, TRIANGLE_AMOUNT);
}

BOOL jcd3d::jcd3d_setup()
{
	Vertex vb1[] = {
		Vertex(-1.0f, 1.0f, 1.0f, 0x66000000), 
		Vertex(1.0f, 1.0f, 1.0f, 0x66000000), 
		Vertex(1.0f, -1.0f, 1.0f, 0x66000000), 
		Vertex(-1.0f, -1.0f, 1.0f, 0x66000000)
	};
	Vertex vb2[] = {
		Vertex(-3.0f, 3.0f, 3.0f, 0xFF0000FF), 
		Vertex(3.0f, 3.0f, 3.0f, 0xFFFFFFFF), 
		Vertex(3.0f, -3.0f, 3.0f, 0xFFFF0000), 
		Vertex(-3.0f, -3.0f, 3.0f, 0xFF00FF00)
	};
	WORD ib[] = {
		0, 1, 3, 
		1, 2, 3
	};
	createVBIB(&lpRect1VB, &lpRect1IB, vb1, ib);
	createVBIB(&lpRect2VB, &lpRect2IB, vb2, ib);

	D3DXMATRIX view;
	D3DXVECTOR3 eye(0.0f, 0.0f, -3.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&view, &eye, &target, &up);
	lpd3dd->SetTransform(D3DTS_VIEW, &view);

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI * 0.5f, (FLOAT)WIN_WIDTH / (FLOAT)WIN_HEIGHT, 1.0f, 1000.0f);
	lpd3dd->SetTransform(D3DTS_PROJECTION, &proj);

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
		lpd3dd->Clear(0, NULL, D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET, 0x00000000, 1.0f, 0);
		lpd3dd->BeginScene();

		lpd3dd->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		lpd3dd->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		lpd3dd->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		lpd3dd->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		drawRect(lpRect2VB, lpRect2IB);
		drawRect(lpRect1VB, lpRect1IB);
		lpd3dd->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

		lpd3dd->EndScene();
		lpd3dd->Present(NULL, NULL, NULL, NULL);
	}
}

VOID jcd3d::jcd3d_release()
{
	jcd3d_releaseHandle(&lpRect1VB);
	jcd3d_releaseHandle(&lpRect1IB);
	jcd3d_releaseHandle(&lpRect2VB);
	jcd3d_releaseHandle(&lpRect2IB);
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