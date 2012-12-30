#ifndef __JC_ANIMATIONSET_H__
#define __JC_ANIMATIONSET_H__

#include <Windows.h>

#include "JCAnimation.h"
#include "jccommon.h"

class JCAnimationSet
{
public:
	JCAnimationSet();
	~JCAnimationSet();

	CHAR* m_lpName;
	DWORD m_timeTotal;
	JCAnimationSet* m_lpNextAnimationSet;
	DWORD m_numAnimations;
	JCAnimation* m_lpAnimations;
};

#endif