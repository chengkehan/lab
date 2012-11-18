#ifndef __JC_DISPLAYOBJECT_H__
#define __JC_DISPLAYOBJECT_H__

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9math.h>

#include "JCEventDispatcher.h"
#include "jccommon.h"

class JCDisplayObjectContainer;

class JCDisplayObject : public JCEventDispatcher
{
public:
	struct Vertex
	{
		FLOAT x, y, z;
		FLOAT rhw;
		D3DCOLOR diffuse;
		FLOAT u, v;
		Vertex(FLOAT px, FLOAT py, D3DCOLOR pdiffuse, FLOAT pu, FLOAT pv)
		{
			x = px; y = py; z = 0.0f;
			rhw = 0.0f;
			diffuse = pdiffuse;
			u = pu; v = pv;
		}
		static CONST DWORD FVF;
	};

public:
	JCDisplayObject(IDirect3DDevice9* lpd3dd);
	virtual ~JCDisplayObject();

public:
	VOID setX(FLOAT value, BOOL updateImmediate = FALSE);
	FLOAT getX();

	VOID setY(FLOAT value, BOOL updateImmediate = FALSE);
	FLOAT getY();

	VOID setXY(FLOAT x, FLOAT y, BOOL updateImmediate = FALSE);

	VOID setRefX(FLOAT value, BOOL updateImmediate = FALSE);
	FLOAT getRefX();

	VOID setRefY(FLOAT value, BOOL updateImmediate = FALSE);
	FLOAT getRefY();

	VOID setRefXY(FLOAT x, FLOAT y, BOOL updateImmediate = FALSE);

	VOID setWidth(FLOAT value, BOOL updateImmediate = FALSE);
	FLOAT getWidth();
	FLOAT getWidthOriginal();

	VOID setHeight(FLOAT value, BOOL updateImmediate = FALSE);
	FLOAT getHeight();
	FLOAT getHeightOriginal();
	
	VOID setWidthHeight(FLOAT width, FLOAT height, BOOL updateImmediate = FALSE);

	VOID setRotation(FLOAT value, BOOL updateImmediate = FALSE);
	FLOAT getRotation();

	VOID setTexture(IDirect3DTexture9* texture, BOOL updateImmediate = FALSE);
	IDirect3DTexture9* getTexture();

	VOID setParent(JCDisplayObjectContainer* parent);
	JCDisplayObjectContainer* getParent();

	VOID setAlpha(FLOAT value, BOOL updateImmediate = FALSE);
	FLOAT getAlpha();

	VOID setAlphaEnabled(BOOL value);
	BOOL getAlphaEnabled();

	VOID renderUpdate(BOOL alpha = TRUE, BOOL xywh = TRUE);
	VOID render();

private:
	JCDisplayObject();

private:
	FLOAT m_refX;
	FLOAT m_refY;
	FLOAT m_x;
	FLOAT m_y;
	FLOAT m_width;
	FLOAT m_widthOriginal;
	FLOAT m_height;
	FLOAT m_heightOriginal;
	FLOAT m_rotation;
	IDirect3DTexture9* m_lpTexture;
	JCDisplayObjectContainer* m_lpParent;
	IDirect3DVertexBuffer9* m_lpVB;
	Vertex* m_lpVBData;
	IDirect3DDevice9* m_lpd3dd;
	FLOAT m_alpha;
	BOOL m_alphaEnabled;

private:
	static CONST UINT VB_SIZE;

private:
	VOID initVertexBuffer();
	VOID releaseVertexBuffer();
	inline VOID lockVertexBuffer();
	inline VOID updateVertexBufferXYWH();
	inline VOID updateVertexBufferAlpha();
	inline VOID unlockVertexBuffer();
};

#endif