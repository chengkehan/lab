#include "JCTextureManager.h"

using namespace std;

JCTextureManager::JCTextureManager()
{

}

JCTextureManager::~JCTextureManager()
{
	for (TEXTUREMAP::iterator iter = m_textureMap.begin(); iter != m_textureMap.end(); ++iter)
	{
		JCTexture* data = iter->second;
		jccommon_deleteM(data);
	}
}

JCTexture* JCTextureManager::addTexture(INT id, IDirect3DTexture9* lpTexture, D3DXIMAGE_INFO* lpInfo)
{
	if(lpTexture == NULL)
	{
		return NULL;
	}

	if(containsTexture(id))
	{
		return getTexture(id);
	}
	else
	{
		JCTexture* data = new JCTexture(lpTexture, lpInfo);
		jccommon_assertM(data != NULL);
		m_textureMap[id] = data;
		return data;
	}
}

BOOL JCTextureManager::removeTexture(INT id)
{
	for (TEXTUREMAP::iterator iter = m_textureMap.begin(); iter != m_textureMap.end(); ++iter)
	{
		if(iter->first == id)
		{
			JCTexture* data = iter->second;
			m_textureMap.erase(iter);
			jccommon_deleteM(data);
			return TRUE;
		}
	}
	return FALSE;
}

JCTexture* JCTextureManager::getTexture(INT id)
{
	if(containsTexture(id))
	{
		return m_textureMap[id];
	}
	else
	{
		return NULL;
	}
}

JCTexture* JCTextureManager::loadFileTexture(INT id, LPCWSTR path, IDirect3DDevice9* lpd3dd)
{
	if(containsTexture(id))
	{
		return getTexture(id);
	}
	else
	{
		IDirect3DTexture9* lpTexture = NULL;
		D3DXIMAGE_INFO imgInfo;
		if(FAILED(D3DXCreateTextureFromFileEx(
				lpd3dd, path, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, &imgInfo, NULL, &lpTexture
		)))
		{
			return NULL;
		}
		else
		{
			JCTexture* data = new JCTexture(lpTexture, &imgInfo);
			jccommon_assertM(data != NULL);
			m_textureMap[id] = data;
			return data;
		}
	}
}

BOOL JCTextureManager::containsTexture(INT id)
{
	return m_textureMap.find(id) != m_textureMap.end();
}