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
	JCDisplayObject();
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

	VOID setWidth(INT value);
	INT getWidth();

	VOID setHeight(INT value);
	INT getHeight();

	VOID setScaleX(FLOAT value);
	FLOAT getScaleX();

	VOID setScaleY(FLOAT value);
	FLOAT getScaleY();

	VOID setRotation(FLOAT value);
	FLOAT getRotation();

	VOID setTexture(IDirect3DTexture9* texture);
	IDirect3DTexture9* getTexture();

	VOID setParent(JCDisplayObjectContainer* parent);
	JCDisplayObjectContainer* getParent();

private:
	D3DXVECTOR3 m_refAxis;
	INT m_width;
	INT m_height;
	FLOAT m_rotation;
	D3DXMATRIX m_transformCombine;
	IDirect3DTexture9* m_lpTexture;
	JCDisplayObjectContainer* m_lpParent;
};

#endif