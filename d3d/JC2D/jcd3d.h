#ifndef __JC_D3D_H__
#define __JC_D3D_H__

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <map>

#include "jccommon.h"
#include "jcwin32.h"

// window process
namespace jcd3d
{
	typedef VOID (*JCD3D_MessageCallback)(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam);
	// message mapping, key msg, value callback
	typedef std::map<UINT, JCD3D_MessageCallback> MSGMAP;

	extern MSGMAP jcd3d_msgMap;

	LRESULT CALLBACK jcd3d_winProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam);
	VOID jcd3d_winDestroyProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam);
	VOID jcd3d_winCloseProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam);
}

class JCD3D
{
public:
	JCD3D();
	~JCD3D();

	typedef BOOL (*SETUPCALLBACK)(VOID);
	typedef VOID (*RELEASECALLBACK)(VOID);
	typedef VOID (*FRAMECALLBACK)(DWORD timeDelta);

	SETUPCALLBACK setup;
	RELEASECALLBACK release;
	FRAMECALLBACK frame;

	BOOL init(HINSTANCE hInstance, INT windowX = 0, INT windowY = 0, INT windowWidth = 800, INT windowHeight = 600, 
		BOOL windowed = TRUE, D3DDEVTYPE deviceType = D3DDEVTYPE_HAL, DWORD maxTextureBlendStages = 1, UINT fps = 30);
	VOID run();
	HWND getHWnd();
	HINSTANCE getHInstance();
	IDirect3DDevice9* getDirect3DDevice();
	INT getWindowX();
	INT getWindowY();
	INT getWindowWidth();
	INT getWindowHeight();
	UINT getFPS();
	VOID setFPS(UINT value);
	BOOL getWindowd();
	BOOL setMessageCallback(UINT msg, jcd3d::JCD3D_MessageCallback callback);
	BOOL clearMessageCallback(UINT msg);
	BOOL containsMesageCallback(UINT msg);
	jcd3d::JCD3D_MessageCallback getMessageCallback(UINT msg);

	static CONST D3DXCOLOR COLOR_WHITE;
	static CONST D3DXCOLOR COLOR_BLACK;
	static CONST D3DXCOLOR COLOR_RED;
	static CONST D3DXCOLOR COLOR_GREEN;
	static CONST D3DXCOLOR COLOR_BLUE;
	static CONST D3DXCOLOR COLOR_YELLOW;
	static CONST D3DXCOLOR COLOR_CYAN;
	static CONST D3DXCOLOR COLOR_MAGENTA;

	static CONST D3DMATERIAL9 MATERIAL_WHITE;
	static CONST D3DMATERIAL9 MATERIAL_RED;
	static CONST D3DMATERIAL9 MATERIAL_GREEN;
	static CONST D3DMATERIAL9 MATERIAL_BLUE;
	static CONST D3DMATERIAL9 MATERIAL_YELLOW;

	static BOOL setRenderState(
		LPDIRECT3DDEVICE9 lpd3dd, DWORD cullMode = D3DCULL_CCW, BOOL lighting = TRUE, BOOL zEnable = TRUE, 
		DWORD shadeMode = D3DSHADE_GOURAUD, DWORD fillMode = D3DFILL_SOLID, BOOL alphaBlendEnable = FALSE
	);
	static BOOL setProjectionPerspectiveTransform(LPDIRECT3DDEVICE9 lpd3dd, INT windowWidth, INT windowHeight);
	static BOOL setProjectionOrthoTransform(LPDIRECT3DDEVICE9 lpd3dd, INT windowWidth, INT windowHeight);
	static BOOL setViewTransform(LPDIRECT3DDEVICE9 lpd3dd, D3DXVECTOR3* lpeye, D3DXVECTOR3* lptarget, D3DXVECTOR3* lpup);
	static BOOL setViewTransform(LPDIRECT3DDEVICE9 lpd3dd, FLOAT eyeX, FLOAT eyeY, FLOAT eyeZ, FLOAT targetX, FLOAT targetY, FLOAT targetZ, FLOAT upX, FLOAT upY, FLOAT upZ);
	static D3DMATERIAL9 createMaterial(D3DXCOLOR ambient, D3DXCOLOR diffuse, D3DXCOLOR specular, D3DXCOLOR emissive, FLOAT power);
	static D3DLIGHT9 createDirectionalLight(D3DXVECTOR3* direction, D3DXCOLOR* color);
	static D3DLIGHT9 createPointLight(D3DXVECTOR3* position, D3DXCOLOR* color);
	static D3DLIGHT9 createSpotLight(D3DXVECTOR3* position, D3DXVECTOR3* direction, D3DXCOLOR* color);

	struct Vertex_xyzrhw_texture
	{
		FLOAT x, y, z;
		FLOAT rhw;
		FLOAT u, v;
		Vertex_xyzrhw_texture(FLOAT px, FLOAT py, FLOAT pz, FLOAT prhw, FLOAT pu, FLOAT pv)
		{
			x = px; y = py; z = pz;
			rhw = prhw;
			u = pu; v = pv;
		}
		static CONST DWORD FVF;
	};
	struct Vertex_xyzrhw_diffuse_texture
	{
		FLOAT x, y, z;
		FLOAT rhw;
		D3DCOLOR color;
		FLOAT u, v;
		Vertex_xyzrhw_diffuse_texture(FLOAT px, FLOAT py, FLOAT pz, FLOAT prhw, D3DCOLOR pcolor, FLOAT pu, FLOAT pv)
		{
			x = px; y = py; z = pz;
			rhw = prhw;
			color = pcolor;
			u = pu; v = pv;
		}
		static CONST DWORD FVF;
	};
	struct Vertex_xyz_diffuse
	{
		FLOAT x, y, z;
		D3DCOLOR color;
		Vertex_xyz_diffuse(FLOAT px, FLOAT py, FLOAT pz, D3DCOLOR pcolor)
		{
			x = px; y = py; z = pz;
			color = pcolor;
		}
		static CONST DWORD FVF;
	};
	struct Vertex_xyz_normal
	{
		FLOAT x, y, z;
		FLOAT nx, ny, nz;
		Vertex_xyz_normal(FLOAT px, FLOAT py, FLOAT pz, FLOAT pnx, FLOAT pny, FLOAT pnz)
		{
			x = px; y = py; z = pz;
			nx = pnx; ny = pny; nz = pnz;
		}
		static CONST DWORD FVF;
	};
	struct Vertex_xyz_diffuse_texture
	{
		FLOAT x, y, z;
		D3DCOLOR color;
		FLOAT u, v;
		Vertex_xyz_diffuse_texture(FLOAT px, FLOAT py, FLOAT pz, D3DCOLOR pcolor, FLOAT pu, FLOAT pv)
		{
			x = px; y = py; z = pz;
			color = pcolor;
			u = pu; v = pv;
		}
		static CONST DWORD FVF;
	};
	struct Vertex_xyz_diffuse_texture2
	{
		FLOAT x, y, z;
		D3DCOLOR color;
		FLOAT u1, v1;
		FLOAT u2, v2;
		Vertex_xyz_diffuse_texture2(FLOAT px, FLOAT py, FLOAT pz, D3DCOLOR pcolor, FLOAT pu1, FLOAT pv1, FLOAT pu2, FLOAT pv2)
		{
			x = px; y = py; z = pz;
			color = pcolor;
			u1 = pu1; v1 = pv1;
			u2 = pu2; v2 = pv2;
		}
		static CONST DWORD FVF;
	};

private:
	JCD3D(CONST JCD3D& jcd3d);

	HWND m_hWnd;
	HINSTANCE m_hInstance;
	IDirect3DDevice9* m_lpd3dd;
	INT m_windowX;
	INT m_windowY;
	INT m_windowWidth;
	INT m_windowHeight;
	BOOL m_windowd;
	BOOL m_init;
	BOOL m_running;
	UINT m_fps;
	UINT m_fpsTime;
};

#endif