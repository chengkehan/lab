#include "JCAnimationSet.h"

JCAnimationSet::JCAnimationSet()
{
	m_lpName = NULL;
	m_timeTotal = 0;
	m_lpNextAnimationSet = NULL;
	m_numAnimations = 0;
	m_lpAnimations = NULL;
}

JCAnimationSet::~JCAnimationSet()
{
	jccommon_deleteArrayM(m_lpName); m_lpName = NULL;
	jccommon_deleteM(m_lpAnimations); m_lpAnimations = NULL;
	jccommon_deleteM(m_lpNextAnimationSet); m_lpNextAnimationSet = NULL;
}