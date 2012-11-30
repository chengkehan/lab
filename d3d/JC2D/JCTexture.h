#ifndef __JC_TEXTURE_H__
#define __JC_TEXTURE_H__

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "JCObject.h"
#include "jccommon.h"

class JCTexture : public JCObject
{
public:
	JCTexture(IDirect3DTexture9* lpTexture, D3DXIMAGE_INFO* lpInfo = NULL);
	~JCTexture();

	IDirect3DTexture9* getTexture();
	CONST D3DXIMAGE_INFO* getInfo();

private:
	JCTexture();
	JCTexture(CONST JCTexture& value);

	IDirect3DTexture9* m_lpTexture;
	D3DXIMAGE_INFO* m_lpInfo;
};

#endif