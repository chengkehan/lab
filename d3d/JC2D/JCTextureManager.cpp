#include "JCTextureManager.h"

using namespace std;

JCTextureManager::JCTextureManager(IDirect3DDevice9* lpd3dd)
{
	jccommon_assertM(lpd3dd != NULL);
	m_lpd3dd = lpd3dd;
}

JCTextureManager::~JCTextureManager()
{

}

IDirect3DTexture9* JCTextureManager::addTexture(INT id, IDirect3DTexture9* lpTexture)
{
	if(lpTexture == NULL)
	{
		return NULL;
	}

	if(containsTexture(id))
	{
		return NULL;
	}
	else
	{
		m_textureMap[id] = lpTexture;
		return lpTexture;
	}
}

IDirect3DTexture9* JCTextureManager::removeTexture(INT id)
{
	for (map<INT, IDirect3DTexture9*>::iterator iter = m_textureMap.begin(); iter != m_textureMap.end(); ++iter)
	{
		if(iter->first == id)
		{
			IDirect3DTexture9* lpTexture = iter->second;
			m_textureMap.erase(iter);
			return lpTexture;
		}
	}
	return NULL;
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

IDirect3DTexture9* JCTextureManager::loadFileTexture(INT id, LPCWSTR path)
{
	if(containsTexture(id))
	{
		return NULL;
	}
	else
	{
		IDirect3DTexture9* lpTexture = NULL;
		if(FAILED(D3DXCreateTextureFromFile(m_lpd3dd, path, &lpTexture)))
		{
			return NULL;
		}
		else
		{
			return lpTexture;
		}
	}
}

BOOL JCTextureManager::containsTexture(INT id)
{
	return m_textureMap.find(id) != m_textureMap.end();
}