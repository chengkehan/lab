#ifndef __JC_AMINATION_H__
#define __JC_AMINATION_H__

#include <Windows.h>

#include "JCXFrame.h"
#include "JCAnimationKeyMatrix.h"
#include "JCAnimationKeyQuaternion.h"
#include "JCAnimationKeyVector.h"
#include "jccommon.h"

class JCAnimationSet;

class JCAnimation
{
public:
	JCAnimation();
	~JCAnimation();

	JCXFrame* m_lpBone;
	JCAnimation* m_lpNextAnimation;
	JCAnimationSet* m_lpAnimationSet;
	
	DWORD m_numTransformKeys;
	JCAnimationKeyVector* m_lpTransformKeys;

	DWORD m_numScaleKeys;
	JCAnimationKeyVector* m_lpScaleKeys;

	DWORD m_numRotationKeys;
	JCAnimationKeyQuaternion* m_lpRotationKeys;

	DWORD m_numMatrixKey;
	JCAnimationKeyMatrix* m_lpMatrixKeys;
};

#endif