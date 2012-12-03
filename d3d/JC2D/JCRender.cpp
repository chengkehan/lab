#include "JCRender.h"

JCRender::JCRender():
m_lpd3dd(NULL), m_lpVbBuffer(NULL), m_lpVbBufferData(NULL), m_lpIbBuffer(NULL), m_lpIbBufferData(NULL), m_lpTextureCurrent(NULL), 
m_bufferDisplayObjectAmount(0), m_displayObjectCount(0), m_alphaEnbaledCurrent(FALSE), vbibLocked(FALSE)
{

}

JCRender::~JCRender()
{
	jccommon_releaseComM(m_lpVbBuffer);
	jccommon_releaseComM(m_lpIbBuffer);
}

BOOL JCRender::init(IDirect3DDevice9* lpd3dd, INT bufferDisplayObjectAmount)
{
	if(lpd3dd != NULL && bufferDisplayObjectAmount > 0 && m_lpVbBuffer == NULL && m_lpIbBuffer == NULL)
	{
		m_lpd3dd = lpd3dd;
		m_bufferDisplayObjectAmount = bufferDisplayObjectAmount;
		if(FAILED(m_lpd3dd->CreateVertexBuffer(m_bufferDisplayObjectAmount * 4 * sizeof(JCDisplayObject::Vertex), D3DUSAGE_WRITEONLY, JCDisplayObject::Vertex::FVF, D3DPOOL_MANAGED, &m_lpVbBuffer, NULL)))
		{
			return FALSE;
		}
		if(FAILED(m_lpd3dd->CreateIndexBuffer(m_bufferDisplayObjectAmount * 6 * sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_lpIbBuffer, NULL)))
		{
			return FALSE;
		}

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

VOID JCRender::beginScene()
{
	unlockVbIb();
	lockVbIb();
	m_displayObjectCount = 0;
	m_lpTextureCurrent = NULL;
	m_alphaEnbaledCurrent = FALSE;
}

BOOL JCRender::renderDisplayObject(JCDisplayObject* lpDisplayObject)
{
	if(lpDisplayObject == NULL || lpDisplayObject->getTexture() == NULL || lpDisplayObject->getTexture()->getTexture() == NULL || m_displayObjectCount + 1 > m_bufferDisplayObjectAmount)
	{
		return FALSE;
	}
	else
	{
		if(m_lpTextureCurrent != NULL && (lpDisplayObject->getTexture()->getTexture() != m_lpTextureCurrent || lpDisplayObject->getAlphaEnabled() != m_alphaEnbaledCurrent))
		{
			renderBuffer(TRUE);
		}
		m_lpTextureCurrent = lpDisplayObject->getTexture()->getTexture();
		m_alphaEnbaledCurrent = lpDisplayObject->getAlphaEnabled();
		jccommon_memcpyM(&m_lpVbBufferData[m_displayObjectCount * 4], lpDisplayObject->m_vbData, sizeof(JCDisplayObject::Vertex) * 4);
		m_lpIbBufferData[m_displayObjectCount * 6] = m_displayObjectCount * 4;
		m_lpIbBufferData[m_displayObjectCount * 6 + 1] = m_displayObjectCount * 4 + 1;
		m_lpIbBufferData[m_displayObjectCount * 6 + 2] = m_displayObjectCount * 4 + 2;
		m_lpIbBufferData[m_displayObjectCount * 6 + 3] = m_displayObjectCount * 4 + 3;
		m_lpIbBufferData[m_displayObjectCount * 6 + 4] = m_displayObjectCount * 4 + 2;
		m_lpIbBufferData[m_displayObjectCount * 6 + 5] = m_displayObjectCount * 4 + 1;
		++m_displayObjectCount;
		return TRUE;
	}
}

VOID JCRender::endScene()
{
	renderBuffer(FALSE);	
}

VOID JCRender::renderBuffer(BOOL relock)
{
	unlockVbIb();
	if(m_displayObjectCount > 0)
	{
		m_lpd3dd->SetTexture(0, m_lpTextureCurrent);
		if(m_alphaEnbaledCurrent)
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
		m_lpd3dd->SetStreamSource(0, m_lpVbBuffer, 0, sizeof(JCDisplayObject::Vertex));
		m_lpd3dd->SetIndices(m_lpIbBuffer);
		m_lpd3dd->SetFVF(JCDisplayObject::Vertex::FVF);
		m_lpd3dd->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_displayObjectCount * 4, 0, m_displayObjectCount * 2);
	}
	if(relock)
	{
		lockVbIb();
	}
	m_displayObjectCount = 0;
	m_lpTextureCurrent = NULL;
	m_alphaEnbaledCurrent = FALSE;
}

VOID JCRender::lockVbIb()
{
	if(!vbibLocked)
	{
		m_lpVbBuffer->Lock(0, m_bufferDisplayObjectAmount * 4 * sizeof(JCDisplayObject::Vertex), (VOID**)&m_lpVbBufferData, 0);
		m_lpIbBuffer->Lock(0, m_bufferDisplayObjectAmount * 6 * sizeof(WORD), (VOID**)&m_lpIbBufferData, 0);
		vbibLocked = TRUE;
	}
	
}

VOID JCRender::unlockVbIb()
{
	if(vbibLocked)
	{
		m_lpVbBuffer->Unlock();
		m_lpIbBuffer->Unlock();
		vbibLocked = FALSE;
	}
}