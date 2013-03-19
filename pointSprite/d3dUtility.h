/*********************************************************************************
PURPOISE:
	Provides utility functions for simplifying common tasks.
*********************************************************************************/

#ifndef D3D_UTILITY_H
#define D3D_UTILITY_H

#pragma warning(disable : 4819)

#include <d3dx9.h>

template<typename T>
void safe_release(T obj)
{
	if(obj == NULL)
		return;

	obj->Release();
	obj = NULL;
}

template<typename T>
void safe_delete(T obj)
{
	if(obj == NULL)
		return;

	delete obj;
	obj = NULL;
}

///////////////////////////////////////////////////////////////////////////////////

typedef bool (*DISPLAY_FUNC_PTR)(float time_delta);

bool init_d3d(HINSTANCE instance,			// application instance
			  int width, int height,		// backbuffer dimensions
			  bool is_window,				// true - windowed mode, false - full screen mode.
			  D3DDEVTYPE device_type,		// HAL or REF
			  IDirect3DDevice9** device);	// the create device

int enter_msg_loop(DISPLAY_FUNC_PTR display);

LRESULT CALLBACK wnd_proc(HWND hwnd, UINT msg, WPARAM word_param, LPARAM long_param);

// lights
D3DLIGHT9 init_directional_light(D3DXVECTOR3* direction, D3DXCOLOR* color);
D3DLIGHT9 init_point_light(D3DXVECTOR3* position, D3DXCOLOR* color);
D3DLIGHT9 init_spot_light(D3DXVECTOR3* position, D3DXVECTOR3* direction, D3DXCOLOR* color);

// materials
D3DMATERIAL9 init_material(D3DXCOLOR ambient, D3DXCOLOR diffuse, D3DXCOLOR specular, 
						   D3DXCOLOR emissive, float power);

///////////////////////////////////////////////////////////////////////////////////

const D3DXCOLOR      WHITE 			= D3DCOLOR_XRGB(255, 255, 255);
const D3DXCOLOR      BLACK 			= D3DCOLOR_XRGB(  0,   0,   0);
const D3DXCOLOR        RED 			= D3DCOLOR_XRGB(255,   0,   0);
const D3DXCOLOR      GREEN 			= D3DCOLOR_XRGB(  0, 255,   0);
const D3DXCOLOR       BLUE 			= D3DCOLOR_XRGB(  0,   0, 255);
const D3DXCOLOR     YELLOW 			= D3DCOLOR_XRGB(255, 255,   0);
const D3DXCOLOR       CYAN 			= D3DCOLOR_XRGB(  0, 255, 255);
const D3DXCOLOR    MAGENTA 			= D3DCOLOR_XRGB(255,   0, 255);

const D3DXCOLOR BEACH_SAND  		= D3DCOLOR_XRGB(255, 249, 157);
const D3DXCOLOR DESERT_SAND 		= D3DCOLOR_XRGB(250, 205, 135);

const D3DXCOLOR LIGHTGREEN  		= D3DCOLOR_XRGB( 60, 184, 120);
const D3DXCOLOR PUREGREEN   		= D3DCOLOR_XRGB(  0, 166,  81);
const D3DXCOLOR DARKGREEN   		= D3DCOLOR_XRGB(  0, 114,  54);

const D3DXCOLOR LIGHT_YELLOW_GREEN	= D3DCOLOR_XRGB(124, 197, 118);
const D3DXCOLOR PURE_YELLOW_GREEN	= D3DCOLOR_XRGB( 57, 181,  74);
const D3DXCOLOR DARK_YELLOW_GREEN	= D3DCOLOR_XRGB( 25, 123,  48);

const D3DXCOLOR LIGHTBROWN 			= D3DCOLOR_XRGB(198, 156, 109);
const D3DXCOLOR DARKBROWN  			= D3DCOLOR_XRGB(115, 100,  87);

const D3DMATERIAL9 WHITE_MATERIAL  = init_material(WHITE,  WHITE,  WHITE,  BLACK, 2.0f);
const D3DMATERIAL9 RED_MATERIAL	   = init_material(RED,	   RED,	   RED,    BLACK, 2.0f);
const D3DMATERIAL9 GREEN_MATERIAL  = init_material(GREEN,  GREEN,  GREEN,  BLACK, 2.0f);
const D3DMATERIAL9 BLUE_MATERIAL   = init_material(BLUE,   BLUE,   BLUE,   BLACK, 2.0f);
const D3DMATERIAL9 YELLOW_MATERIAL = init_material(YELLOW, YELLOW, YELLOW, BLACK, 2.0f);

///////////////////////////////////////////////////////////////////////////////////

const float INFINITY = FLT_MAX;
const float EPSILON  = 0.001f;

class cBoundingBox
{
public:
	D3DXVECTOR3 m_min, m_max;
	
	cBoundingBox();
	bool is_point_inside(D3DXVECTOR3& point);
};

class cBoundingSphere
{
public:
	D3DXVECTOR3 m_center;
	float		m_radius;

	cBoundingSphere();
};

///////////////////////////////////////////////////////////////////////////////////

class cTextureVertex
{
public:
	float m_x, m_y, m_z;
	float m_nx, m_ny, m_nz;
	float m_u, m_v;

	cTextureVertex() { }

	cTextureVertex(float x, float y, float z, 
				   float nx, float ny, float nz,
				   float u, float v)
	{
		m_x  = x;  m_y  = y;  m_z  = z;
		m_nx = nx; m_ny = ny; m_nz = nz;
		m_u  = u;  m_v  = v;
	}	
};

const DWORD TEXTURE_VERTEX_FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

// function references "desert.bmp" internally, this file must be in the working directory.
bool draw_basic_scene(
	IDirect3DDevice9* device,	// pass in NULL for cleanup
	float scale);				// uniform scale

///////////////////////////////////////////////////////////////////////////////////

// return random float in [low_bound, high_bound] interval
float get_random_float(float low_bound, float high_bound);

// returns a random vector in the bounds specified by min and max
void get_random_vector(D3DXVECTOR3* out, D3DXVECTOR3* min, D3DXVECTOR3* max);

// conversion
DWORD float_to_dword(float f);

// interpolation
float lerp(float a, float b, float t);

#endif



