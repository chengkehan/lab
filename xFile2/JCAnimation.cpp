#include "JCAnimation.h"

JCAnimation::JCAnimation()
{
	m_lpBone = NULL;
	m_lpNextAnimation = NULL;
	m_numTransformKeys = 0;
	m_lpTransformKeys = NULL;
	m_numScaleKeys = 0;
	m_lpScaleKeys = NULL;
	m_numRotationKeys = 0;
	m_lpRotationKeys = NULL;
	m_numMatrixKey = 0;
	m_lpMatrixKeys = NULL;
	m_lpAnimationSet = NULL;
}

JCAnimation::~JCAnimation()
{
	m_lpBone = NULL;
	m_lpAnimationSet = NULL;
	jccommon_deleteArrayM(m_lpTransformKeys); m_lpTransformKeys = NULL;
	jccommon_deleteArrayM(m_lpScaleKeys); m_lpScaleKeys = NULL;
	jccommon_deleteArrayM(m_lpRotationKeys); m_lpRotationKeys = NULL;
	jccommon_deleteArrayM(m_lpMatrixKeys); m_lpMatrixKeys = NULL;
	jccommon_deleteM(m_lpNextAnimation); m_lpNextAnimation = NULL;
}