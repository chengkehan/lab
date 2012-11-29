#include "JCD3D.h"

using namespace jcd3d;

MSGMAP jcd3d::jcd3d_msgMap;

JCD3D::JCD3D():
m_hWnd(NULL), m_hInstance(NULL), m_lpd3dd(NULL), 
m_windowX(0), m_windowY(0), m_windowWidth(0), m_windowHeight(0), m_windowd(FALSE), 
m_init(FALSE), m_running(FALSE)
{
	setMessageCallback(WM_DESTROY, jcd3d_winDestroyProc);
	setMessageCallback(WM_KEYDOWN, jcd3d_winCloseProc);
}

JCD3D::~JCD3D()
{
	if(release != NULL)
	{
		release();
	}
	jccommon_releaseComM(m_lpd3dd);
}

BOOL JCD3D::init(HINSTANCE hInstance, INT windowX, INT windowY, INT windowWidth, INT windowHeight, BOOL windowed, 
	D3DDEVTYPE deviceType, DWORD maxTextureBlendStages, UINT fps)
{
	if(m_init)
	{
		return TRUE;
	}

	m_hInstance = hInstance;
	m_windowX = windowX;
	m_windowY = windowY;
	m_windowWidth = windowWidth;
	m_windowHeight = windowHeight;

	setFPS(fps);

	// Create window
	WNDCLASS wc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = jcd3d_winProc;
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

	HWND hwnd = CreateWindow(L"jcd3dApp", L"jcd3dApp", WS_EX_TOPMOST, windowX, windowY, windowWidth, windowHeight, NULL, NULL, hInstance, NULL);
	if(!hwnd)
	{
		jcwin32_messageBoxErrorM("CreateWindow Failed");
		return FALSE;
	}

	m_hWnd = hwnd;
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	// init D3D
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

	HRESULT hr = d3d9->CreateDevice(D3DADAPTER_DEFAULT, deviceType, hwnd, vp, &d3dpp, &m_lpd3dd);
	if(FAILED(hr))
	{
		d3d9->Release();
		jcwin32_messageBoxErrorM("CreateDevice Failed");
		return FALSE;
	}

	if(setup != NULL)
	{
		if(!setup())
		{
			d3d9->Release();
			if(release != NULL)
			{
				release();
			}
			jcwin32_messageBoxErrorM("jcd3d_setup Failed");
			return FALSE;
		}
	}

	d3d9->Release();
	m_init = TRUE;

	return TRUE;
}

VOID JCD3D::run()
{
	if(!m_init)
	{
		return;
	}
	if(m_running)
	{
		return;
	}
	m_running = TRUE;

	MSG msg;
	jccommon_zeromem(&msg, sizeof(MSG));

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
			if(currTime - lastTime >= m_fpsTime)
			{
				DWORD timeDelta = currTime - lastTime;
				m_lpd3dd->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
				m_lpd3dd->BeginScene();
				if(frame != NULL)
				{
					frame(currTime - lastTime);
				}
				m_lpd3dd->EndScene();
				m_lpd3dd->Present(NULL, NULL, NULL, NULL);
				lastTime = currTime;
			}
		}
	}
}

HWND JCD3D::getHWnd()
{
	return m_hWnd;
}

HINSTANCE JCD3D::getHInstance()
{
	return m_hInstance;
}

IDirect3DDevice9* JCD3D::getDirect3DDevice()
{
	return m_lpd3dd;
}

INT JCD3D::getWindowX()
{
	return m_windowX;
}

INT JCD3D::getWindowY()
{
	return m_windowY;
}

INT JCD3D::getWindowWidth()
{
	return m_windowWidth;
}

INT JCD3D::getWindowHeight()
{
	return m_windowHeight;
}

UINT JCD3D::getFPS()
{
	return m_fps;
}

VOID JCD3D::setFPS(UINT value)
{
	m_fps = value;
	m_fpsTime = 1000 / m_fps;
}

BOOL JCD3D::getWindowd()
{
	return m_windowd;
}

BOOL JCD3D::setMessageCallback(UINT msg, JCD3D_MessageCallback callback)
{
	if(callback == NULL)
	{
		return FALSE;
	}
	else
	{
		jcd3d_msgMap[msg] = callback;
		return TRUE;
	}
}

BOOL JCD3D::clearMessageCallback(UINT msg)
{
	if(containsMesageCallback(msg))
	{
		jcd3d_msgMap.erase(msg);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL JCD3D::containsMesageCallback(UINT msg)
{
	return jcd3d_msgMap.find(msg) != jcd3d_msgMap.end();
}

JCD3D_MessageCallback JCD3D::getMessageCallback(UINT msg)
{
	if(containsMesageCallback(msg))
	{
		return jcd3d_msgMap[msg];
	}
	else
	{
		return NULL;
	}
}

CONST D3DXCOLOR JCD3D::COLOR_WHITE(D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF));
CONST D3DXCOLOR JCD3D::COLOR_BLACK(D3DCOLOR_XRGB(0x00, 0x00, 0x00));
CONST D3DXCOLOR JCD3D::COLOR_RED(D3DCOLOR_XRGB(0xFF, 0x00, 0x00));
CONST D3DXCOLOR JCD3D::COLOR_GREEN(D3DCOLOR_XRGB(0x00, 0xFF, 0x00));
CONST D3DXCOLOR JCD3D::COLOR_BLUE(D3DCOLOR_XRGB(0x00, 0x00, 0xFF));
CONST D3DXCOLOR JCD3D::COLOR_YELLOW(D3DCOLOR_XRGB(0xFF, 0xFF, 0x00));
CONST D3DXCOLOR JCD3D::COLOR_CYAN(D3DCOLOR_XRGB(0x00, 0xFF, 0xFF));
CONST D3DXCOLOR JCD3D::COLOR_MAGENTA(D3DCOLOR_XRGB(0xFF, 0x00, 0xFF));

CONST D3DMATERIAL9 JCD3D::MATERIAL_WHITE = JCD3D::createMaterial(JCD3D::COLOR_WHITE, JCD3D::COLOR_WHITE, JCD3D::COLOR_WHITE, JCD3D::COLOR_BLACK, 2.0f);
CONST D3DMATERIAL9 JCD3D::MATERIAL_RED = JCD3D::createMaterial(JCD3D::COLOR_RED, JCD3D::COLOR_RED, JCD3D::COLOR_RED, JCD3D::COLOR_BLACK, 2.0f);
CONST D3DMATERIAL9 JCD3D::MATERIAL_GREEN = JCD3D::createMaterial(JCD3D::COLOR_GREEN, JCD3D::COLOR_GREEN, JCD3D::COLOR_GREEN, JCD3D::COLOR_BLACK, 2.0f);
CONST D3DMATERIAL9 JCD3D::MATERIAL_BLUE = JCD3D::createMaterial(JCD3D::COLOR_BLUE, JCD3D::COLOR_BLUE, JCD3D::COLOR_BLUE, JCD3D::COLOR_BLACK, 2.0f);
CONST D3DMATERIAL9 JCD3D::MATERIAL_YELLOW = JCD3D::createMaterial(JCD3D::COLOR_YELLOW, JCD3D::COLOR_YELLOW, JCD3D::COLOR_YELLOW, JCD3D::COLOR_YELLOW, 2.0f);

BOOL JCD3D::setRenderState( LPDIRECT3DDEVICE9 lpd3dd, DWORD cullMode, BOOL lighting, BOOL zEnable, DWORD shadeMode, DWORD fillMode, BOOL alphaBlendEnable)
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

BOOL JCD3D::setProjectionPerspectiveTransform(LPDIRECT3DDEVICE9 lpd3dd, INT windowWidth, INT windowHeight)
{
	if(lpd3dd == NULL)
	{
		return NULL;
	}
	else
	{
		D3DXMATRIX out;
		D3DXMatrixPerspectiveFovLH(&out, D3DX_PI * 0.5f, (FLOAT)windowWidth / (FLOAT)windowHeight, 1.0f, 1000.0f);
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

BOOL JCD3D::setProjectionOrthoTransform(LPDIRECT3DDEVICE9 lpd3dd, INT windowWidth, INT windowHeight)
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

BOOL JCD3D::setViewTransform(LPDIRECT3DDEVICE9 lpd3dd, D3DXVECTOR3* lpeye, D3DXVECTOR3* lptarget, D3DXVECTOR3* lpup)
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

BOOL JCD3D::setViewTransform(LPDIRECT3DDEVICE9 lpd3dd, FLOAT eyeX, FLOAT eyeY, FLOAT eyeZ, FLOAT targetX, FLOAT targetY, FLOAT targetZ, FLOAT upX, FLOAT upY, FLOAT upZ)
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

D3DMATERIAL9 JCD3D::createMaterial(D3DXCOLOR ambient, D3DXCOLOR diffuse, D3DXCOLOR specular, D3DXCOLOR emissive, FLOAT power)
{
	D3DMATERIAL9 mtrl;
	mtrl.Ambient = ambient;
	mtrl.Diffuse = diffuse;
	mtrl.Specular = specular;
	mtrl.Emissive = emissive;
	mtrl.Power = power;

	return mtrl;
}

D3DLIGHT9 JCD3D::createDirectionalLight(D3DXVECTOR3* direction, D3DXCOLOR* color)
{
	D3DLIGHT9 light;
	jccommon_zeromem(&light, sizeof(D3DLIGHT9));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Ambient = (*color) * 0.4f;
	light.Diffuse = *color;
	light.Specular = (*color) * 0.6f;
	light.Direction = *direction;
	
	return light;
}

D3DLIGHT9 JCD3D::createPointLight(D3DXVECTOR3* position, D3DXCOLOR* color)
{
	D3DLIGHT9 light;
	jccommon_zeromem(&light, sizeof(D3DLIGHT9));
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

D3DLIGHT9 JCD3D::createSpotLight(D3DXVECTOR3* position, D3DXVECTOR3* direction, D3DXCOLOR* color)
{
	D3DLIGHT9 light;
	jccommon_zeromem(&light, sizeof(D3DLIGHT9));
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

LRESULT CALLBACK jcd3d::jcd3d_winProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	for (MSGMAP::iterator iter = jcd3d_msgMap.begin(); iter != jcd3d_msgMap.end(); ++iter)
	{
		if(iter->first == msg && iter->second != NULL)
		{
			iter->second(hWnd, msg, wparam, lparam);
			break;
		}
	}
	return DefWindowProc(hWnd, msg, wparam, lparam);
}

VOID jcd3d::jcd3d_winDestroyProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	PostQuitMessage(0);
}

VOID jcd3d::jcd3d_winCloseProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if(wparam == VK_ESCAPE)
	{
		DestroyWindow(hWnd);
	}
}