#include "JCXFrame.h"

JCXFrame::JCXFrame()
{
	Name = NULL;
	pMeshContainer = NULL;
	pFrameSibling = NULL;
	pFrameFirstChild = NULL;
	D3DXMatrixIdentity(&matrixCombine);
	D3DXMatrixIdentity(&matrixOriginal);
	D3DXMatrixIdentity(&TransformationMatrix);
}

JCXFrame::~JCXFrame()
{
	delete []Name;
	Name = NULL;
	delete pFrameSibling;
	pFrameSibling = NULL;
	delete pFrameFirstChild;
	pFrameFirstChild = NULL;
}