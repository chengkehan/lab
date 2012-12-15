#ifndef __D3D_XFRAME_EX_H__
#define __D3D_XFRAME_EX_H__

#include <Windows.h>
#include <d3dx9.h>

class D3DXFrameEx : public D3DXFRAME
{
public:
	D3DXFrameEx();
	~D3DXFrameEx();

	D3DXMATRIX matrixCombine;
	D3DXMATRIX matrixOriginal;

private:
	D3DXFrameEx(CONST D3DXFrameEx& value);
};

#endif