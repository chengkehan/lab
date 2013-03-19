#include "ParticleSystem.h"
#include <cassert>

ParticleSystem::ParticleSystem()
{
	
}

ParticleSystem::~ParticleSystem()
{
	if(m_lpVB != NULL)
	{
		m_lpVB->Release();
	}
	m_lpd3dd = NULL;
	m_lpTexture = NULL;
	m_lpVB = NULL;
	m_lpParticles = NULL;
	delete []m_lpParticles;
}

void ParticleSystem::init(IDirect3DDevice9* lpd3dd, D3DXVECTOR3& origin, Box& bounds, float particleSize, IDirect3DTexture9* lpTexture, UINT numParticles, UINT batchSize)
{
	assert(lpd3dd != NULL);
	assert(lpTexture != NULL);
	assert(numParticles % batchSize == 0);
	m_lpd3dd = lpd3dd;
	m_origin = origin;
	m_bounds = bounds;
	m_particleSize = particleSize;
	m_lpTexture = lpTexture;
	m_numParticles = numParticles;
	m_batchSize = batchSize;
	m_batchIndex = 0;
	m_lpVB = NULL;

	assert(SUCCEEDED(
		m_lpd3dd->CreateVertexBuffer(sizeof(ParticlePoint) * numParticles, D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS, ParticlePoint::FVF, D3DPOOL_DEFAULT, &m_lpVB, NULL)
		));

	m_lpParticles = new ParticlePointArrt[m_numParticles];
	for (UINT i = 0; i < m_numParticles; ++i)
	{
		resetParticle(&m_lpParticles[i]);
	}
}

void ParticleSystem::render()
{
	if(m_numParticles == 0)
	{
		return;
	}

	preRender();

	m_lpd3dd->SetTexture(0, m_lpTexture);
	m_lpd3dd->SetFVF(ParticlePoint::FVF);
	m_lpd3dd->SetStreamSource(0, m_lpVB, 0, sizeof(ParticlePoint));

	ParticlePoint* lpParticleVB = NULL;
	UINT numParticlesInBatch = 0;
	bool locked = false;

	for (UINT i = 0; i < m_numParticles; ++i)
	{
		if(numParticlesInBatch == 0)
		{
			DWORD lockFlag;
			if((m_batchIndex + 1) * m_batchSize >= m_numParticles)
			{
				m_batchIndex = 0;
				lockFlag = D3DLOCK_DISCARD;
			}
			else
			{
				lockFlag = D3DLOCK_NOOVERWRITE;
			}

			assert(SUCCEEDED(
				m_lpVB->Lock(m_batchIndex * m_batchSize * sizeof(ParticlePoint), m_batchSize * sizeof(ParticlePoint), (VOID**)&lpParticleVB, lockFlag)
				));
			locked = true;
		}

		ParticlePointArrt* lpParticle = &m_lpParticles[i];

		if(!lpParticle->alive)
		{
			continue;
		}

		lpParticleVB->position = lpParticle->position;
		lpParticleVB->diffuse = lpParticle->diffuse;
		++lpParticleVB;

		++numParticlesInBatch;
		if(numParticlesInBatch == m_batchSize)
		{
			assert(SUCCEEDED(m_lpVB->Unlock()));
			m_lpd3dd->DrawPrimitive(D3DPT_POINTLIST, m_batchIndex * m_batchSize, numParticlesInBatch);

			numParticlesInBatch = 0;
			++m_batchIndex;
			lpParticleVB = NULL;
			locked = false;
		}
	}

	if(locked)
	{
		m_lpVB->Unlock();
		if(numParticlesInBatch > 0)
		{
			m_lpd3dd->DrawPrimitive(D3DPT_POINTLIST, m_batchIndex * m_batchSize, numParticlesInBatch);
			++m_batchIndex;
		}
	}

	postRender();
}

void ParticleSystem::preRender()
{
	m_lpd3dd->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_lpd3dd->SetRenderState(D3DRS_LIGHTING,	 FALSE);
	m_lpd3dd->SetRenderState(D3DRS_POINTSPRITEENABLE, TRUE);
	m_lpd3dd->SetRenderState(D3DRS_POINTSCALEENABLE, TRUE);
	m_lpd3dd->SetRenderState(D3DRS_POINTSIZE, float_to_dword(m_particleSize));
	m_lpd3dd->SetRenderState(D3DRS_POINTSIZE_MIN, float_to_dword(0.0f));

	// control the size of the particle relative to distance
	m_lpd3dd->SetRenderState(D3DRS_POINTSCALE_A,	 float_to_dword(0.0f));
	m_lpd3dd->SetRenderState(D3DRS_POINTSCALE_B,	 float_to_dword(0.0f));
	m_lpd3dd->SetRenderState(D3DRS_POINTSCALE_C,	 float_to_dword(1.0f));

	// use alpha from texture
	m_lpd3dd->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_lpd3dd->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	m_lpd3dd->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_lpd3dd->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);

	m_lpd3dd->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_lpd3dd->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_ONE);
	m_lpd3dd->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
}

void ParticleSystem::postRender()
{
	m_lpd3dd->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_lpd3dd->SetRenderState(D3DRS_POINTSPRITEENABLE, FALSE);
	m_lpd3dd->SetRenderState(D3DRS_POINTSCALEENABLE,  FALSE);
	m_lpd3dd->SetRenderState(D3DRS_ALPHABLENDENABLE,  FALSE);
}