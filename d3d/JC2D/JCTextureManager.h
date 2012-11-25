#ifndef __JC_TEXTURE_MANAGER_H__
#define __JC_TEXTURE_MANAGER_H__

#include <Windows.h>
#include <map>
#include <d3d9.h>
#include <d3dx9.h>

#include "jccommon.h"

class JCTextureManager
{
public:
	JCTextureManager(IDirect3DDevice9* lpd3dd);
	~JCTextureManager();

public:
	IDirect3DTexture9* addTexture(INT id, IDirect3DTexture9* lpTexture);
	IDirect3DTexture9* removeTexture(INT id);
	IDirect3DTexture9* getTexture(INT id);
	IDirect3DTexture9* loadFileTexture(INT id, LPCWSTR path);
	BOOL containsTexture(INT id);

private:
	JCTextureManager();
	JCTextureManager(CONST JCTextureManager& manager);

private:
	// key id, value lpTexture
	std::map<INT, IDirect3DTexture9*> m_textureMap;
	IDirect3DDevice9* m_lpd3dd;
};

#endif