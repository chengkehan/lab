#include "JCTexture.h"

JCTexture::JCTexture(IDirect3DTexture9* lpTexture, D3DXIMAGE_INFO* lpInfo):m_lpTexture(lpTexture), m_lpInfo(NULL)
{
	if(lpInfo != NULL)
	{
		jccommon_newM(m_lpInfo, D3DXIMAGE_INFO);
		jccommon_memcpyM(m_lpInfo, lpInfo, sizeof(D3DXIMAGE_INFO));
	}
}

JCTexture::~JCTexture()
{
	jccommon_releaseComM(m_lpTexture);
	jccommon_deleteM(m_lpInfo);
}

IDirect3DTexture9* JCTexture::getTexture()
{
	return m_lpTexture;
}

CONST D3DXIMAGE_INFO* JCTexture::getInfo()
{
	return m_lpInfo;
}