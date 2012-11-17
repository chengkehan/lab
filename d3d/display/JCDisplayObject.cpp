#include "JCDisplayObject.h"

JCDisplayObject::JCDisplayObject():m_width(0), m_height(0), m_rotation(0), m_lpTexture(NULL), m_lpParent(NULL)
{
	D3DXMatrixIdentity(&m_transformCombine);
	jccommon_zeromem(m_refAxis, sizeof(m_refAxis));
}

JCDisplayObject::~JCDisplayObject()
{
	m_lpParent = NULL;
	m_lpTexture = NULL;
}

VOID JCDisplayObject::setX(FLOAT value)
{
	m_transformCombine._41 = value;
}

FLOAT JCDisplayObject::getX()
{
	return m_transformCombine._41;
}

VOID JCDisplayObject::setY(FLOAT value)
{
	m_transformCombine._42 = value;
}

FLOAT JCDisplayObject::getY()
{
	return m_transformCombine._42;
}

VOID JCDisplayObject::setRefX(FLOAT value)
{
	m_refAxis.x = value + m_transformCombine._41;
	setRotation(m_rotation);
}

FLOAT JCDisplayObject::getRefX()
{
	return m_refAxis.x - m_transformCombine._41;
}

VOID JCDisplayObject::setRefY(FLOAT value)
{
	m_refAxis.y = value + m_transformCombine._42;
	setRotation(m_rotation);
}

FLOAT JCDisplayObject::getRefY()
{
	return m_refAxis.y - m_transformCombine._42;
}

VOID JCDisplayObject::setWidth(INT value)
{
	setScaleX(FLOAT(value) / FLOAT(m_width));
}

INT JCDisplayObject::getWidth()
{
	return (INT)(m_width / m_transformCombine._11);
}

VOID JCDisplayObject::setHeight(INT value)
{
	setScaleY(FLOAT(value) / FLOAT(m_width));
}

INT JCDisplayObject::getHeight()
{
	return (INT)(m_height / m_transformCombine._22);
}

VOID JCDisplayObject::setScaleX(FLOAT value)
{
	m_transformCombine._11 = value;
}

FLOAT JCDisplayObject::getScaleX()
{
	return m_transformCombine._11;
}

VOID JCDisplayObject::setScaleY(FLOAT value)
{
	m_transformCombine._22 = value;
}

FLOAT JCDisplayObject::getScaleY()
{
	return m_transformCombine._22;
}

VOID JCDisplayObject::setRotation(FLOAT value)
{
	m_rotation = value;
	D3DXMatrixRotationAxis(&m_transformCombine, &m_refAxis, value);
}

FLOAT JCDisplayObject::getRotation()
{
	return m_rotation;
}

VOID JCDisplayObject::setTexture(IDirect3DTexture9* texture)
{
	m_lpTexture = texture;
}

IDirect3DTexture9* JCDisplayObject::getTexture()
{
	return m_lpTexture;
}

VOID JCDisplayObject::setParent(JCDisplayObjectContainer* parent)
{
	m_lpParent = parent;
}

JCDisplayObjectContainer* JCDisplayObject::getParent()
{
	return m_lpParent;
}