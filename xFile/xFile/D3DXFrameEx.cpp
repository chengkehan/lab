#include "D3DXFrameEx.h"

D3DXFrameEx::D3DXFrameEx()
{
	Name = NULL;
	pMeshContainer = NULL;
	pFrameSibling = NULL;
	pFrameFirstChild = NULL;
	D3DXMatrixIdentity(&matrixCombine);
	D3DXMatrixIdentity(&matrixOriginal);
	D3DXMatrixIdentity(&TransformationMatrix);
}

D3DXFrameEx::~D3DXFrameEx()
{
	delete []Name;
	Name = NULL;
	delete pFrameSibling;
	pFrameSibling = NULL;
	delete pFrameFirstChild;
	pFrameFirstChild = NULL;
}