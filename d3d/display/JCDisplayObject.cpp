#include "JCDisplayObject.h"

CONST DWORD JCDisplayObject::Vertex::FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;
CONST UINT JCDisplayObject::VB_SIZE = 4 * sizeof(JCDisplayObject::Vertex);

JCDisplayObject::JCDisplayObject(IDirect3DDevice9* lpd3dd):m_refX(0.0f), m_refY(0.0f), m_x(0.0f), m_y(0.0f), m_width(0.0f), m_height(0.0f), m_widthOriginal(0.0f), m_heightOriginal(0.0f), m_rotation(0.0f), m_lpTexture(NULL), m_lpParent(NULL), m_lpVB(NULL), m_alpha(1.0f), m_alphaEnabled(TRUE)
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

VOID JCDisplayObject::setX(FLOAT value, BOOL updateImmediate)
{
	m_x = value;
	if(m_lpTexture != NULL && updateImmediate)
	{
		lockVertexBuffer();
		updateVertexBufferXYWH();
		unlockVertexBuffer();
	}
}

FLOAT JCDisplayObject::getX()
{
	return m_x;
}

VOID JCDisplayObject::setY(FLOAT value, BOOL updateImmediate)
{
	m_y = value;
	if(m_lpTexture != NULL && updateImmediate)
	{
		lockVertexBuffer();
		updateVertexBufferXYWH();
		unlockVertexBuffer();
	}
}

FLOAT JCDisplayObject::getY()
{
	return m_y;
}

VOID JCDisplayObject::setXY(FLOAT x, FLOAT y, BOOL updateImmediate)
{
	m_x = x;
	m_y = y;
	if(m_lpTexture != NULL && updateImmediate)
	{
		lockVertexBuffer();
		updateVertexBufferXYWH();
		unlockVertexBuffer();
	}
}

VOID JCDisplayObject::setRefX(FLOAT value, BOOL updateImmediate)
{
	m_refX = value;
	if(m_lpTexture != NULL && updateImmediate)
	{
		lockVertexBuffer();
		updateVertexBufferXYWH();
		unlockVertexBuffer();
	}
}

FLOAT JCDisplayObject::getRefX()
{
	return m_refX;
}

VOID JCDisplayObject::setRefY(FLOAT value, BOOL updateImmediate)
{
	m_refY = value;
	if(m_lpTexture != NULL && updateImmediate)
	{
		lockVertexBuffer();
		updateVertexBufferXYWH();
		unlockVertexBuffer();
	}
}

FLOAT JCDisplayObject::getRefY()
{
	return m_refY;
}

VOID JCDisplayObject::setRefXY(FLOAT x, FLOAT y, BOOL updateImmediate)
{
	m_refX = x;
	m_refY = y;
	if(m_lpTexture != NULL && updateImmediate)
	{
		lockVertexBuffer();
		updateVertexBufferXYWH();
		unlockVertexBuffer();
	}
}

VOID JCDisplayObject::setWidth(FLOAT value, BOOL updateImmediate)
{
	m_width = value;
	if(m_lpTexture != NULL && updateImmediate)
	{
		lockVertexBuffer();
		updateVertexBufferXYWH();
		unlockVertexBuffer();
	}
}

FLOAT JCDisplayObject::getWidth()
{
	return m_width;
}

FLOAT JCDisplayObject::getWidthOriginal()
{
	return m_widthOriginal;
}

VOID JCDisplayObject::setHeight(FLOAT value, BOOL updateImmediate)
{
	m_height = value;
	if(m_lpTexture != NULL && updateImmediate)
	{
		lockVertexBuffer();
		updateVertexBufferXYWH();
		unlockVertexBuffer();
	}
}

FLOAT JCDisplayObject::getHeight()
{
	return m_height;
}

FLOAT JCDisplayObject::getHeightOriginal()
{
	return m_heightOriginal;
}

VOID JCDisplayObject::setWidthHeight(FLOAT width, FLOAT height, BOOL updateImmediate)
{
	m_width = width;
	m_height = height;
	if(m_lpTexture != NULL && updateImmediate)
	{
		lockVertexBuffer();
		updateVertexBufferXYWH();
		unlockVertexBuffer();
	}	
}

VOID JCDisplayObject::setRotation(FLOAT value, BOOL updateImmediate)
{
	m_rotation = value;
	if(m_lpTexture != NULL && updateImmediate)
	{
		lockVertexBuffer();
		updateVertexBufferXYWH();
		unlockVertexBuffer();
	}
}

FLOAT JCDisplayObject::getRotation()
{
	return m_rotation;
}

VOID JCDisplayObject::setTexture(IDirect3DTexture9* texture, BOOL updateImmediate)
{
	m_lpTexture = texture;
	initVertexBuffer();
	if(m_lpTexture != NULL)
	{
		D3DSURFACE_DESC dest;
		m_lpTexture->GetLevelDesc(0, &dest);
		m_widthOriginal = (FLOAT)dest.Width;
		m_heightOriginal = (FLOAT)dest.Height;
		setWidthHeight((FLOAT)dest.Width, (FLOAT)dest.Height, updateImmediate);
	}
	else
	{
		m_widthOriginal = 0.0f;
		m_heightOriginal = 0.0f;
		setWidthHeight(0.0f, 0.0f, updateImmediate);
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

VOID JCDisplayObject::setAlpha(FLOAT value, BOOL updateImmediate)
{
	m_alpha = min(max(value, 0.0f), 1.0f);
	if(m_lpTexture != NULL && updateImmediate)
	{
		lockVertexBuffer();
		updateVertexBufferAlpha();
		unlockVertexBuffer();
	}
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

VOID JCDisplayObject::renderUpdate(BOOL alpha, BOOL xywh)
{
	lockVertexBuffer();
	if(alpha)
	{
		updateVertexBufferAlpha();
	}
	if(xywh)
	{
		updateVertexBufferXYWH();
	}
	unlockVertexBuffer();
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

	m_lpVBData[0].x = m_x + cosf(m_rotation) * (-m_refX) - sinf(m_rotation) * (m_height - m_refY);
	m_lpVBData[0].y = m_y + cosf(m_rotation) * (m_height - m_refY) + sinf(m_rotation) * (-m_refX);

	m_lpVBData[1].x = m_x + cosf(m_rotation) * (-m_refX) - sinf(m_rotation) * (-m_refY);
	m_lpVBData[1].y = m_y + cosf(m_rotation) * (-m_refY) + sinf(m_rotation) * (-m_refX);

	m_lpVBData[2].x = m_x + cosf(m_rotation) * (m_width - m_refX) - sinf(m_rotation) * (m_height - m_refY);
	m_lpVBData[2].y = m_y + cosf(m_rotation) * (m_height - m_refY) + sinf(m_rotation) * (m_width - m_refX);

	m_lpVBData[3].x = m_x + cosf(m_rotation) * (m_width - m_refX) - sinf(m_rotation) * (-m_refY);
	m_lpVBData[3].y = m_y + cosf(m_rotation) * (-m_refY) + sinf(m_rotation) * (m_width - m_refX);
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