#ifndef __JCD3D_H__
#define __JCD3D_H__

#include <Windows.h>
#include <MMSystem.h>
#include <d3d9.h>
#include <d3dx9.h>

namespace jcd3d
{
	// Util
	inline BOOL jcd3d_keyDown(INT vkCode)
	{
		return GetAsyncKeyState(vkCode) & 0x8000 ? TRUE : FALSE;
	}

	inline BOOL jcd3d_keyUp(INT vkCode)
	{
		return GetAsyncKeyState(vkCode) & 0x8000 ? FALSE : TRUE;
	}

	// D3D
	extern HWND jcd3d_hwnd;
	extern IDirect3DDevice9* jcd3d_lpd3dd;
	BOOL jcd3d_init(HINSTANCE hInstance, INT width, INT height, BOOL windowed, D3DDEVTYPE deviceType, DWORD maxTextureBlendStages = 1, HWND hwnd = NULL);
	BOOL jcd3d_setup();
	VOID jcd3d_release();
	VOID jcd3d_display(DWORD timeDelta);
	LRESULT CALLBACK jcd3d_wndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	VOID jcd3d_loop();

	// D3D util
	BOOL jcd3d_initRenderState(
		LPDIRECT3DDEVICE9 lpd3dd, DWORD cullMode = D3DCULL_CCW, BOOL lighting = TRUE, BOOL zEnable = TRUE, DWORD shadeMode = D3DSHADE_GOURAUD, 
		DWORD fillMode = D3DFILL_SOLID, BOOL alphaBlendEnable = FALSE
	);

	BOOL jcd3d_setProjectionPerspectiveTransform(LPDIRECT3DDEVICE9 lpd3dd, INT windowWidth, INT windowHeight);
	BOOL jcd3d_setViewTransform(LPDIRECT3DDEVICE9 lpd3dd, D3DXVECTOR3* eye, D3DXVECTOR3* target, D3DXVECTOR3* up);
	BOOL jcd3d_setViewTransform(LPDIRECT3DDEVICE9 lpd3dd, FLOAT eyeX, FLOAT eyeY, FLOAT eyeZ, FLOAT targetX, FLOAT targetY, FLOAT targetZ, FLOAT upX, FLOAT upY, FLOAT upZ);

	template<class T>
	VOID jcd3d_releaseHandle(T handle)
	{
		if((*handle) != NULL)
		{
			(*handle)->Release();
			(*handle) = NULL;
		}
	}

	// Color
	const D3DXCOLOR JCD3D_WHITE(D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));
	const D3DXCOLOR JCD3D_BLACK(D3DCOLOR_XRGB(0x00, 0x00, 0x00));
	const D3DXCOLOR JCD3D_RED(D3DCOLOR_XRGB(0xFF, 0, 0));
	const D3DXCOLOR JCD3D_GREEN(D3DCOLOR_XRGB(0, 0xFF, 0));
	const D3DXCOLOR JCD3D_BLUE(D3DCOLOR_XRGB(0, 0, 0xFF));
	const D3DXCOLOR JCD3D_YELLOW(D3DCOLOR_XRGB(0xFF, 0xFF, 0));
	const D3DXCOLOR JCD3D_CYAN(D3DCOLOR_XRGB(0, 0xFF, 0xFF));
	const D3DXCOLOR JCD3D_MAGENTA(D3DCOLOR_XRGB(0xFF, 0, 0xFF));

	// Material
	D3DMATERIAL9 jcd3d_initMaterial(D3DXCOLOR ambient, D3DXCOLOR diffuse, D3DXCOLOR specular, D3DXCOLOR emissive, FLOAT power);
	const D3DMATERIAL9 JCD3D_WHITE_MATERIAL = jcd3d_initMaterial(JCD3D_WHITE, JCD3D_WHITE, JCD3D_WHITE, JCD3D_BLACK, 2.0f);
	const D3DMATERIAL9 JCD3D_RED_MATERIAL = jcd3d_initMaterial(JCD3D_RED, JCD3D_RED, JCD3D_RED, JCD3D_BLACK, 2.0f);
	const D3DMATERIAL9 JCD3D_GREEN_MATERIAL = jcd3d_initMaterial(JCD3D_GREEN, JCD3D_GREEN, JCD3D_GREEN, JCD3D_BLACK, 2.0f);
	const D3DMATERIAL9 JCD3D_BLUE_MATERIAL = jcd3d_initMaterial(JCD3D_BLUE, JCD3D_BLUE, JCD3D_BLUE, JCD3D_BLACK, 2.0f);
	const D3DMATERIAL9 JCD3D_YELLOW_MATERIAL = jcd3d_initMaterial(JCD3D_YELLOW, JCD3D_YELLOW, JCD3D_YELLOW, JCD3D_BLACK, 2.0f);

	// Light
	D3DLIGHT9 jcd3d_initDirectionalLight(D3DXVECTOR3* direction, D3DXCOLOR* color);
	D3DLIGHT9 jcd3d_initPointLight(D3DXVECTOR3* position, D3DXCOLOR* color);
	D3DLIGHT9 jcd3d_initSpotLight(D3DXVECTOR3* position, D3DXVECTOR3* direction, D3DXCOLOR* color);

	// Vertex
	struct JCD3D_Vertex_xyz_diffuse
	{
		FLOAT x, y, z;
		D3DCOLOR color;
		JCD3D_Vertex_xyz_diffuse(FLOAT px, FLOAT py, FLOAT pz, D3DCOLOR pcolor)
		{
			x = px; y = py; z = pz;
			color = pcolor;
		}
		static CONST DWORD FVF;
	};
	struct JCD3D_Vertex_xyz_normal
	{
		FLOAT x, y, z;
		FLOAT nx, ny, nz;
		JCD3D_Vertex_xyz_normal(FLOAT px, FLOAT py, FLOAT pz, FLOAT pnx, FLOAT pny, FLOAT pnz)
		{
			x = px; y = py; z = pz;
			nx = pnx; ny = pny; nz = pnz;
		}
		static CONST DWORD FVF;
	};
	struct JCD3D_Vertex_xyz_diffuse_texture
	{
		FLOAT x, y, z;
		D3DCOLOR color;
		FLOAT u, v;
		JCD3D_Vertex_xyz_diffuse_texture(FLOAT px, FLOAT py, FLOAT pz, D3DCOLOR pcolor, FLOAT pu, FLOAT pv)
		{
			x = px; y = py; z = pz;
			color = pcolor;
			u = pu; v = pv;
		}
		static CONST DWORD FVF;
	};
	struct JCD3D_Vertex_xyz_diffuse_texture2
	{
		FLOAT x, y, z;
		D3DCOLOR color;
		FLOAT u1, v1;
		FLOAT u2, v2;
		JCD3D_Vertex_xyz_diffuse_texture2(FLOAT px, FLOAT py, FLOAT pz, D3DCOLOR pcolor, FLOAT pu1, FLOAT pv1, FLOAT pu2, FLOAT pv2)
		{
			x = px; y = py; z = pz;
			color = pcolor;
			u1 = pu1; v1 = pv1;
			u2 = pu2; v2 = pv2;
		}
		static CONST DWORD FVF;
	};
}

#endif