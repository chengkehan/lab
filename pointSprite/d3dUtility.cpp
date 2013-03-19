/*********************************************************************************
PURPOISE:
	Provides utility functions for simplifying common tasks.
*********************************************************************************/

#include "D3DUtility.h"

//-----------------------------------------------------------------------
// Initialize windows and direct 3D.
//-----------------------------------------------------------------------
bool init_d3d(HINSTANCE instance,			// application instance
			  int width, int height,		// backbuffer dimensions
			  bool is_window,				// true - windowed mode, false - full screen mode.
			  D3DDEVTYPE device_type,		// HAL or REF
			  IDirect3DDevice9** device)	// the create device
{
	const wchar_t* classname = L"Direct3D9App";

	WNDCLASS wc;
	
	wc.style		 = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = wnd_proc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = instance;
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = classname;

	if(! RegisterClass(&wc))
	{
		MessageBox(NULL, L"RegisterClass() - failed.", NULL, MB_OK);
		return false;
	}

	HWND hwnd = CreateWindow(classname, L"Direct3D9App", WS_EX_TOPMOST, 
							 0, 0, width, height, NULL, NULL, instance, NULL);
	
	if(hwnd == NULL)
	{
		MessageBox(NULL, L"CreateWindow() - failed.", NULL, MB_OK);
		return false;
	}

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	// initialize D3D

	// step 1: Create the IDirect3D9 object.

	IDirect3D9* d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

	if(d3d9 == NULL)
	{
		MessageBox(NULL, L"Direct3DCreate9() - failed.", NULL, MB_OK);
		return false;
	}

	// step 2: check for hardware vertex presentation.
	
	D3DCAPS9 caps;	
	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, device_type, &caps);
	
	int vp = 0;
	if(caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	// step 3: fill out the D3DPRESENT_PARAMETERS structure.

	D3DPRESENT_PARAMETERS d3dpp;

	d3dpp.BackBufferWidth				= width;
	d3dpp.BackBufferHeight				= height;
	d3dpp.BackBufferFormat				= D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount				= 1;
	d3dpp.MultiSampleType				= D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality			= 0;
	d3dpp.SwapEffect					= D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow					= hwnd;
	d3dpp.Windowed						= is_window;
	d3dpp.EnableAutoDepthStencil		= true;
	d3dpp.AutoDepthStencilFormat		= D3DFMT_D24S8;
	d3dpp.Flags							= 0;
	d3dpp.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval			= D3DPRESENT_INTERVAL_IMMEDIATE;

	// step 4: create the device.

	if(FAILED(d3d9->CreateDevice(D3DADAPTER_DEFAULT, device_type, hwnd, vp, &d3dpp, device)))
	{
		// try again using a 16-bit depth buffer
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

		if(FAILED(d3d9->CreateDevice(D3DADAPTER_DEFAULT, device_type, hwnd, vp, &d3dpp, device)))
		{
			d3d9->Release();	// done with d3d9 object
			MessageBox(NULL, L"CreateDevice() - failed.", NULL, MB_OK);
			return false;
		}
	}

	d3d9->Release();		// done with d3d9 object
	return true;
}

//-----------------------------------------------------------------------
// Enter windows message loop and render game frames if there is no message 
// comes from thread message queue.
//-----------------------------------------------------------------------
int enter_msg_loop(DISPLAY_FUNC_PTR display)
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	
	// The timeGetTime function retrieves the system time, in milliseconds. 
	// The system time is the time elapsed since Windows was started.	
	static float last_time = (float) timeGetTime();

	while(msg.message != WM_QUIT)
	{
		// The PeekMessage function dispatches incoming sent messages, checks the thread message queue for a 
		// posted message, and retrieves the message (if any exist).
		//
		// If a message is available, the return value is nonzero.
		// If no messages are available, the return value is zero. 

		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			float curr_time  = (float) timeGetTime();
			float time_delta = (curr_time - last_time) * 0.001f;

			display(time_delta);

			last_time = curr_time;

			Sleep(3);	// avoid cpu 100% use
		}
	}

	return (int) msg.wParam;
}

//////////////////////////////////////////////////////////////////////////////////////////////

D3DLIGHT9 init_directional_light(D3DXVECTOR3* direction, D3DXCOLOR* color)
{
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(light));

	light.Type		= D3DLIGHT_DIRECTIONAL;
	light.Ambient	= *color * 0.6f;
	light.Diffuse	= *color;
	light.Specular	= *color * 0.6f;
	light.Direction = *direction;

	return light;
}

D3DLIGHT9 init_point_light(D3DXVECTOR3* position, D3DXCOLOR* color)
{
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(light));

	light.Type			= D3DLIGHT_POINT;
	light.Ambient		= *color * 0.6f;
	light.Diffuse		= *color;
	light.Specular		= *color * 0.6f;
	light.Position		= *position;
	light.Range			= 1000.0f;
	light.Falloff		= 1.0f;
	light.Attenuation0	= 1.0f;
	light.Attenuation1	= 0.0f;
	light.Attenuation2	= 0.0f;

	return light;
}

D3DLIGHT9 init_spot_light(D3DXVECTOR3* position, D3DXVECTOR3* direction, D3DXCOLOR* color)
{
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(light));

	light.Type			= D3DLIGHT_SPOT;
	light.Ambient		= *color * 0.0f;
	light.Diffuse		= *color;
	light.Specular		= *color * 0.6f;
	light.Position		= *position;
	light.Direction		= *direction;
	light.Range			= 1000.0f;
	light.Falloff		= 1.0f;
	light.Attenuation0	= 1.0f;
	light.Attenuation1	= 0.0f;
	light.Attenuation2	= 0.0f;
	light.Theta			= 0.4f;
	light.Phi			= 0.9f;

	return light;
}

D3DMATERIAL9 init_material(D3DXCOLOR ambient, D3DXCOLOR diffuse, D3DXCOLOR specular, 
						   D3DXCOLOR emissive, float power)
{
	D3DMATERIAL9 material;

	material.Ambient  = ambient;
	material.Diffuse  = diffuse;
	material.Specular = specular;
	material.Emissive = emissive;
	material.Power	  = power;

	return material;
}

//////////////////////////////////////////////////////////////////////////////////////////////

cBoundingBox::cBoundingBox()
{	
	m_min.x = INFINITY;
	m_min.y = INFINITY;
	m_min.z = INFINITY;

	m_max.x = -INFINITY;
	m_max.y = -INFINITY;
	m_max.z = -INFINITY;
}

bool cBoundingBox::is_point_inside(D3DXVECTOR3 &point)
{
	return (point.x >= m_min.x && point.y >= m_min.y && point.z >= m_min.z &&
			point.x <= m_max.x && point.y <= m_max.y && point.z <= m_max.z);
}

cBoundingSphere::cBoundingSphere()
{
	m_radius = 0.0f;
}

//////////////////////////////////////////////////////////////////////////////////////////////

bool draw_basic_scene(IDirect3DDevice9* device, float scale)
{
	static IDirect3DVertexBuffer9*  floor_vb	= NULL;
	static IDirect3DTexture9*		texture		= NULL;
	static ID3DXMesh*				pillar_mesh = NULL;

	if(device == NULL)
	{
		if(floor_vb && texture && pillar_mesh)
		{
			// they already exist, destroy them.
			safe_release<IDirect3DVertexBuffer9*>(floor_vb);
			safe_release<IDirect3DTexture9*>(texture);
			safe_release<ID3DXMesh*>(pillar_mesh);
		}
	}
	else if(floor_vb == NULL && texture == NULL && pillar_mesh == NULL)
	{
		// they don't exist, create them.

		device->CreateVertexBuffer(6 * sizeof(cTextureVertex), 0, TEXTURE_VERTEX_FVF, D3DPOOL_MANAGED, &floor_vb, 0);

		cTextureVertex* v;

		floor_vb->Lock(0, 0, (void**)&v, 0);

		v[0] = cTextureVertex(-20.0f, -2.5f, -20.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
		v[1] = cTextureVertex(-20.0f, -2.5f,  20.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
		v[2] = cTextureVertex( 20.0f, -2.5f,  20.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);

		v[3] = cTextureVertex(-20.0f, -2.5f, -20.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
		v[4] = cTextureVertex( 20.0f, -2.5f,  20.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
		v[5] = cTextureVertex( 20.0f, -2.5f, -20.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);

		floor_vb->Unlock();

		D3DXCreateCylinder(device, 0.5f, 0.5f, 5.0f, 20, 20, &pillar_mesh, NULL);
		D3DXCreateTextureFromFile(device, L"desert.bmp", &texture);
	}
	else
	{
		// pre-render setup

		device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

		D3DXVECTOR3 dir(0.707f, -0.707f, 0.707f);
		D3DXCOLOR	color(1.0f, 1.0f, 1.0f, 1.0f);
		D3DLIGHT9	light = init_directional_light(&dir, &color);

		device->SetLight(0, &light);
		device->LightEnable(0, TRUE);
		device->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
		device->SetRenderState(D3DRS_SPECULARENABLE,   TRUE);

		// render

		D3DXMATRIX transform_matrix, rotate_matrix, pillor_matrix, scale_matrix;
		D3DXMatrixScaling(&scale_matrix, scale, scale, scale);

		// used to rotate cylinders to be parallel with world's y-axis
		D3DXMatrixRotationX(&rotate_matrix, -D3DX_PI * 0.5f);

		// draw floor

		D3DXMatrixIdentity(&transform_matrix);
		transform_matrix *= scale_matrix;
		device->SetTransform(D3DTS_WORLD, &transform_matrix);

		device->SetMaterial(&WHITE_MATERIAL);
		device->SetTexture(0, texture);
		device->SetStreamSource(0, floor_vb, 0, sizeof(cTextureVertex));
		device->SetFVF(TEXTURE_VERTEX_FVF);

		device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

		// draw pillars

		device->SetMaterial(&BLUE_MATERIAL);
		device->SetTexture(0, NULL);

		for(int i = 0; i < 5; i++)
		{
			// left pillar
			D3DXMatrixTranslation(&transform_matrix, -5.0f, 0.0f, -15.0f + (i * 7.5f));
			pillor_matrix = rotate_matrix * transform_matrix * scale_matrix;
			device->SetTransform(D3DTS_WORLD, &pillor_matrix);

			pillar_mesh->DrawSubset(0);

			// right pillar
			D3DXMatrixTranslation(&transform_matrix, 5.0f, 0.0f, -15.0f + (i * 7.5f));
			pillor_matrix = rotate_matrix * transform_matrix * scale_matrix;
			device->SetTransform(D3DTS_WORLD, &pillor_matrix);

			pillar_mesh->DrawSubset(0);
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////

float get_random_float(float low_bound, float high_bound)
{
	if(low_bound >= high_bound)	// bad input
		return low_bound;

	// get random float in [0, 1] interval
	float f = (rand() % 10000) * 0.0001f;

	// return float in [low_bound, high_bound] interval
	return f * (high_bound - low_bound) + low_bound;
}

void get_random_vector(D3DXVECTOR3* out, D3DXVECTOR3* min, D3DXVECTOR3* max)
{
	out->x = get_random_float(min->x, max->x);
	out->y = get_random_float(min->y, max->y);
	out->z = get_random_float(min->z, max->z);
}

DWORD float_to_dword(float f)
{
	return *((DWORD*)&f);
}

float lerp(float a, float b, float t)
{
	return a * (1 - t) + (b * t);
}

