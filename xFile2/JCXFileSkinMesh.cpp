#include "JCXFileSkinMesh.h"

JCXFileSkinMesh::JCXFileSkinMesh(IDirect3DDevice9* lpd3dd, JCXFileBone* lpXFileBone):m_lpMeshContainerHeadNode(NULL),m_lpd3dd(lpd3dd), m_lpXFileBone(lpXFileBone)
{
	jccommon_assertM(lpd3dd != NULL);
	jccommon_assertM(lpXFileBone != NULL);
}

JCXFileSkinMesh::~JCXFileSkinMesh()
{

}

JCXMeshContainer* JCXFileSkinMesh::getMeshContainerHeadNode()
{
	return m_lpMeshContainerHeadNode;
}

VOID* JCXFileSkinMesh::parseChild(ID3DXFileData* lpXFileData, BOOL isReference, ID3DXFileData* lpXFileDataParent, VOID* lpDataParent, GUID* lpGuid)
{
	if(*lpGuid == TID_D3DRMMesh && !isReference)
	{
		ID3DXBuffer* lpMaterials = NULL;
		DWORD numMaterials = 0;
		ID3DXSkinInfo* lpSkinInfo = NULL;
		ID3DXMesh* lpMesh = NULL;
		ID3DXBuffer* lpAdjacency = NULL;
		xVerifyFailedIf(D3DXLoadSkinMeshFromXof(lpXFileData, D3DXMESH_SYSTEMMEM, m_lpd3dd, &lpAdjacency, &lpMaterials, NULL, &numMaterials, &lpSkinInfo, &lpMesh))
			return NULL;
		xVerifyFailedEndIf
		jccommon_assertM(lpMesh != NULL);
		jccommon_assertM(lpSkinInfo != NULL);
		DWORD numBones = lpSkinInfo->GetNumBones();
		jccommon_assertM(numBones > 0);
		jccommon_releaseComM(lpAdjacency);

		JCXMeshContainer* lpMeshContainer = new JCXMeshContainer();
		lpMeshContainer->pNextMeshContainer = m_lpMeshContainerHeadNode;
		m_lpMeshContainerHeadNode = lpMeshContainer;

		lpMeshContainer->MeshData.pMesh = lpMesh;
		lpMeshContainer->pSkinInfo = lpSkinInfo;
		lpMeshContainer->NumMaterials = numMaterials;
		lpMeshContainer->pMaterials = new D3DXMATERIAL[numMaterials];
		lpMeshContainer->lplpTextures = new IDirect3DTexture9*[numMaterials];
		
		D3DXMATERIAL* lpMaterialMemory = (D3DXMATERIAL*)lpMaterials->GetBufferPointer();
		for (DWORD materialIndex = 0; materialIndex < numMaterials; ++materialIndex)
		{
			lpMeshContainer->pMaterials[materialIndex].MatD3D = lpMaterialMemory[materialIndex].MatD3D;
			lpMeshContainer->pMaterials[materialIndex].pTextureFilename = NULL;
			lpMeshContainer->lplpTextures[materialIndex] = NULL;
			if(lpMaterialMemory[materialIndex].pTextureFilename != NULL)
			{
				xVerifyFailedIf(D3DXCreateTextureFromFileA(m_lpd3dd, lpMaterialMemory[materialIndex].pTextureFilename, &lpMeshContainer->lplpTextures[materialIndex]))
					// Do nothing
				xVerifyFailedEndIf
			}
		}
		jccommon_releaseComM(lpMaterials);

		xVerifyFailedIf(lpMeshContainer->MeshData.pMesh->CloneMeshFVF(D3DXMESH_SYSTEMMEM, lpMeshContainer->MeshData.pMesh->GetFVF(), m_lpd3dd, &lpMeshContainer->lpSkinMesh))
			return NULL;
		xVerifyFailedEndIf

		lpMeshContainer->lplpFrameMatrices = new D3DXMATRIX*[numBones];
		lpMeshContainer->lpBoneMatrices = new D3DXMATRIX[numBones];
		for (DWORD boneIndex = 0; boneIndex < numBones; ++boneIndex)
		{
			LPCSTR lpBoneName = lpSkinInfo->GetBoneName(boneIndex);
			JCXFrame* lpBone = m_lpXFileBone->findBone(lpBoneName);
			jccommon_assertM(lpBone != NULL);
			lpMeshContainer->lplpFrameMatrices[boneIndex] = &lpBone->matrixCombine;
		}

		return NULL;
	}
	else
	{
		return NULL;
	}
}

VOID JCXFileSkinMesh::cleanUp()
{
	jccommon_deleteM(m_lpMeshContainerHeadNode);
}