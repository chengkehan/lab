#ifndef __JC_XMESHCONTAINER_H__
#define __JC_XMESHCONTAINER_H__

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>

class JCXMeshContainer : public D3DXMESHCONTAINER
{
public:
	JCXMeshContainer();
	~JCXMeshContainer();

	IDirect3DTexture9** lplpTextures;
	ID3DXMesh* lpSkinMesh;

	D3DXMATRIX** lplpFrameMatrices;
	D3DXMATRIX* lpBoneMatrices;

private:
	JCXMeshContainer(CONST JCXMeshContainer& value);
};

#endif