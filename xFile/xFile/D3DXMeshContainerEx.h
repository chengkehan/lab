#ifndef __D3D_XMESHCONTAINER_EX_H__
#define __D3D_XMESHCONTAINER_EX_H__

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>

class D3DXMeshContainerEx : public D3DXMESHCONTAINER
{
public:
	D3DXMeshContainerEx();
	~D3DXMeshContainerEx();

	IDirect3DTexture9** lplpTextures;
	ID3DXMesh* lpSkinMesh;

	D3DXMATRIX** lplpFrameMatrices;
	D3DXMATRIX* lpBoneMatrices;

private:
	D3DXMeshContainerEx(CONST D3DXMeshContainerEx& value);
};

#endif