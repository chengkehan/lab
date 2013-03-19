#ifndef __PARTICLE_SYS_1_H__
#define __PARTICLE_SYS_1_H__

#include "ParticleSystem.h"

class ParticleSys1 : public ParticleSystem
{
public:
	ParticleSys1(IDirect3DDevice9* lpd3dd, IDirect3DTexture9* lpTexture);
	virtual ~ParticleSys1();

	virtual void update(float timeDelta);
	virtual void resetParticle(ParticlePointArrt* lpParticleAttr);

private:
	ParticleSys1();
	ParticleSys1(const ParticleSys1& value);
};

#endif