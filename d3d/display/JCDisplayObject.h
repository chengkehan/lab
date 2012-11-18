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
	VOID setX(FLOAT value);
	FLOAT getX();

	VOID setY(FLOAT value);
	FLOAT getY();

	VOID setRefX(FLOAT value);
	FLOAT getRefX();

	VOID setRefY(FLOAT value);
	FLOAT getRefY();

	VOID setWidth(FLOAT value);
	FLOAT getWidth();

	VOID setHeight(FLOAT value);
	FLOAT getHeight();

	VOID setRotation(FLOAT value);
	FLOAT getRotation();

	VOID setTexture(IDirect3DTexture9* texture);
	IDirect3DTexture9* getTexture();

	VOID setParent(JCDisplayObjectContainer* parent);
	JCDisplayObjectContainer* getParent();

	VOID setAlpha(FLOAT value);
	FLOAT getAlpha();

	VOID setAlphaEnabled(BOOL value);
	BOOL getAlphaEnabled();

	VOID setWidthHeight(FLOAT width, FLOAT height);
	VOID setXY(FLOAT x, FLOAT y);

	VOID render();

private:
	JCDisplayObject();

private:
	FLOAT m_refX;
	FLOAT m_refY;
	FLOAT m_x;
	FLOAT m_y;
	FLOAT m_width;
	FLOAT m_height;
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