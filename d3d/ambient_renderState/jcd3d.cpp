#include "jcd3d.h"

CONST DWORD jcd3d::JCD3D_Vertex_xyz_diffuse::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
CONST DWORD jcd3d::JCD3D_Vertex_xyz_normal::FVF = D3DFVF_XYZ | D3DFVF_NORMAL;

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

BOOL jcd3d::jcd3d_init(HINSTANCE hInstance, INT width, INT height, BOOL windowed, D3DDEVTYPE deviceType, LPDIRECT3DDEVICE9* device)
{
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
		MessageBox(NULL, L"RegiserClass Failed", NULL, MB_OK);
		return FALSE;
	}

	HWND hwnd = CreateWindow(L"jcd3dApp", L"JCD3D_APP", WS_EX_TOPMOST, 0, 0, width, height, NULL, NULL, hInstance, NULL);
	if(!hwnd)
	{
		MessageBox(NULL, L"CreateWindow Failed", NULL, MB_OK);
		return FALSE;
	}

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	// 初始化D3D
	LPDIRECT3D9 d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
	if(!d3d9)
	{
		MessageBox(NULL, L"Direct3DCreate9 Failed", NULL, MB_OK);
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

	D3DPRESENT_PARAMETERS d3dpp;
	d3dpp.BackBufferWidth = width;
	d3dpp.BackBufferHeight = height;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;
	//d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
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

	HRESULT hr = d3d9->CreateDevice(D3DADAPTER_DEFAULT, deviceType, hwnd, vp, &d3dpp, device);
	if(FAILED(hr))
	{
		d3d9->Release();
		MessageBox(NULL, L"CreateDevice Failed", NULL, MB_OK);
		return FALSE;
	}

	if(!jcd3d_setup())
	{
		d3d9->Release();
		MessageBox(NULL, L"jcd3d_setup Failed", NULL, MB_OK);
		return FALSE;
	}

	d3d9->Release();

	return TRUE;
}

VOID jcd3d::jcd3d_loop(VOID (*display)(DWORD timeDelta))
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
			display(currTime - lastTime);
			lastTime = currTime;
		}
	}
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