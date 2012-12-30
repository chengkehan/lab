#include "JCXFileAnimation.h"

using namespace jcstring;

JCXFileAnimation::JCXFileAnimation(JCXFileBone* lpXFileBone)
{
	jccommon_assertM(lpXFileBone != NULL);
	m_lpAnimationSetRoot = NULL;
	m_lpXFileBone = lpXFileBone;
}

JCXFileAnimation::~JCXFileAnimation()
{
	cleanUp();
}

BOOL JCXFileAnimation::play(LPCSTR lpAnimationSetName, DWORD time)
{
	if(lpAnimationSetName == NULL)
	{
		return FALSE;
	}

	JCAnimationSet* lpAset = m_lpAnimationSetRoot;
	while(lpAset != NULL)
	{
		if(jcstring_csequ(lpAset->m_lpName, lpAnimationSetName))
		{
			m_lpXFileBone->resetHierarchy(m_lpXFileBone->getRootBone());

			INT times = time / lpAset->m_timeTotal;
			time = time - times * lpAset->m_timeTotal;
			
			JCAnimation* lpAnimation = lpAset->m_lpAnimations;
			while(lpAnimation != NULL)
			{
				D3DXMatrixIdentity(&lpAnimation->m_lpBone->TransformationMatrix);
				for(DWORD i = 0; i < lpAnimation->m_numTransformKeys; ++i)
				{
					JCAnimationKeyVector* lpAnimationKey = &lpAnimation->m_lpTransformKeys[i];
					if(time >= lpAnimationKey->m_time && (i == lpAnimation->m_numTransformKeys - 1 || time < lpAnimation->m_lpTransformKeys[i + 1].m_time))
					{
						D3DXMatrixTranslation(&lpAnimation->m_lpBone->TransformationMatrix, lpAnimationKey->m_vectorKey.x, lpAnimationKey->m_vectorKey.y, lpAnimationKey->m_vectorKey.z);
						break;
					}
				}
				for (DWORD i = 0; i < lpAnimation->m_numScaleKeys; ++i)
				{
					JCAnimationKeyVector* lpAnimationKey = &lpAnimation->m_lpScaleKeys[i];
					if(time >= lpAnimationKey->m_time && (i == lpAnimation->m_numScaleKeys - 1 || time < lpAnimation->m_lpScaleKeys[i + 1].m_time))
					{
						D3DXMatrixScaling(&lpAnimation->m_lpBone->TransformationMatrix, lpAnimationKey->m_vectorKey.x, lpAnimationKey->m_vectorKey.y, lpAnimationKey->m_vectorKey.z);
						break;
					}
				}
				for (DWORD i = 0; i < lpAnimation->m_numRotationKeys; ++i)
				{
					JCAnimationKeyQuaternion* lpAnimationKey = &lpAnimation->m_lpRotationKeys[i];
					if(time >= lpAnimationKey->m_time && (i == lpAnimation->m_numRotationKeys - 1 || time < lpAnimation->m_lpRotationKeys[i + 1].m_time))
					{
						D3DXMatrixRotationQuaternion(&lpAnimation->m_lpBone->TransformationMatrix, &lpAnimationKey->m_quaternionKey);
						break;
					}
				}
				for (DWORD i = 0; i < lpAnimation->m_numMatrixKey; ++i)
				{
					JCAnimationKeyMatrix* lpAnimationKey = &lpAnimation->m_lpMatrixKeys[i];
					if(time >= lpAnimationKey->m_time && (i == lpAnimation->m_numMatrixKey - 1 || time < lpAnimation->m_lpMatrixKeys[i + 1].m_time))
					{
						lpAnimation->m_lpBone->TransformationMatrix *= lpAnimationKey->m_matrixKey;
						break;
					}
				}

				lpAnimation = lpAnimation->m_lpNextAnimation;
			}

			m_lpXFileBone->updateHierarchy(m_lpXFileBone->getRootBone());

			return TRUE;
		}
	}

	return FALSE;
}

VOID* JCXFileAnimation::parseChild(ID3DXFileData* lpXFileData, BOOL isReference, ID3DXFileData* lpXFileDataParent, VOID* lpDataParent, GUID* lpGuid)
{
	if(*lpGuid == TID_D3DRMAnimationSet && !isReference)
	{
		JCAnimationSet* lpAnimationSet = new JCAnimationSet();
		lpAnimationSet->m_lpNextAnimationSet = m_lpAnimationSetRoot;
		m_lpAnimationSetRoot = lpAnimationSet;

		SIZE_T nameSize = 0;
		CHAR* lpName = NULL;
		xVerifyFailedIf(lpXFileData->GetName(NULL, &nameSize))
			return NULL;
		xVerifyFailedEndIf
		lpName = new CHAR[nameSize];
		xVerifyFailedIf(lpXFileData->GetName(lpName, &nameSize))
			jccommon_deleteArrayM(lpName);
			return NULL;
		xVerifyFailedEndIf
		lpAnimationSet->m_lpName = lpName;

		return lpAnimationSet;
	}
	else if(*lpGuid == TID_D3DRMAnimation && !isReference)
	{
		jccommon_assertM(lpDataParent != NULL);

		JCAnimationSet* lpAnimationSet = (JCAnimationSet*)lpDataParent;
		JCAnimation* lpAnimation = new JCAnimation();
		lpAnimation->m_lpAnimationSet = lpAnimationSet;
		++lpAnimationSet->m_numAnimations;
		lpAnimation->m_lpNextAnimation = lpAnimationSet->m_lpAnimations;
		lpAnimationSet->m_lpAnimations = lpAnimation;
		
		return lpAnimation;
	}
	else if(*lpGuid == TID_D3DRMFrame && isReference && lpDataParent != NULL)
	{
		jccommon_assertM(lpDataParent != NULL);

		JCAnimation* lpAnimation = (JCAnimation*)lpDataParent;
		SIZE_T nameSize = 0;
		CHAR* lpName = NULL;
		xVerifyFailedIf(lpXFileData->GetName(NULL, &nameSize))
			return NULL;
		xVerifyFailedEndIf
		lpName = new CHAR[nameSize];
		xVerifyFailedIf(lpXFileData->GetName(lpName, &nameSize))
			jccommon_deleteArrayM(lpName);
			return NULL;
		xVerifyFailedEndIf
		JCXFrame* lpBone = m_lpXFileBone->findBone(lpName);
		jccommon_deleteArrayM(lpName);
		jccommon_assertM(lpBone != NULL);
		lpAnimation->m_lpBone = lpBone;

		return NULL;
	}
	else if(*lpGuid == TID_D3DRMAnimationKey && !isReference)
	{
		jccommon_assertM(lpDataParent != NULL);

		JCAnimation* lpAnimation = (JCAnimation*)lpDataParent;
		SIZE_T memSize = 0;
		LPCVOID lpMem = NULL;
		xVerifyFailedIf(lpXFileData->Lock(&memSize, &lpMem))
			return NULL;
		xVerifyFailedEndIf
		DWORD* lpMemData = (DWORD*)lpMem;
		DWORD keyType = *lpMemData; ++lpMemData;
		DWORD numKeys = *lpMemData; ++lpMemData;
		switch(keyType)
		{
			// Rotation
			case 0:
			{
				lpAnimation->m_numRotationKeys = numKeys;
				lpAnimation->m_lpRotationKeys = new JCAnimationKeyQuaternion[numKeys];
				for (DWORD i = 0; i < numKeys; ++i)
				{
					lpAnimation->m_lpRotationKeys[i].m_time = *lpMemData; ++lpMemData;
					if(lpAnimation->m_lpRotationKeys[i].m_time > lpAnimation->m_lpAnimationSet->m_timeTotal)
					{
						lpAnimation->m_lpAnimationSet->m_timeTotal = lpAnimation->m_lpRotationKeys[i].m_time;
					}
					++lpMemData;

					FLOAT* lpMemDataF = (FLOAT*)lpMemData;
					lpAnimation->m_lpRotationKeys[i].m_quaternionKey.w = *lpMemDataF; ++lpMemDataF;
					lpAnimation->m_lpRotationKeys[i].m_quaternionKey.x = *lpMemDataF; ++lpMemDataF;
					lpAnimation->m_lpRotationKeys[i].m_quaternionKey.y = *lpMemDataF; ++lpMemDataF;
					lpAnimation->m_lpRotationKeys[i].m_quaternionKey.z = *lpMemDataF; ++lpMemDataF;
					lpMemData += 4;
				}
				break;
			}
			// Scale
			case 1:
			{
				lpAnimation->m_numScaleKeys = numKeys;
				lpAnimation->m_lpScaleKeys = new JCAnimationKeyVector[numKeys];
				for (DWORD i = 0; i < numKeys; ++i)
				{
					lpAnimation->m_lpScaleKeys[i].m_time = *lpMemData; ++lpMemData;
					if(lpAnimation->m_lpScaleKeys[i].m_time > lpAnimation->m_lpAnimationSet->m_timeTotal)
					{
						lpAnimation->m_lpAnimationSet->m_timeTotal = lpAnimation->m_lpScaleKeys[i].m_time;
					}
					++lpMemData;

					D3DXVECTOR3* lpMemDataVec = (D3DXVECTOR3*)lpMemData;
					lpAnimation->m_lpScaleKeys[i].m_vectorKey = *lpMemDataVec;
					lpMemData += 3;
				}
				break;
			}
			// Move
			case 2:
			{
				lpAnimation->m_numTransformKeys = numKeys;
				lpAnimation->m_lpTransformKeys = new JCAnimationKeyVector[numKeys];
				for (DWORD i = 0; i < numKeys; ++i)
				{
					lpAnimation->m_lpTransformKeys[i].m_time = *lpMemData; ++lpMemData;
					if(lpAnimation->m_lpTransformKeys[i].m_time > lpAnimation->m_lpAnimationSet->m_timeTotal)
					{
						lpAnimation->m_lpAnimationSet->m_timeTotal = lpAnimation->m_lpTransformKeys[i].m_time;
					}
					++lpMemData;

					D3DXVECTOR3* lpMemDataVec = (D3DXVECTOR3*)lpMemData;
					lpAnimation->m_lpTransformKeys[i].m_vectorKey = *lpMemDataVec;
					lpMemData += 3;
				}
				break;
			}
			// Matrix
			case 4:
			{
				lpAnimation->m_numMatrixKey = numKeys;
				lpAnimation->m_lpMatrixKeys = new JCAnimationKeyMatrix[numKeys];
				for (DWORD i = 0; i < numKeys; ++i)
				{
					lpAnimation->m_lpMatrixKeys[i].m_time = *lpMemData; ++lpMemData;
					if(lpAnimation->m_lpMatrixKeys[i].m_time > lpAnimation->m_lpAnimationSet->m_timeTotal)
					{
						lpAnimation->m_lpAnimationSet->m_timeTotal = lpAnimation->m_lpMatrixKeys[i].m_time;
					}
					++lpMemData;

					D3DXMATRIX* lpMemDataMatrix = (D3DXMATRIX*)lpMemData;
					lpAnimation->m_lpMatrixKeys[i].m_matrixKey = *lpMemDataMatrix;
					lpMemData += 16;
				}
				break;
			}
			default:
			{
				jccommon_assertM(FALSE);
			}
		}
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

VOID JCXFileAnimation::cleanUp()
{
	jccommon_deleteM(m_lpAnimationSetRoot);
}