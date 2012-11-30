#ifndef __JC_TEXTURE_MANAGER_H__
#define __JC_TEXTURE_MANAGER_H__

#include <Windows.h>
#include <map>
#include <d3d9.h>
#include <d3dx9.h>

#include "JCObject.h"
#include "JCTexture.h"
#include "jccommon.h"

class JCTextureManager : public JCObject
{
public:
	JCTextureManager();
	~JCTextureManager();

	JCTexture* addTexture(INT id, IDirect3DTexture9* lpTexture, D3DXIMAGE_INFO* lpInfo = NULL);
	BOOL removeTexture(INT id);
	JCTexture* getTexture(INT id);
	JCTexture* loadFileTexture(INT id, LPCWSTR path, IDirect3DDevice9* lpd3dd);
	BOOL containsTexture(INT id);

private:
	JCTextureManager(CONST JCTextureManager& manager);

	// key id, value lpTexture
	typedef std::map<INT, JCTexture*> TEXTUREMAP;

	TEXTUREMAP m_textureMap;
};

#endif