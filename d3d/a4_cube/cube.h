#ifndef __CUBE_H__
#define __CUBE_H__

#include <d3d9.h>
#include <d3dx9.h>

class Cube
{
public:
	Cube(LPDIRECT3DDEVICE9 lpd3dd);
	~Cube();

	BOOL draw(LPD3DXMATRIX lpworld, D3DMATERIAL9*, LPDIRECT3DTEXTURE9 lptex);

private:
	LPDIRECT3DDEVICE9 lpd3dd;
	LPDIRECT3DVERTEXBUFFER9 lpd3dvb;
	LPDIRECT3DINDEXBUFFER9 lpd3dib;
};

#endif