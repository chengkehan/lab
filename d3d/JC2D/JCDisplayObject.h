#ifndef __JC_DISPLAYOBJECT_H__
#define __JC_DISPLAYOBJECT_H__

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9math.h>
#include <climits>

#include "JCEventDispatcher.h"
#include "jccommon.h"
#include "JCRect.h"

class JCDisplayObjectContainer;

class JCDisplayObject : public JCEventDispatcher
{
public:
	JCDisplayObject(IDirect3DDevice9* lpd3dd);
	virtual ~JCDisplayObject();

	friend class JCDisplayObjectContainer;

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
	FLOAT getWidthReal();

	VOID setHeight(FLOAT value);
	FLOAT getHeight();
	FLOAT getHeightOriginal();
	FLOAT getHeightReal();

	CONST JCRect* getBounds();

	VOID setScaleX(FLOAT value);
	FLOAT getScaleX();

	VOID setScaleY(FLOAT value);
	FLOAT getScaleY();

	VOID setRotation(FLOAT value);
	FLOAT getRotation();

	VOID setTexture(IDirect3DTexture9* texture);
	IDirect3DTexture9* getTexture();

	JCDisplayObjectContainer* getParent();

	VOID setAlpha(FLOAT value);
	FLOAT getAlpha();

	VOID setAlphaEnabled(BOOL value);
	BOOL getAlphaEnabled();

	BOOL isContainer();

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
	BOOL m_isContainer;

	inline virtual VOID updateRealWHAndBounds(FLOAT parentGlobalX, FLOAT parentGlobalY);

private:
	JCDisplayObject();
	JCDisplayObject(CONST JCDisplayObject& value);

	FLOAT m_refX;
	FLOAT m_refY;
	FLOAT m_x;
	FLOAT m_y;
	FLOAT m_widthOriginal;
	FLOAT m_heightOriginal;
	FLOAT m_widthReal;
	FLOAT m_heightReal;
	FLOAT m_scaleX;
	FLOAT m_scaleY;
	FLOAT m_rotation;
	IDirect3DTexture9* m_lpTexture;
	JCDisplayObjectContainer* m_lpParent;
	IDirect3DVertexBuffer9* m_lpVB;
	Vertex* m_lpVBData;
	IDirect3DDevice9* m_lpd3dd;
	FLOAT m_alpha;
	BOOL m_alphaEnabled;
	JCRect m_bounds;

	static CONST UINT VB_SIZE;

	VOID initVertexBuffer();
	VOID releaseVertexBuffer();
	inline VOID lockVertexBuffer();
	inline VOID updateVertexBufferXYWH();
	inline VOID updateVertexBufferAlpha();
	inline VOID unlockVertexBuffer();
	VOID setParent(JCDisplayObjectContainer* parent);
};

#endif