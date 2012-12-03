#include "JCDisplayObject.h"

CONST DWORD JCDisplayObject::Vertex::FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;

JCDisplayObject::JCDisplayObject(IDirect3DDevice9* lpd3dd):
m_refX(0.0f), m_refY(0.0f), m_x(0.0f), m_y(0.0f), m_scaleX(1.0f), m_scaleY(1.0f), 
m_widthOriginal(0.0f), m_heightOriginal(0.0f), m_rotation(0.0f), m_lpTexture(NULL), 
m_lpParent(NULL), m_alpha(1.0f), m_alphaEnabled(TRUE), m_isContainer(FALSE)
{
	jccommon_assertM(lpd3dd != NULL);
	jccommon_zeromem(&m_boundsGlobal, sizeof(m_boundsGlobal));
	jccommon_zeromem(m_vbData, sizeof(m_vbData));
	m_lpd3dd = lpd3dd;
	initVertexBufferData();
}

JCDisplayObject::~JCDisplayObject()
{
	m_lpParent = NULL;
	m_lpTexture = NULL;
	m_lpd3dd = NULL;
}

VOID JCDisplayObject::setX(FLOAT value)
{
	m_x = value;
}

FLOAT JCDisplayObject::getX()
{
	return m_x;
}

VOID JCDisplayObject::setY(FLOAT value)
{
	m_y = value;
}

FLOAT JCDisplayObject::getY()
{
	return m_y;
}

VOID JCDisplayObject::setRefX(FLOAT value)
{
	m_refX = value;
}

FLOAT JCDisplayObject::getRefX()
{
	return m_refX;
}

VOID JCDisplayObject::setRefY(FLOAT value)
{
	m_refY = value;
}

FLOAT JCDisplayObject::getRefY()
{
	return m_refY;
}

VOID JCDisplayObject::setWidth(FLOAT value)
{
	m_scaleX = value / m_widthOriginal;
}

FLOAT JCDisplayObject::getWidth()
{
	return m_widthOriginal * m_scaleX;
}

FLOAT JCDisplayObject::getWidthOriginal()
{
	return m_widthOriginal;
}

VOID JCDisplayObject::setHeight(FLOAT value)
{
	m_scaleY = value / m_heightOriginal;
}

FLOAT JCDisplayObject::getHeight()
{
	return m_heightOriginal * m_scaleY;
}

FLOAT JCDisplayObject::getHeightOriginal()
{
	return m_heightOriginal;
}

CONST JCRect* JCDisplayObject::getBoundsGlobal()
{
	m_boundsGlobal.left = min(min(min(m_vbData[0].x, m_vbData[1].x), m_vbData[2].x), m_vbData[3].x);
	m_boundsGlobal.top = min(min(min(m_vbData[0].y, m_vbData[1].y), m_vbData[2].y), m_vbData[3].y);
	m_boundsGlobal.right = max(max(max(m_vbData[0].x, m_vbData[1].x), m_vbData[2].x), m_vbData[3].x);
	m_boundsGlobal.bottom = max(max(max(m_vbData[0].y, m_vbData[1].y), m_vbData[2].y), m_vbData[3].y);
	return &m_boundsGlobal;
}

VOID JCDisplayObject::setScaleX(FLOAT value)
{
	m_scaleX = value;
}

FLOAT JCDisplayObject::getScaleX()
{
	return m_scaleX;
}

VOID JCDisplayObject::setScaleY(FLOAT value)
{
	m_scaleY = value;
}

FLOAT JCDisplayObject::getScaleY()
{
	return m_scaleY;
}

VOID JCDisplayObject::setRotation(FLOAT value)
{
	m_rotation = value;
}

FLOAT JCDisplayObject::getRotation()
{
	return m_rotation;
}

VOID JCDisplayObject::setTexture(JCTexture* texture)
{
	m_lpTexture = texture;
	if(m_lpTexture != NULL)
	{
		m_widthOriginal = (FLOAT)texture->getInfo()->Width;
		m_heightOriginal = (FLOAT)texture->getInfo()->Height;
		updateVertexBufferData();
	}
	else
	{
		m_widthOriginal = 0.0f;
		m_heightOriginal = 0.0f;
		jccommon_zeromem(&m_boundsGlobal, sizeof(m_boundsGlobal));
		jccommon_zeromem(m_vbData, sizeof(m_vbData));
	}
}

JCTexture* JCDisplayObject::getTexture()
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

VOID JCDisplayObject::setAlpha(FLOAT value)
{
	m_alpha = min(max(value, 0.0f), 1.0f);
}

FLOAT JCDisplayObject::getAlpha()
{
	return m_alpha;
}

VOID JCDisplayObject::setAlphaEnabled(BOOL value)
{
	m_alphaEnabled = value;
}

BOOL JCDisplayObject::getAlphaEnabled()
{
	return m_alphaEnabled;
}

BOOL JCDisplayObject::isContainer()
{
	return m_isContainer;
}

VOID JCDisplayObject::updateVertexBufferData()
{
	// x1=cos(angle)*x-sin(angle)*y;
	// y1=cos(angle)*y+sin(angle)*x;

	FLOAT global_x = 0.0f;
	FLOAT global_y = 0.0f;
	FLOAT global_scaleX = 1.0f;
	FLOAT global_scaleY = 1.0f;
	FLOAT global_Rotation = 0.0f;
	FLOAT global_alpha = 1.0f;
	JCDisplayObject* target = (JCDisplayObject*)getParent();
	while(target != NULL)
	{
		global_x += target->getX() + target->getRefX();
		global_y += target->getY() + target->getRefY();
		global_scaleX *= target->getScaleX();
		global_scaleY *= target->getScaleY();
		global_Rotation += target->getRotation();
		global_alpha *= target->getAlpha();
		target = (JCDisplayObject*)target->getParent();
	}

	FLOAT x = global_x + m_x;
	FLOAT y = global_y + m_y;
	FLOAT width = m_widthOriginal * m_scaleX * global_scaleX;
	FLOAT height = m_heightOriginal * m_scaleY * global_scaleY;
	FLOAT rotation = m_rotation + global_Rotation;
	FLOAT refX = m_refX * m_scaleX * global_scaleX;
	FLOAT refY = m_refY * m_scaleY * global_scaleY;

	m_vbData[0].x = x + cosf(rotation) * (-refX) - sinf(rotation) * (height - refY);
	m_vbData[0].y = y + cosf(rotation) * (height - refY) + sinf(rotation) * (-refX);

	m_vbData[1].x = x + cosf(rotation) * (-refX) - sinf(rotation) * (-refY);
	m_vbData[1].y = y + cosf(rotation) * (-refY) + sinf(rotation) * (-refX);

	m_vbData[2].x = x + cosf(rotation) * (width - refX) - sinf(rotation) * (height - refY);
	m_vbData[2].y = y + cosf(rotation) * (height - refY) + sinf(rotation) * (width - refX);

	m_vbData[3].x = x + cosf(rotation) * (width - refX) - sinf(rotation) * (-refY);
	m_vbData[3].y = y + cosf(rotation) * (-refY) + sinf(rotation) * (width - refX);

	m_vbData[0].diffuse = (((INT)(global_alpha * m_alpha * 255.0f) & 0xFF) << 24) + (m_vbData[0].diffuse & 0xFFFFFF);
	m_vbData[1].diffuse = m_vbData[0].diffuse;
	m_vbData[2].diffuse = m_vbData[0].diffuse;
	m_vbData[3].diffuse = m_vbData[0].diffuse;
}

VOID JCDisplayObject::initVertexBufferData()
{
	m_vbData[0] = Vertex(0.0f, 0.0f, 0xFF000000, 0.0f, 1.0f);
	m_vbData[1] = Vertex(0.0f, 0.0f, 0xFF000000, 0.0f, 0.0f);
	m_vbData[2] = Vertex(0.0f, 0.0f, 0xFF000000, 1.0f, 1.0f);
	m_vbData[3] = Vertex(0.0f, 0.0f, 0xFF000000, 1.0f, 0.0f);
}