#include "JCDisplayObject.h"

CONST DWORD JCDisplayObject::Vertex::FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;
CONST UINT JCDisplayObject::VB_SIZE = 4 * sizeof(JCDisplayObject::Vertex);

JCDisplayObject::JCDisplayObject(IDirect3DDevice9* lpd3dd):m_refX(0), m_refY(0), m_x(0), m_y(0), m_width(0), m_height(0), m_rotation(0), m_lpTexture(NULL), m_lpParent(NULL), m_lpVB(NULL), m_alpha(1.0f), m_alphaEnabled(TRUE)
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
	lockVertexBuffer();
	updateVertexBufferXYWH();
	unlockVertexBuffer();
}

FLOAT JCDisplayObject::getX()
{
	return m_x;
}

VOID JCDisplayObject::setY(FLOAT value)
{
	m_y = value;
	lockVertexBuffer();
	updateVertexBufferXYWH();
	unlockVertexBuffer();
}

FLOAT JCDisplayObject::getY()
{
	return m_y;
}

VOID JCDisplayObject::setRefX(FLOAT value)
{
	m_refX = value;
	lockVertexBuffer();
	updateVertexBufferXYWH();
	unlockVertexBuffer();
}

FLOAT JCDisplayObject::getRefX()
{
	return m_refX;
}

VOID JCDisplayObject::setRefY(FLOAT value)
{
	m_refY = value;
	lockVertexBuffer();
	updateVertexBufferXYWH();
	unlockVertexBuffer();
}

FLOAT JCDisplayObject::getRefY()
{
	return m_refY;
}

VOID JCDisplayObject::setWidth(FLOAT value)
{
	m_width = value;
	lockVertexBuffer();
	updateVertexBufferXYWH();
	unlockVertexBuffer();
}

FLOAT JCDisplayObject::getWidth()
{
	return m_width;
}

VOID JCDisplayObject::setHeight(FLOAT value)
{
	m_height = value;
	lockVertexBuffer();
	updateVertexBufferXYWH();
	unlockVertexBuffer();
}

FLOAT JCDisplayObject::getHeight()
{
	return m_height;
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
		setWidthHeight((FLOAT)dest.Width, (FLOAT)dest.Height);
	}
	else
	{
		setWidthHeight(0.0f, 0.0f);
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
	lockVertexBuffer();
	updateVertexBufferAlpha();
	unlockVertexBuffer();
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

VOID JCDisplayObject::setWidthHeight(FLOAT width, FLOAT height)
{
	m_width = width;
	m_height = height;
	lockVertexBuffer();
	updateVertexBufferXYWH();
	unlockVertexBuffer();
}

VOID JCDisplayObject::setXY(FLOAT x, FLOAT y)
{
	m_x = x;
	m_y = y;
	lockVertexBuffer();
	updateVertexBufferXYWH();
	unlockVertexBuffer();
}

VOID JCDisplayObject::render()
{
	if(m_lpTexture != NULL)
	{
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
	m_lpVBData[0].x = m_x - m_refX;
	m_lpVBData[0].y = m_y - m_refY + m_height;
	m_lpVBData[1].x = m_x - m_refX;
	m_lpVBData[1].y = m_y - m_refY;
	m_lpVBData[2].x = m_x - m_refX + m_width;
	m_lpVBData[2].y = m_y - m_refY + m_height;
	m_lpVBData[3].x = m_x - m_refX + m_width;
	m_lpVBData[3].y = m_y - m_refY;
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