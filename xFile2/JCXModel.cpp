#include "JCXModel.h"

JCXModel::JCXModel(IDirect3DDevice9* lpd3dd):m_lpBone(NULL), m_lpSkinMesh(NULL), m_lpd3dd(lpd3dd)
{
	jccommon_assertM(lpd3dd);
}

JCXModel::~JCXModel()
{
	cleanUp();	
}

VOID JCXModel::draw()
{
	JCXMeshContainer* lpSkinMesh = m_lpSkinMesh == NULL ? NULL : m_lpSkinMesh->getMeshContainerHeadNode();
	while(lpSkinMesh != NULL)
	{
		for (DWORD boneIndex = 0; boneIndex < lpSkinMesh->pSkinInfo->GetNumBones(); ++ boneIndex)
		{
			lpSkinMesh->lpBoneMatrices[boneIndex] = *lpSkinMesh->pSkinInfo->GetBoneOffsetMatrix(boneIndex);
			if(lpSkinMesh->lplpFrameMatrices[boneIndex] != NULL)
			{
				lpSkinMesh->lpBoneMatrices[boneIndex] *= *lpSkinMesh->lplpFrameMatrices[boneIndex];
			}
		}

		VOID* lpSrc, *lpDest;
		xVerifyFailedIf(lpSkinMesh->lpSkinMesh->LockVertexBuffer(D3DLOCK_READONLY, &lpSrc))xVerifyFailedEndIf
		xVerifyFailedIf(lpSkinMesh->MeshData.pMesh->LockVertexBuffer(0, &lpDest))xVerifyFailedEndIf
		xVerifyFailedIf(lpSkinMesh->pSkinInfo->UpdateSkinnedMesh(lpSkinMesh->lpBoneMatrices, NULL, lpSrc, lpDest))xVerifyFailedEndIf
		xVerifyFailedIf(lpSkinMesh->lpSkinMesh->UnlockVertexBuffer())xVerifyFailedEndIf
		xVerifyFailedIf(lpSkinMesh->MeshData.pMesh->UnlockVertexBuffer())xVerifyFailedEndIf

		for (DWORD materialIndex = 0; materialIndex < lpSkinMesh->NumMaterials; ++materialIndex)
		{
			xVerifyFailedIf(m_lpd3dd->SetMaterial(&lpSkinMesh->pMaterials[materialIndex].MatD3D))xVerifyFailedEndIf
			xVerifyFailedIf(m_lpd3dd->SetTexture(0, lpSkinMesh->lplpTextures[materialIndex]))xVerifyFailedEndIf
			xVerifyFailedIf(lpSkinMesh->MeshData.pMesh->DrawSubset(materialIndex))xVerifyFailedEndIf
		}

		lpSkinMesh = (JCXMeshContainer*)lpSkinMesh->pNextMeshContainer;
	}
}

BOOL JCXModel::parse(LPCSTR lpXFileData)
{
	cleanUp();
	
	m_lpBone = new JCXFileBone();
	if(!m_lpBone->parse(lpXFileData))
	{
		return FALSE;
	}

	JCXFileBoneTransform boneTransform(m_lpBone);
	if(!boneTransform.parse(lpXFileData))
	{
		return FALSE;
	}

	m_lpSkinMesh = new JCXFileSkinMesh(m_lpd3dd, m_lpBone);
	if(!m_lpSkinMesh->parse(lpXFileData))
	{
		return FALSE;
	}

	return TRUE;
}

JCXFileBone* JCXModel::getBone()
{
	return m_lpBone;
}

JCXFileSkinMesh* JCXModel::getSkinMesh()
{
	return m_lpSkinMesh;
}

VOID JCXModel::cleanUp()
{
	jccommon_deleteM(m_lpBone);
	jccommon_deleteM(m_lpSkinMesh);
}