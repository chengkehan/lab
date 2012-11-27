#include "JCTextureManager.h"

using namespace std;

JCTextureManager::JCTextureManager()
{

}

JCTextureManager::~JCTextureManager()
{
	for (TEXTUREMAP::iterator iter = m_textureMap.begin(); iter != m_textureMap.end(); ++iter)
	{
		IDirect3DTexture9* lpTexture = iter->second;
		jccommon_releaseComM(lpTexture);
	}
}

IDirect3DTexture9* JCTextureManager::addTexture(INT id, IDirect3DTexture9* lpTexture)
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
		m_textureMap[id] = lpTexture;
		return lpTexture;
	}
}

BOOL JCTextureManager::removeTexture(INT id)
{
	for (TEXTUREMAP::iterator iter = m_textureMap.begin(); iter != m_textureMap.end(); ++iter)
	{
		if(iter->first == id)
		{
			IDirect3DTexture9* lpTexture = iter->second;
			m_textureMap.erase(iter);
			jccommon_releaseComM(lpTexture);
			return TRUE;
		}
	}
	return FALSE;
}

IDirect3DTexture9* JCTextureManager::getTexture(INT id)
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

IDirect3DTexture9* JCTextureManager::loadFileTexture(INT id, LPCWSTR path, IDirect3DDevice9* lpd3dd)
{
	if(containsTexture(id))
	{
		return getTexture(id);
	}
	else
	{
		IDirect3DTexture9* lpTexture = NULL;
		if(FAILED(D3DXCreateTextureFromFile(lpd3dd, path, &lpTexture)))
		{
			return NULL;
		}
		else
		{
			m_textureMap[id] = lpTexture;
			return lpTexture;
		}
	}
}

BOOL JCTextureManager::containsTexture(INT id)
{
	return m_textureMap.find(id) != m_textureMap.end();
}