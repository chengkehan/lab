#include "PatricleSys1.h"
#include "jccommon.h"

ParticleSys1::ParticleSys1(IDirect3DDevice9* lpd3dd, IDirect3DTexture9* lpTexture)
{
	init(lpd3dd, D3DXVECTOR3(0.0f, 0.0f, 0.0f), Box(D3DXVECTOR3(-2.0f, -2.0f, -2.0f), D3DXVECTOR3(2.0f, 2.0f, 2.0f)), 0.15f, lpTexture, 500, 100);
}

ParticleSys1::~ParticleSys1()
{

}

void ParticleSys1::update(float timeDelta)
{
	for (UINT i = 0; i < m_numParticles; ++i)
	{
		ParticlePointArrt* lpPatricle = &m_lpParticles[i];
		lpPatricle->velocity += lpPatricle->acceleration;
		lpPatricle->position += lpPatricle->velocity;
		
		if(!m_bounds.contains(&lpPatricle->position))
		{
			resetParticle(lpPatricle);
		}
	}
}

void ParticleSys1::resetParticle(ParticlePointArrt* lpParticleAttr)
{
	if(lpParticleAttr == NULL)
	{
		return;
	}

	lpParticleAttr->diffuse = (UINT)((float)rand() / (float)RAND_MAX * 0xFFFFFF) + ((UINT)((float)rand() / (float)RAND_MAX * 0x88) << 24);
	lpParticleAttr->position.x = (float)rand() / (float)RAND_MAX * m_bounds.maxP.x * 2.0f - m_bounds.maxP.x;
	lpParticleAttr->position.y = m_bounds.minP.y;
	lpParticleAttr->position.z = (float)rand() / (float)RAND_MAX * m_bounds.maxP.z * 2.0f - m_bounds.maxP.z;
	lpParticleAttr->alive = true;
	lpParticleAttr->acceleration.x = 0.0f;
	lpParticleAttr->acceleration.y = (float)rand() / (float)RAND_MAX * 0.01f;
	lpParticleAttr->acceleration.z = 0.0f;
	lpParticleAttr->velocity.x = 0.0f;
	lpParticleAttr->velocity.y = 0.0f;
	lpParticleAttr->velocity.z = 0.0f;
}