#include "jcd3d.h"

LPDIRECT3DDEVICE9 lpd3dd = NULL;
const INT WND_WIDTH = 800;
const INT WND_HEIGHT = 600;

LPDIRECT3DVERTEXBUFFER9 lpd3dvb = NULL;
LPDIRECT3DINDEXBUFFER9 lpd3dib = NULL;

struct JCVertex
{
	FLOAT x, y, z;
	D3DCOLOR color;

	JCVertex(FLOAT px, FLOAT py, FLOAT pz, D3DCOLOR pcolor)
	{
		x = px;
		y = py;
		z = pz;
		color = pcolor;
	}

	static const DWORD fvf;
};
const DWORD JCVertex::fvf = D3DFVF_XYZ | D3DFVF_DIFFUSE;

LRESULT CALLBACK jcd3d::jcd3d_wndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(msg)
	{
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}
	case WM_KEYDOWN:
		{
			if(wparam == VK_ESCAPE)
			{
				DestroyWindow(hwnd);
				break;
			}
		}
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

BOOL jcd3d::jcd3d_setup()
{
	if(FAILED(lpd3dd->CreateVertexBuffer(8 * sizeof(JCVertex), D3DUSAGE_WRITEONLY, JCVertex::fvf, D3DPOOL_MANAGED, &lpd3dvb, NULL)))
	{
		return FALSE;
	}

	if(FAILED(lpd3dd->CreateIndexBuffer(36 * sizeof(DWORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &lpd3dib, NULL)))
	{
		return FALSE;
	}

	// vertex buffer
	JCVertex* pvertex;
	if(FAILED(lpd3dvb->Lock(0, 0, (VOID**)&pvertex, 0)))
	{
		return FALSE;
	}

	pvertex[0] = JCVertex(-1.0f, -1.0f, -1.0f, 0xFF0000);
	pvertex[1] = JCVertex(-1.0f,  1.0f, -1.0f, 0x00FF00);
	pvertex[2] = JCVertex( 1.0f,  1.0f, -1.0f, 0x0000FF);
	pvertex[3] = JCVertex( 1.0f, -1.0f, -1.0f, 0xFFFF00);
	pvertex[4] = JCVertex(-1.0f, -1.0f,  1.0f, 0x00FFFF);
	pvertex[5] = JCVertex(-1.0f,  1.0f,  1.0f, 0xFF00FF);
	pvertex[6] = JCVertex( 1.0f,  1.0f,  1.0f, 0xFFFFFF);
	pvertex[7] = JCVertex( 1.0f, -1.0f,  1.0f, 0x000000);

	if(FAILED(lpd3dvb->Unlock()))
	{
		return FALSE;
	}

	// index buffer
	WORD* pindex;
	if(FAILED(lpd3dib->Lock(0, 0, (VOID**)&pindex, 0)))
	{
		return FALSE;
	}

	pindex[0]  = 0; pindex[1]  = 1; pindex[2]  = 2;
	pindex[3]  = 0; pindex[4]  = 2; pindex[5]  = 3;

	pindex[6]  = 4; pindex[7]  = 6; pindex[8]  = 5;
	pindex[9]  = 4; pindex[10] = 7; pindex[11] = 6;

	pindex[12] = 4; pindex[13] = 5; pindex[14] = 1;
	pindex[15] = 4; pindex[16] = 1; pindex[17] = 0;

	pindex[18] = 3; pindex[19] = 2; pindex[20] = 6;
	pindex[21] = 3; pindex[22] = 6; pindex[23] = 7;

	pindex[24] = 1; pindex[25] = 5; pindex[26] = 6;
	pindex[27] = 1; pindex[28] = 6; pindex[29] = 2;

	pindex[30] = 4; pindex[31] = 0; pindex[32] = 3;
	pindex[33] = 4; pindex[34] = 3; pindex[35] = 7;

	if(FAILED(lpd3dib->Unlock()))
	{
		return FALSE;
	}

	// view matrix
	D3DXVECTOR3 position(0.0f, 0.0f, -5.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMATRIX v;
	D3DXMatrixLookAtLH(&v, &position, &target, &up);

	if(FAILED(lpd3dd->SetTransform(D3DTS_VIEW, &v)))
	{
		return FALSE;
	}

	// projection matrix
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI * .5, (FLOAT)WND_WIDTH / (FLOAT)WND_HEIGHT, 1.0f, 1000.0f);
	if(FAILED(lpd3dd->SetTransform(D3DTS_PROJECTION, &proj)))
	{
		return FALSE;
	}

	// render state
	if(FAILED(lpd3dd->SetRenderState(D3DRS_LIGHTING, FALSE)))
	{
		return FALSE;
	}
	if(FAILED(lpd3dd->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID)))
	{
		return FALSE;
	}

	return TRUE;
}

VOID jcd3d::jcd3d_display(DWORD timeDelta)
{
	if(lpd3dd)
	{
		static FLOAT y = 0.0f;
		D3DXMATRIX ry;
		D3DXMATRIX rx;
		D3DXMatrixRotationX(&rx, -.4f);
		D3DXMatrixRotationY(&ry, y);
		y += .01f;
		if(y >= D3DX_PI * 2)
		{
			y = 0;
		}

		D3DXMATRIX wm = rx * ry;
		if(FAILED(lpd3dd->SetTransform(D3DTS_WORLD, &wm)))
		{
			return;
		}

		lpd3dd->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
		lpd3dd->BeginScene();

		lpd3dd->SetStreamSource(0, lpd3dvb, 0, sizeof(JCVertex));
		lpd3dd->SetIndices(lpd3dib);
		lpd3dd->SetFVF(JCVertex::fvf);

		lpd3dd->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);

		lpd3dd->EndScene();
		lpd3dd->Present(NULL, NULL, NULL, NULL);
	}
}

VOID jcd3d::jcd3d_release()
{
	jcd3d::jcd3d_releaseHandle(&lpd3dvb);
	jcd3d::jcd3d_releaseHandle(&lpd3dib);
	jcd3d::jcd3d_releaseHandle(&lpd3dd);
}

INT WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
{
	if(!jcd3d::jcd3d_init(hInstance, WND_WIDTH, WND_HEIGHT, true, D3DDEVTYPE_HAL, &lpd3dd))
	{
		return 0;
	}

	jcd3d::jcd3d_loop(jcd3d::jcd3d_display);

	jcd3d::jcd3d_release();

	return 0;
}