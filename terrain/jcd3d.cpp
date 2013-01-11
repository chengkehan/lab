#include "jcd3d.h"

HWND jcd3d::jcd3d_hwnd = NULL;
IDirect3DDevice9* jcd3d::jcd3d_lpd3dd = NULL;
HINSTANCE jcd3d::jcd3d_hInstance = NULL;

CONST DWORD jcd3d::JCD3D_Vertex_xyzrhw_diffuse_texture::FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;
CONST DWORD jcd3d::JCD3D_Vertex_xyzrhw_texture::FVF = D3DFVF_XYZRHW | D3DFVF_TEX1;
CONST DWORD jcd3d::JCD3D_Vertex_xyz_diffuse::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
CONST DWORD jcd3d::JCD3D_Vertex_xyz_normal::FVF = D3DFVF_XYZ | D3DFVF_NORMAL;
CONST DWORD jcd3d::JCD3D_Vertex_xyz_diffuse_texture::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;
CONST DWORD jcd3d::JCD3D_Vertex_xyz_diffuse_texture2::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX2;

INT jcd3d::jcd3d_windowX = 0;
INT jcd3d::jcd3d_windowY = 0;
INT jcd3d::jcd3d_windowWidth = 0;
INT jcd3d::jcd3d_windowHeight = 0;

jcd3d::JCCALLBACK jcd3d::jcd3d_windowMoveCallback = NULL;

BOOL jcd3d::jcd3d_initRenderState(LPDIRECT3DDEVICE9 lpd3dd, DWORD cullMode, BOOL lighting, BOOL zEnable, DWORD shadeMode , DWORD fillMode , BOOL alphaBlendEnable)
{
	if(lpd3dd == NULL)
	{
		return FALSE;
	}
	else
	{
		if(FAILED(lpd3dd->SetRenderState(D3DRS_CULLMODE, cullMode)))
		{
			return FALSE;
		}
		if(FAILED(lpd3dd->SetRenderState(D3DRS_LIGHTING, lighting)))
		{
			return FALSE;
		}
		if(FAILED(lpd3dd->SetRenderState(D3DRS_ZENABLE, zEnable)))
		{
			return FALSE;
		}
		if(FAILED(lpd3dd->SetRenderState(D3DRS_SHADEMODE, shadeMode)))
		{
			return FALSE;
		}
		if(FAILED(lpd3dd->SetRenderState(D3DRS_FILLMODE, fillMode)))
		{
			return FALSE;
		}
		if(FAILED(lpd3dd->SetRenderState(D3DRS_ALPHABLENDENABLE, alphaBlendEnable)))
		{
			return FALSE;
		}

		return TRUE;
	}
}

BOOL jcd3d::jcd3d_setProjectionPerspectiveTransform(LPDIRECT3DDEVICE9 lpd3dd, INT windowWidth, INT windowHeight)
{
	if(lpd3dd == NULL)
	{
		return NULL;
	}
	else
	{
		D3DXMATRIX out;
		D3DXMatrixPerspectiveFovLH(&out, D3DX_PI * 0.3f, (FLOAT)windowWidth / (FLOAT)windowHeight, 1.0f, 1000.0f);
		if(FAILED(lpd3dd->SetTransform(D3DTS_PROJECTION, &out)))
		{
			return FALSE;
		}
		else
		{
			return TRUE;
		}
	}
}

BOOL jcd3d::jcd3d_setProjectionOrthoTransform(LPDIRECT3DDEVICE9 lpd3dd, INT windowWidth, INT windowHeight)
{
	if(lpd3dd == NULL)
	{
		return NULL;
	}
	else
	{
		D3DXMATRIX out;
		D3DXMatrixOrthoLH(&out, (FLOAT)windowWidth, (FLOAT)windowHeight, 1.0f, 1000.0f);
		if(FAILED(lpd3dd->SetTransform(D3DTS_PROJECTION, &out)))
		{
			return FALSE;
		}
		else
		{
			return TRUE;
		}
	}
}

BOOL jcd3d::jcd3d_setViewTransform(LPDIRECT3DDEVICE9 lpd3dd, D3DXVECTOR3* lpeye, D3DXVECTOR3* lptarget, D3DXVECTOR3* lpup)
{
	if(lpd3dd == NULL || lpeye == NULL || lptarget == NULL || lpup == NULL)
	{
		return FALSE;
	}
	else
	{
		D3DXMATRIX out;
		D3DXMatrixLookAtLH(&out, lpeye, lptarget, lpup);
		if(FAILED(lpd3dd->SetTransform(D3DTS_VIEW, &out)))
		{
			return FALSE;
		}
		else
		{
			return TRUE;
		}
	}
}
BOOL jcd3d::jcd3d_setViewTransform(LPDIRECT3DDEVICE9 lpd3dd, FLOAT eyeX, FLOAT eyeY, FLOAT eyeZ, FLOAT targetX, FLOAT targetY, FLOAT targetZ, FLOAT upX, FLOAT upY, FLOAT upZ)
{
	if(lpd3dd == NULL)
	{
		return FALSE;
	}
	else
	{
		D3DXMATRIX out;
		D3DXVECTOR3 eye(eyeX, eyeY, eyeZ);
		D3DXVECTOR3 target(targetX, targetY, targetZ);
		D3DXVECTOR3 up(upX, upY, upZ);
		D3DXMatrixLookAtLH(&out, &eye, &target, &up);
		if(FAILED(lpd3dd->SetTransform(D3DTS_VIEW, &out)))
		{
			return FALSE;
		}
		else
		{
			return TRUE;
		}
	}
}

BOOL jcd3d::jcd3d_init(HINSTANCE hInstance, INT windowX, INT windowY, INT windowWidth, INT windowHeight, BOOL windowed, D3DDEVTYPE deviceType, DWORD maxTextureBlendStages)
{
	jcd3d_hInstance = hInstance;
	jcd3d_windowX = windowX;
	jcd3d_windowY = windowY;
	jcd3d_windowWidth = windowWidth;
	jcd3d_windowHeight = windowHeight;

	// 创建窗口
	WNDCLASS wc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = jcd3d_wndProc;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = L"jcd3dApp";

	if(!RegisterClass(&wc))
	{
		jcwin32_messageBoxErrorM("RegiserClass Failed");
		return FALSE;
	}

	HWND hwnd = CreateWindow(L"jcd3dApp", L"JCD3D_APP", WS_EX_TOPMOST, windowX, windowY, windowWidth, windowHeight, NULL, NULL, hInstance, NULL);
	if(!hwnd)
	{
		jcwin32_messageBoxErrorM("CreateWindow Failed");
		return FALSE;
	}

	jcd3d_hwnd = hwnd;
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	// 初始化D3D
	LPDIRECT3D9 d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
	if(!d3d9)
	{
		jcwin32_messageBoxErrorM("Direct3DCreate9 Failed");
		return FALSE;
	}

	D3DCAPS9 caps;
	INT vp;
	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, deviceType, &caps);
	if(caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	if(caps.MaxTextureBlendStages < maxTextureBlendStages)
	{
		jcwin32_messageBoxErrorM("MaxTextureBlendStages Failed");
		return FALSE;
	}

	D3DPRESENT_PARAMETERS d3dpp;
	d3dpp.BackBufferWidth = windowWidth;
	d3dpp.BackBufferHeight = windowHeight;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONMASKABLE;
	d3dpp.MultiSampleQuality = 0;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hwnd;
	d3dpp.Windowed = windowed;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.Flags = 0;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT; // D3DPRESENT_INTERVAL_IMMEDIATE

	HRESULT hr = d3d9->CreateDevice(D3DADAPTER_DEFAULT, deviceType, hwnd, vp, &d3dpp, &jcd3d_lpd3dd);
	if(FAILED(hr))
	{
		d3d9->Release();
		jcwin32_messageBoxErrorM("CreateDevice Failed");
		return FALSE;
	}

	if(!jcd3d_setup())
	{
		d3d9->Release();
		jcd3d_release();
		jcwin32_messageBoxErrorM("jcd3d_setup Failed");
		return FALSE;
	}

	d3d9->Release();

	return TRUE;
}

VOID jcd3d::jcd3d_loop()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	DWORD lastTime = timeGetTime();
	while(msg.message != WM_QUIT)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			DWORD currTime = timeGetTime();
			DWORD timeDelta = currTime - lastTime;
			jcd3d_lpd3dd->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
			jcd3d_lpd3dd->BeginScene();
			jcd3d_display(currTime - lastTime);
			jcd3d_lpd3dd->EndScene();
			jcd3d_lpd3dd->Present(NULL, NULL, NULL, NULL);
			lastTime = currTime;
		}
	}

	jcd3d_release();
	jccommon_releaseComM(jcd3d_lpd3dd);
	jcd3d_hInstance = NULL;
	jcd3d_hwnd = NULL;
	jcd3d_windowMoveCallback = NULL;
}

LRESULT CALLBACK jcd3d::jcd3d_wndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(msg)
	{
		case WM_DESTROY:
		{
			DestroyWindow(hwnd);
			break;
		}
		case WM_MOVE:
		{
			if(jcd3d_windowMoveCallback != NULL)
			{
				jcd3d_windowMoveCallback();
			}
			break;
		}
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

D3DMATERIAL9 jcd3d::jcd3d_initMaterial(D3DXCOLOR ambient, D3DXCOLOR diffuse, D3DXCOLOR specular, D3DXCOLOR emissive, FLOAT power)
{
	D3DMATERIAL9 mtrl;
	mtrl.Ambient = ambient;
	mtrl.Diffuse = diffuse;
	mtrl.Specular = specular;
	mtrl.Emissive = emissive;
	mtrl.Power = power;

	return mtrl;
}

D3DLIGHT9 jcd3d::jcd3d_initDirectionalLight(D3DXVECTOR3* direction, D3DXCOLOR* color)
{
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(D3DLIGHT9));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Ambient = (*color) * 0.4f;
	light.Diffuse = *color;
	light.Specular = (*color) * 0.6f;
	light.Direction = *direction;

	return light;
}

D3DLIGHT9 jcd3d::jcd3d_initPointLight(D3DXVECTOR3* position, D3DXCOLOR* color)
{
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(D3DLIGHT9));
	light.Type = D3DLIGHT_POINT;
	light.Ambient = (*color) * 0.4f;
	light.Diffuse = *color;
	light.Specular = (*color) * 0.6f;
	light.Position = *position;
	light.Range = 1000.0f;
	light.Falloff = 1.0f;
	light.Attenuation0 = 1.0f;
	light.Attenuation1 = 0.0f;
	light.Attenuation2 = 0.0f;

	return light;
}

D3DLIGHT9 jcd3d::jcd3d_initSpotLight(D3DXVECTOR3* position, D3DXVECTOR3* direction, D3DXCOLOR* color)
{
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(D3DLIGHT9));
	light.Type = D3DLIGHT_SPOT;
	light.Ambient = *color * 0.4f;
	light.Diffuse = *color;
	light.Specular = *color * 0.6f;
	light.Position = *position;
	light.Direction = *direction;
	light.Range  = 1000.0f;
	light.Falloff = 1.0f;
	light.Attenuation0 = 1.0f;
	light.Attenuation1 = 0.0f;
	light.Attenuation2 = 0.0f;
	light.Theta = 0.5f;
	light.Phi = 0.7f;

	return light;
}