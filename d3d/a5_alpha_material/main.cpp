#include "jcd3d.h"

const INT WIN_WIDTH = 800;
const INT WIN_HEIGHT = 600;

IDirect3DDevice9* lpd3dd;

ID3DXMesh* lpteapot;
D3DMATERIAL9 teapotMaterial;
D3DMATERIAL9 backgroundMaterial;

IDirect3DVertexBuffer9* lpbackgroundvb;
IDirect3DTexture9* lpbackgroundtex;

using namespace jcd3d;

struct Vertex
{
	FLOAT x, y, z;
	FLOAT nx, ny, nz;
	FLOAT u, v;
	
	Vertex(FLOAT px, FLOAT py, FLOAT pz, FLOAT pnx, FLOAT pny, FLOAT pnz, FLOAT pu, FLOAT pv)
	{
		x = px; y = py; z = pz;
		nx = pnx; ny = pny; nz = pnz;
		u = pu; v = pv;
	}

	static const DWORD FVF;
};
const DWORD Vertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

HRESULT CALLBACK jcd3d::jcd3d_wndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(msg)
	{
	case WM_DESTROY:
		{
			DestroyWindow(hwnd);
			break;
		}
	case WM_KEYDOWN:
		{
			if(wparam == VK_ESCAPE)
			{
				PostQuitMessage(0);
				break;
			}
		}
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

BOOL jcd3d::jcd3d_setup()
{
	teapotMaterial = JCD3D_RED_MATERIAL;
	teapotMaterial.Diffuse.a = 0.5f;

	backgroundMaterial = JCD3D_WHITE_MATERIAL;

	D3DXCreateTeapot(lpd3dd, &lpteapot, NULL);

	lpd3dd->CreateVertexBuffer(6 * sizeof(Vertex), D3DUSAGE_WRITEONLY, Vertex::FVF, D3DPOOL_MANAGED, &lpbackgroundvb, NULL);

	Vertex* v;
	lpbackgroundvb->Lock(0, 0, (VOID**)&v, 0);
	v[0] = Vertex(-10.0f, -10.0f, 5.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[1] = Vertex(-10.0f,  10.0f, 5.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[2] = Vertex( 10.0f,  10.0f, 5.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	v[3] = Vertex(-10.0f, -10.0f, 5.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[4] = Vertex( 10.0f,  10.0f, 5.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[5] = Vertex( 10.0f, -10.0f, 5.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);
	lpbackgroundvb->Unlock();

	D3DLIGHT9 dir;
	::ZeroMemory(&dir, sizeof(dir));
	dir.Type      = D3DLIGHT_DIRECTIONAL;
	dir.Diffuse   = JCD3D_WHITE;
	dir.Specular  = JCD3D_WHITE * 0.2f;
	dir.Ambient   = JCD3D_WHITE * 0.6f;
	dir.Direction = D3DXVECTOR3(0.707f, 0.0f, 0.707f);

	lpd3dd->SetLight(0, &dir);
	lpd3dd->LightEnable(0, true);

	lpd3dd->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	lpd3dd->SetRenderState(D3DRS_SPECULARENABLE, true);

	D3DXCreateTextureFromFile(
		lpd3dd, L"bg.jpg", &lpbackgroundtex
	);

	lpd3dd->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	lpd3dd->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	lpd3dd->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	lpd3dd->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	lpd3dd->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	lpd3dd->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	lpd3dd->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	D3DXVECTOR3 pos(0.0f, 0.0f, -5.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMATRIX V;
	D3DXMatrixLookAtLH(&V, &pos, &target, &up);
	lpd3dd->SetTransform(D3DTS_VIEW, &V);

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj, D3DX_PI * 0.5f, (FLOAT)WIN_WIDTH / (FLOAT)WIN_HEIGHT, 1.0f, 1000.0f
	);
	lpd3dd->SetTransform(D3DTS_PROJECTION, &proj);

	return TRUE;
}

VOID jcd3d::jcd3d_display(DWORD timeDelta)
{
	if(lpd3dd != NULL)
	{
		if(GetAsyncKeyState('A') & 0x8000f )
		{
			teapotMaterial.Diffuse.a += 0.01f;
		}
		if(GetAsyncKeyState('S') & 0x8000f )
		{
			teapotMaterial.Diffuse.a -= 0.01f;
		}

		if(teapotMaterial.Diffuse.a > 1.0f)
		{
			teapotMaterial.Diffuse.a = 1.0f;
		}
		if(teapotMaterial.Diffuse.a < 0.0f)
		{
			teapotMaterial.Diffuse.a = 0.0f;
		}

		lpd3dd->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
		lpd3dd->BeginScene();

		D3DXMATRIX W;
		D3DXMatrixIdentity(&W);
		lpd3dd->SetTransform(D3DTS_WORLD, &W);
		lpd3dd->SetFVF(Vertex::FVF);
		lpd3dd->SetStreamSource(0, lpbackgroundvb, 0, sizeof(Vertex));
		lpd3dd->SetMaterial(&backgroundMaterial);
		lpd3dd->SetTexture(0, lpbackgroundtex);
		lpd3dd->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

		lpd3dd->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		D3DXMatrixScaling(&W, 1.5f, 1.5f, 1.5f);
		lpd3dd->SetTransform(D3DTS_WORLD, &W);
		lpd3dd->SetMaterial(&teapotMaterial);
		lpd3dd->SetTexture(0, 0);
		lpteapot->DrawSubset(0);  
		lpd3dd->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

		lpd3dd->EndScene();
		lpd3dd->Present(NULL, NULL, NULL, NULL);
	}
}

VOID jcd3d::jcd3d_release()
{
	jcd3d_releaseHandle(&lpteapot);
	jcd3d_releaseHandle(&lpbackgroundvb);
	jcd3d_releaseHandle(&lpd3dd);
}

INT WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
{
	if(!jcd3d_init(hInstance, WIN_WIDTH, WIN_HEIGHT, true, D3DDEVTYPE_HAL, &lpd3dd))
	{
		return 0;
	}

	jcd3d_loop(jcd3d_display);
	jcd3d_release();

	return 0;
}