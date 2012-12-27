#ifndef __JC_XFRAME_H__
#define __JC_XFRAME_H__

#include <Windows.h>
#include <d3dx9.h>

class JCXFrame : public D3DXFRAME
{
public:
	JCXFrame();
	~JCXFrame();

	D3DXMATRIX matrixCombine;
	D3DXMATRIX matrixOriginal;

private:
	JCXFrame(CONST JCXFrame& value);
};

#endif