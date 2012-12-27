#include "JCXFileBoneTransform.h"

JCXFileBoneTransform::JCXFileBoneTransform(JCXFileBone* lpXFileBone):m_lpXFileBone(lpXFileBone)
{
	jccommon_assertM(lpXFileBone != NULL);
}

JCXFileBoneTransform::~JCXFileBoneTransform()
{

}

VOID* JCXFileBoneTransform::parseChild(ID3DXFileData* lpXFileData, ID3DXFileData* lpXFileDataParent, VOID* lpDataParent, GUID* lpGuid)
{
	if(*lpGuid == TID_D3DRMFrameTransformMatrix)
	{
		jccommon_assertM(lpXFileDataParent != NULL);

		GUID guidParent;
		xVerifyFailedIf(lpXFileDataParent->GetType(&guidParent))
			return NULL;
		xVerifyFailedEndIf
		jccommon_assertM(guidParent == TID_D3DRMFrame);

		SIZE_T boneNameNumChars;
		CHAR* lpBoneName;
		xVerifyFailedIf(lpXFileDataParent->GetName(NULL, &boneNameNumChars))
			return NULL;
		xVerifyFailedEndIf
		lpBoneName = new CHAR[boneNameNumChars];
		xVerifyFailedIf(lpXFileDataParent->GetName(lpBoneName, &boneNameNumChars))
			jccommon_deleteArrayM(lpBoneName);
			return NULL;
		xVerifyFailedEndIf

		JCXFrame* lpBone = m_lpXFileBone->findBone(lpBoneName);
		jccommon_deleteArrayM(lpBoneName);
		jccommon_assertM(lpBone != NULL);

		SIZE_T dataSize;
		LPCVOID lpData;
		xVerifyFailedIf(lpXFileData->Lock(&dataSize, &lpData))
			return NULL;
		xVerifyFailedEndIf

		lpBone->TransformationMatrix = *(D3DXMATRIX*)lpData;
		lpBone->matrixOriginal = lpBone->TransformationMatrix;

		xVerifyFailedIf(lpXFileData->Unlock())
			return NULL;
		xVerifyFailedEndIf

		return NULL;
	}
	else
	{
		return NULL;
	}
}

VOID JCXFileBoneTransform::cleanUp()
{
	// Do nothing
}