#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include <list>
#include "d3dUtility.h"
#include "camera.h"

using namespace std;

struct sParticle
{
	D3DXVECTOR3 position;
	D3DCOLOR	color;
};

const DWORD PARTICLE_FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;

/////////////////////////////////////////////////////////////////////////////////////////////////

struct sParticleAttribute
{
	sParticleAttribute()
	{
		life_time = 0.0f;
		age		  = 0.0f;
		is_alive  = true;
	}

	D3DXVECTOR3 position;
	D3DXVECTOR3 velocity;
	D3DXVECTOR3 acceleration;
	float		life_time;		// how long the particle lives for before dying
	float		age;			// current age of the particle
	D3DXCOLOR	color;			// current color of the particle
	D3DXCOLOR	color_fade;		// how the color fades with respect to time
	bool		is_alive;
};

/////////////////////////////////////////////////////////////////////////////////////////////////

class cParticleSystem
{
protected:
	IDirect3DDevice9*			m_device;
	D3DXVECTOR3					m_origin;
	cBoundingBox				m_bounding_box;
	float						m_emit_rate;		// rate new particles are added to system
	float						m_size;				// size of particles
	IDirect3DTexture9*			m_texture;
	IDirect3DVertexBuffer9* 	m_vertex_buffer;
	list<sParticleAttribute>	m_particles;
	int							m_max_particles;	// max allowed particles system can have

	// following three data elements used for rendering the particle system efficiently

	DWORD	m_vb_num;		// particle number in vertex buffer
	DWORD	m_vb_offset;	// offset in vertex buffer to lock
	DWORD	m_vb_batch_num;	// number of vertices to lock starting at m_vb_offset

public:
	cParticleSystem();
	virtual ~cParticleSystem();

	virtual bool init(IDirect3DDevice9*	device, const char* texture_filename);

	// sometimes we don't want to free the memory of a dead particle, but rather respawn it instead.
	virtual void reset_particle(sParticleAttribute* particl_attr) = 0;
	virtual void add_particle();

	virtual void update(float time_delta) = 0;

	virtual void pre_render();
	virtual void render();
	virtual void post_render();

	bool is_empty();
	bool is_dead();

protected:
	virtual void remove_dead_particles();
};

/////////////////////////////////////////////////////////////////////////////////////////////////

class cSnow : public cParticleSystem
{
public:
	cSnow(cBoundingBox* bounding_box, int num_particles);
	virtual void reset_particle(sParticleAttribute* attr);
	virtual void update(float time_delta);
};

#endif
