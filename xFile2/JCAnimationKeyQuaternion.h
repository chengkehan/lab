#ifndef __JC_ANIMATIONKEY_QUATERNION_H__
#define __JC_ANIMATIONKEY_QUATERNION_H__

#include <Windows.h>
#include <d3dx9.h>

class JCAnimationKeyQuaternion
{
public:
	DWORD m_time;
	D3DXQUATERNION m_quaternionKey;
};

#endif