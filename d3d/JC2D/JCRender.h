#ifndef __JC_RENDER_H__
#define __JC_RENDER_H__

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "JCObject.h"
#include "JCDisplayObject.h"
#include "jccommon.h"

class JCRender : public JCObject
{
public:
	JCRender();
	~JCRender();

	BOOL init(IDirect3DDevice9* lpd3dd, INT bufferDisplayObjectAmount = 4000);
	VOID beginScene();
	BOOL renderDisplayObject(JCDisplayObject* lpDisplayObject);
	VOID endScene();

private:
	JCRender(CONST JCRender& value);

	IDirect3DDevice9* m_lpd3dd;
	IDirect3DVertexBuffer9* m_lpVbBuffer;
	JCDisplayObject::Vertex* m_lpVbBufferData;
	IDirect3DIndexBuffer9* m_lpIbBuffer;
	WORD* m_lpIbBufferData;
	INT m_bufferDisplayObjectAmount;
	INT m_displayObjectCount;
	IDirect3DTexture9* m_lpTextureCurrent;
	BOOL m_alphaEnbaledCurrent;
	BOOL vbibLocked;

	VOID renderBuffer(BOOL relock);
	VOID lockVbIb();
	VOID unlockVbIb();
};

#endif