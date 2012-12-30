#include "JCXFileBone.h"

using namespace jcstring;

JCXFileBone::JCXFileBone():m_boneRoot(NULL)
{

}

JCXFileBone::~JCXFileBone()
{
	jccommon_deleteM(m_boneRoot);
}

JCXFrame* JCXFileBone::getRootBone()
{
	return m_boneRoot;
}

JCXFrame* JCXFileBone::findBone(LPCSTR lpBoneName)
{
	return fineBoneRecursion(lpBoneName, m_boneRoot);
}

BOOL JCXFileBone::updateHierarchy(JCXFrame* lpBone, D3DXMATRIX* lpTransform)
{
	if(lpBone == NULL)
	{
		return FALSE;
	}

	D3DXMATRIX matrixIdentity;
	if(lpTransform == NULL)
	{
		D3DXMatrixIdentity(&matrixIdentity);
		lpTransform = &matrixIdentity;
	}

	lpBone->matrixCombine = lpBone->TransformationMatrix * (*lpTransform);
	updateHierarchy((JCXFrame*)lpBone->pFrameSibling, lpTransform);
	updateHierarchy((JCXFrame*)lpBone->pFrameFirstChild, &lpBone->matrixCombine);

	return TRUE;
}

BOOL JCXFileBone::resetHierarchy(JCXFrame* lpBone)
{
	if(lpBone == NULL)
	{
		return FALSE;
	}

	lpBone->TransformationMatrix = lpBone->matrixOriginal;
	resetHierarchy((JCXFrame*)lpBone->pFrameSibling);
	resetHierarchy((JCXFrame*)lpBone->pFrameFirstChild);

	return TRUE;
}

VOID* JCXFileBone::parseChild(ID3DXFileData* lpXFileData, BOOL isReference, ID3DXFileData* lpXFileDataParent, VOID* lpDataParent, GUID* lpGuid)
{	
	if(*lpGuid == TID_D3DRMFrame && !isReference)
	{
		JCXFrame* lpBone = new JCXFrame();

		CHAR* lpBoneName = NULL;
		SIZE_T boneNameNumChars = 0;
		xVerifyFailedIf(lpXFileData->GetName(NULL, &boneNameNumChars))
			return NULL;
		xVerifyFailedEndIf
		lpBoneName = new CHAR[boneNameNumChars];
		lpBone->Name = lpBoneName;
		xVerifyFailedIf(lpXFileData->GetName(lpBoneName, &boneNameNumChars))
			jccommon_deleteM(lpBone);
			return NULL;
		xVerifyFailedEndIf

		if(lpXFileDataParent == NULL)
		{
			lpBone->pFrameSibling = m_boneRoot;
			m_boneRoot = lpBone;
		}
		else
		{
			jccommon_assertM(lpDataParent != NULL);
			JCXFrame* lpBoneParent = (JCXFrame*)lpDataParent;
			lpBone->pFrameSibling = lpBoneParent->pFrameFirstChild;
			lpBoneParent->pFrameFirstChild = lpBone;

		}

		return lpBone;
	}
	else
	{
		return NULL;
	}
}

VOID JCXFileBone::cleanUp()
{
	jccommon_deleteM(m_boneRoot);
}

JCXFrame* JCXFileBone::fineBoneRecursion(LPCSTR lpBoneName, JCXFrame* lpBone)
{
	if(lpBone == NULL)
	{
		return NULL;
	}

	if(jcstring_csequ(lpBoneName, lpBone->Name))
	{
		return lpBone;
	}

	JCXFrame* lpBoneResult = fineBoneRecursion(lpBoneName, (JCXFrame*)lpBone->pFrameSibling);
	if(lpBoneResult != NULL)
	{
		return lpBoneResult;
	}

	lpBoneResult = fineBoneRecursion(lpBoneName, (JCXFrame*)lpBone->pFrameFirstChild);
	if(lpBoneResult != NULL)
	{
		return lpBoneResult;
	}

	return NULL;
}