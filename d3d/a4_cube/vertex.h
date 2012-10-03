#ifndef __VERTEX_H__
#define __VERTEX_H__

struct Vertex
{
	FLOAT x, y, z;
	FLOAT nx, ny, nz;
	FLOAT u, v;

	static const DWORD FVF;

	Vertex(FLOAT px, FLOAT py, FLOAT pz, FLOAT pnx, FLOAT pny, FLOAT pnz, FLOAT pu, FLOAT pv)
	{
		x = px;
		y = py;
		z = pz;
		nx = pnx;
		ny = pny;
		nz = pnz;
		u = pu;
		v = pv;
	}
};
const DWORD Vertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

#endif