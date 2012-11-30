#include "JCDisplayObject.h"

CONST DWORD JCDisplayObject::Vertex::FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;
CONST UINT JCDisplayObject::VB_SIZE = 4 * sizeof(JCDisplayObject::Vertex);

JCDisplayObject::JCDisplayObject(IDirect3DDevice9* lpd3dd):
m_refX(0.0f), m_refY(0.0f), m_x(0.0f), m_y(0.0f), m_scaleX(1.0f), m_scaleY(1.0f), 
m_widthOriginal(0.0f), m_heightOriginal(0.0f), m_rotation(0.0f), m_lpTexture(NULL), 
m_lpParent(NULL), m_lpVB(NULL), m_alpha(1.0f), m_alphaEnabled(TRUE), m_isContainer(FALSE), 
m_widthReal(0.0f), m_heightReal(0.0f)
{
	jccommon_assertM(lpd3dd != NULL);
	m_lpd3dd = lpd3dd;
}

JCDisplayObject::~JCDisplayObject()
{
	m_lpParent = NULL;
	m_lpTexture = NULL;
	m_lpd3dd = NULL;
	releaseVertexBuffer();
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

FLOAT JCDisplayObject::getWidthReal()
{
	return m_widthReal;
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

FLOAT JCDisplayObject::getHeightReal()
{
	return m_heightReal;
}

CONST JCRect* JCDisplayObject::getBounds()
{
	return &m_bounds;
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

VOID JCDisplayObject::setTexture(IDirect3DTexture9* texture)
{
	m_lpTexture = texture;
	initVertexBuffer();
	if(m_lpTexture != NULL)
	{
		D3DSURFACE_DESC dest;
		m_lpTexture->GetLevelDesc(0, &dest);
		m_widthOriginal = (FLOAT)dest.Width;
		m_heightOriginal = (FLOAT)dest.Height;
	}
	else
	{
		m_widthOriginal = 0.0f;
		m_heightOriginal = 0.0f;
		m_widthReal = 0.0f;
		m_heightReal = 0.0f;
	}
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

VOID JCDisplayObject::render()
{
	if(m_lpTexture != NULL)
	{
		lockVertexBuffer();
		updateVertexBufferAlpha();
		updateVertexBufferXYWH();
		unlockVertexBuffer();

		m_lpd3dd->SetTexture(0, m_lpTexture);
		if(m_alphaEnabled)
		{
			m_lpd3dd->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			m_lpd3dd->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			m_lpd3dd->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			m_lpd3dd->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			m_lpd3dd->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
			m_lpd3dd->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		}
		else
		{
			m_lpd3dd->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		}
		m_lpd3dd->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_lpd3dd->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		m_lpd3dd->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_ADD);
		m_lpd3dd->SetStreamSource(0, m_lpVB, 0, sizeof(Vertex));
		m_lpd3dd->SetFVF(Vertex::FVF);
		m_lpd3dd->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

VOID JCDisplayObject::initVertexBuffer()
{
	if(m_lpVB == NULL)
	{
		jccommon_hResultVerifyM(m_lpd3dd->CreateVertexBuffer(VB_SIZE, D3DUSAGE_WRITEONLY, Vertex::FVF, D3DPOOL_MANAGED, &m_lpVB, NULL));
		lockVertexBuffer();
		Vertex vbData[] = {
			Vertex(0.0f, 0.0f, 0xFF000000, 0.0f, 1.0f), 
			Vertex(0.0f, 0.0f, 0xFF000000, 0.0f, 0.0f), 
			Vertex(0.0f, 0.0f, 0xFF000000, 1.0f, 1.0f), 
			Vertex(0.0f, 0.0f, 0xFF000000, 1.0f, 0.0f)
		};
		jccommon_memcpyM(m_lpVBData, &vbData, VB_SIZE);
		updateVertexBufferXYWH();
		updateVertexBufferAlpha();
		unlockVertexBuffer();
	}
}

VOID JCDisplayObject::releaseVertexBuffer()
{
	jccommon_releaseComM(m_lpVB);
	m_lpVBData = NULL;
}

inline VOID JCDisplayObject::lockVertexBuffer()
{
	jccommon_hResultVerifyM(m_lpVB->Lock(0, VB_SIZE, (VOID**)&m_lpVBData, 0));
}

inline VOID JCDisplayObject::updateVertexBufferXYWH()
{
	// x1=cos(angle)*x-sin(angle)*y;
	// y1=cos(angle)*y+sin(angle)*x;

	FLOAT global_x = 0.0f;
	FLOAT global_y = 0.0f;
	FLOAT global_scaleX = 1.0f;
	FLOAT global_scaleY = 1.0f;
	FLOAT global_Rotation = 0.0f;
	JCDisplayObject* target = (JCDisplayObject*)getParent();
	while(target != NULL)
	{
		global_x += target->getX() + target->getRefX();
		global_y += target->getY() + target->getRefY();
		global_scaleX *= target->getScaleX();
		global_scaleY *= target->getScaleY();
		global_Rotation += target->getRotation();
		target = (JCDisplayObject*)target->getParent();
	}

	FLOAT x = global_x + m_x;
	FLOAT y = global_y + m_y;
	FLOAT width = m_widthOriginal * m_scaleX * global_scaleX;
	FLOAT height = m_heightOriginal * m_scaleY * global_scaleY;
	FLOAT rotation = m_rotation + global_Rotation;
	FLOAT refX = m_refX * m_scaleX * global_scaleX;
	FLOAT refY = m_refY * m_scaleY * global_scaleY;

	m_lpVBData[0].x = x + cosf(rotation) * (-refX) - sinf(rotation) * (height - refY);
	m_lpVBData[0].y = y + cosf(rotation) * (height - refY) + sinf(rotation) * (-refX);

	m_lpVBData[1].x = x + cosf(rotation) * (-refX) - sinf(rotation) * (-refY);
	m_lpVBData[1].y = y + cosf(rotation) * (-refY) + sinf(rotation) * (-refX);

	m_lpVBData[2].x = x + cosf(rotation) * (width - refX) - sinf(rotation) * (height - refY);
	m_lpVBData[2].y = y + cosf(rotation) * (height - refY) + sinf(rotation) * (width - refX);

	m_lpVBData[3].x = x + cosf(rotation) * (width - refX) - sinf(rotation) * (-refY);
	m_lpVBData[3].y = y + cosf(rotation) * (-refY) + sinf(rotation) * (width - refX);

	updateRealWHAndBounds(global_x, global_y);
}

inline VOID JCDisplayObject::updateRealWHAndBounds(FLOAT parentGlobalX, FLOAT parentGlobalY)
{
	FLOAT xMin = min(min(min(min(FLT_MAX, m_lpVBData[0].x), m_lpVBData[1].x), m_lpVBData[2].x), m_lpVBData[3].x);
	FLOAT xMax = max(max(max(max(FLT_MIN, m_lpVBData[0].x), m_lpVBData[1].x), m_lpVBData[2].x), m_lpVBData[3].x);
	FLOAT yMin = min(min(min(min(FLT_MAX, m_lpVBData[0].y), m_lpVBData[1].y), m_lpVBData[2].y), m_lpVBData[3].y);
	FLOAT yMax = max(max(max(max(FLT_MIN, m_lpVBData[0].y), m_lpVBData[1].y), m_lpVBData[2].y), m_lpVBData[3].y);
	m_widthReal = xMax - xMin;
	m_heightReal = yMax - yMin;

	if(m_lpParent == NULL)
	{
		m_bounds.x = xMin;
		m_bounds.y = yMin;
	}
	else
	{
		m_bounds.x = xMin - parentGlobalX;
		m_bounds.y = yMin - parentGlobalY;
	}
	m_bounds.width = m_widthReal;
	m_bounds.height = m_heightReal;
}

inline VOID JCDisplayObject::updateVertexBufferAlpha()
{
	m_lpVBData[0].diffuse = (((INT)(m_alpha * 255.0f) & 0xFF) << 24) + (m_lpVBData[0].diffuse & 0xFFFFFF);
	m_lpVBData[1].diffuse = m_lpVBData[0].diffuse;
	m_lpVBData[2].diffuse = m_lpVBData[0].diffuse;
	m_lpVBData[3].diffuse = m_lpVBData[0].diffuse;
}

inline VOID JCDisplayObject::unlockVertexBuffer()
{
	jccommon_hResultVerifyM(m_lpVB->Unlock());
	m_lpVBData = NULL;
}