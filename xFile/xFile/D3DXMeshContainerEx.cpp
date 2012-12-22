#include "D3DXMeshContainerEx.h"

D3DXMeshContainerEx::D3DXMeshContainerEx()
{
	Name = NULL;
	MeshData.pMesh = NULL;
	pMaterials = NULL;
	pEffects = NULL;
	NumMaterials = 0;
	pAdjacency = NULL;
	pSkinInfo = NULL;
	pNextMeshContainer = NULL;
	lplpTextures = NULL;
	lpSkinMesh = NULL;
	lplpFrameMatrices = NULL;
	lpBoneMatrices = NULL;
}

D3DXMeshContainerEx::~D3DXMeshContainerEx()
{
	if(lplpTextures != NULL && NumMaterials)
	{
		for (DWORD i = 0; i < NumMaterials; ++i)
		{
			if(lplpTextures[i] != NULL)
			{
				lplpTextures[i]->Release();
				lplpTextures[i] = NULL;
			}
		}
	}
	delete []lplpTextures; lplpTextures = NULL;

	NumMaterials = 0;

	delete []Name; Name = NULL;
	delete []pMaterials; pMaterials = NULL;
	delete pEffects; pEffects = NULL;

	delete []pAdjacency; pAdjacency = NULL;
	delete []lplpFrameMatrices; lplpFrameMatrices = NULL;
	delete []lpBoneMatrices; lpBoneMatrices = NULL;

	if(MeshData.pMesh != NULL)
	{
		MeshData.pMesh->Release();
		MeshData.pMesh = NULL;
	}
	if(pSkinInfo != NULL)
	{
		pSkinInfo->Release();
		pSkinInfo = NULL;
	}
	if(lpSkinMesh != NULL)
	{
		lpSkinMesh->Release();
		lpSkinMesh = NULL;
	}

	delete pNextMeshContainer; pNextMeshContainer = NULL;
}