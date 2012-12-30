#ifndef __JC_ANIMATIONKEY_VECTOR_H__
#define __JC_ANIMATIONKEY_VECTOR_H__

#include <Windows.h>
#include <d3dx9.h>

class JCAnimationKeyVector
{
public:
	DWORD m_time;
	D3DXVECTOR3 m_vectorKey;
};

#endif