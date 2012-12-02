#ifndef __JC_DISPLAYOBJECT_H__
#define __JC_DISPLAYOBJECT_H__

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9math.h>
#include <climits>

#include "JCEventDispatcher.h"
#include "JCRect.h"
#include "JCPoint.h"
#include "JCTexture.h"
#include "jccommon.h"

class JCDisplayObjectContainer;

class JCDisplayObject : public JCEventDispatcher
{
friend class JCDisplayObjectContainer;

public:
	JCDisplayObject(IDirect3DDevice9* lpd3dd);
	virtual ~JCDisplayObject();

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
	FLOAT getWidthOriginal();

	VOID setHeight(FLOAT value);
	FLOAT getHeight();
	FLOAT getHeightOriginal();

	virtual CONST JCRect* getBoundsGlobal();

	VOID setScaleX(FLOAT value);
	FLOAT getScaleX();

	VOID setScaleY(FLOAT value);
	FLOAT getScaleY();

	VOID setRotation(FLOAT value);
	FLOAT getRotation();

	VOID setTexture(JCTexture* texture);
	JCTexture* getTexture();

	JCDisplayObjectContainer* getParent();

	VOID setAlpha(FLOAT value);
	FLOAT getAlpha();

	VOID setAlphaEnabled(BOOL value);
	BOOL getAlphaEnabled();

	virtual VOID render();

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

protected:
	JCRect m_boundsGlobal;

private:
	JCDisplayObject();
	JCDisplayObject(CONST JCDisplayObject& value);

	FLOAT m_refX;
	FLOAT m_refY;
	FLOAT m_x;
	FLOAT m_y;
	FLOAT m_widthOriginal;
	FLOAT m_heightOriginal;
	FLOAT m_scaleX;
	FLOAT m_scaleY;
	FLOAT m_rotation;
	JCTexture* m_lpTexture;
	JCDisplayObjectContainer* m_lpParent;
	IDirect3DVertexBuffer9* m_lpVB;
	Vertex* m_lpVBData;
	JCPoint m_vbXyData[4];
	IDirect3DDevice9* m_lpd3dd;
	FLOAT m_alpha;
	BOOL m_alphaEnabled;

	static CONST UINT VB_SIZE;

	VOID initVertexBuffer();
	VOID releaseVertexBuffer();
	inline VOID lockVertexBuffer();
	inline VOID updateVertexBuffer();
	inline VOID unlockVertexBuffer();
	VOID setParent(JCDisplayObjectContainer* parent);
};

#endif