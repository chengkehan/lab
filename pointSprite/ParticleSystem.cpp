#include <cstdlib>
#include "ParticleSystem.h"

cParticleSystem::cParticleSystem()
{
	m_device		= NULL;
	m_vertex_buffer = NULL;
	m_texture		= NULL;
}

cParticleSystem::~cParticleSystem()
{
	safe_release<IDirect3DVertexBuffer9*>(m_vertex_buffer);
	safe_release<IDirect3DTexture9*>(m_texture);
}

//////////////////////////////////////////////////////////////////////////////////////////////

bool cParticleSystem::init(IDirect3DDevice9* device, const char* texture_filename)
{
	// Vertex buffer's number does not equal the number of particles in our system.  
	// We use the vertex buffer to draw a portion of our particles at a time.  
	// The arbitrary number we choose for the vertex buffer is specified by the m_vb_num variable.

	m_device = device;

	HRESULT hr;

	hr = device->CreateVertexBuffer(
			m_vb_num * sizeof(sParticle),
			D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,
			PARTICLE_FVF,
			D3DPOOL_DEFAULT,	// D3DPOOL_MANAGED can't be used with D3DUSAGE_DYNAMIC
			&m_vertex_buffer,
			NULL);

	if(FAILED(hr))
	{
		MessageBox(NULL, L"CreateVertexBuffer() - FAILED", L"ParticleSystem", MB_OK);
		return false;
	}

	hr = D3DXCreateTextureFromFileA(device, texture_filename, &m_texture);

	if(FAILED(hr))
	{
		MessageBox(NULL, L"D3DXCreateTextureFromFile() - FAILED", L"ParticleSystem", MB_OK);
		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////

void cParticleSystem::add_particle()
{
	sParticleAttribute attr;
	reset_particle(&attr);

	m_particles.push_back(attr);
}

//////////////////////////////////////////////////////////////////////////////////////////////

void cParticleSystem::pre_render()
{
	m_device->SetRenderState(D3DRS_LIGHTING,		  FALSE);
	m_device->SetRenderState(D3DRS_POINTSPRITEENABLE, TRUE);
	m_device->SetRenderState(D3DRS_POINTSCALEENABLE,  TRUE);
	m_device->SetRenderState(D3DRS_POINTSIZE,		  float_to_dword(m_size));
	m_device->SetRenderState(D3DRS_POINTSIZE_MIN,	  float_to_dword(0.0f));

	// control the size of the particle relative to distance
	m_device->SetRenderState(D3DRS_POINTSCALE_A,	float_to_dword(0.0f));
	m_device->SetRenderState(D3DRS_POINTSCALE_B,	float_to_dword(0.0f));
	m_device->SetRenderState(D3DRS_POINTSCALE_C,	float_to_dword(1.0f));

	// use alpha from texture
	m_device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_device->SetTextureStageState(0, D3DTSS_ALPHAOP,	D3DTOP_SELECTARG1);

	m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_device->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
	m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void cParticleSystem::post_render()
{
	m_device->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_device->SetRenderState(D3DRS_POINTSPRITEENABLE, FALSE);
	m_device->SetRenderState(D3DRS_POINTSCALEENABLE,  FALSE);
	m_device->SetRenderState(D3DRS_ALPHABLENDENABLE,  FALSE);
}

void cParticleSystem::render()
{
	// The render method works by filling a section of the vertex buffer with data, then we render that section.  
	// While that section is rendering we lock a new section and begin to fill that section.  
	// Once that sections filled we render it. This process continues until all the particles have been drawn.  
	// The benifit of this method is that we keep the video card and the CPU busy.  

	if(m_particles.empty())
		return;

	// set render states

	pre_render();

	m_device->SetTexture(0, m_texture);
	m_device->SetFVF(PARTICLE_FVF);
	m_device->SetStreamSource(0, m_vertex_buffer, 0, sizeof(sParticle));

	sParticle* v;

	m_vertex_buffer->Lock(
		0,
		m_particles.size() * sizeof(sParticle),
		(void**)&v,
		D3DLOCK_DISCARD);

	// until all particles have been rendered
	for(list<sParticleAttribute>::iterator iter = m_particles.begin(); iter != m_particles.end(); iter++)
	{

		// copy a batch of the living particles to the next vertex buffer segment

		v->position = iter->position;
		v->color	= (D3DCOLOR) iter->color;
		v++;	// next element
	}

	m_vertex_buffer->Unlock();
	m_device->DrawPrimitive(D3DPT_POINTLIST, 0, m_particles.size());

	post_render();					// reset render states
}

//void cParticleSystem::render()
//{
//	// The render method works by filling a section of the vertex buffer with data, then we render that section.  
//	// While that section is rendering we lock a new section and begin to fill that section.  
//	// Once that sections filled we render it. This process continues until all the particles have been drawn.  
//	// The benifit of this method is that we keep the video card and the CPU busy.  
//
//	if(m_particles.empty())
//		return;
//
//	// set render states
//
//	pre_render();
//
//	m_device->SetTexture(0, m_texture);
//	m_device->SetFVF(PARTICLE_FVF);
//	m_device->SetStreamSource(0, m_vertex_buffer, 0, sizeof(sParticle));
//
//	//
//	// render batches one by one
//	//
//
//	// start at beginning if we're at the end of the vertex buffer
//	if(m_vb_offset >= m_vb_num)
//		m_vb_offset = 0;
//
//	sParticle* v;
//
//	m_vertex_buffer->Lock(
//		m_vb_offset * sizeof(sParticle),
//		m_vb_batch_num * sizeof(sParticle),
//		(void**)&v,
//		m_vb_offset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);
//
//	DWORD num_particles_in_batch = 0;
//
//	// until all particles have been rendered
//	for(list<sParticleAttribute>::iterator iter = m_particles.begin(); iter != m_particles.end(); iter++)
//	{
//		if(! iter->is_alive)
//			continue;
//
//		// copy a batch of the living particles to the next vertex buffer segment
//	
//		v->position = iter->position;
//		v->color	= (D3DCOLOR) iter->color;
//		v++;	// next element
//
//		num_particles_in_batch++;
//
//		// if this batch full?
//		if(num_particles_in_batch == m_vb_batch_num)
//		{
//			// draw the last batch of particles that was copied to the vertex buffer
//			m_vertex_buffer->Unlock();
//			m_device->DrawPrimitive(D3DPT_POINTLIST, m_vb_offset, m_vb_batch_num);
//
//			// 
//			// While that batch is drawing, start filling the next batch with particles.
//			//
//
//			// move the offset to the start of the next batch
//			m_vb_offset += m_vb_batch_num;
//
//			// Don't offset into memory that is outside the vb's range.
//			// If we're at the end, start at the beginning.
//			if(m_vb_offset >= m_vb_num)
//				m_vb_offset = 0;
//
//			m_vertex_buffer->Lock(
//				m_vb_offset * sizeof(sParticle),
//				m_vb_batch_num * sizeof(sParticle),
//				(void**)&v,
//				m_vb_offset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);
//			
//			num_particles_in_batch = 0;	// reset for new batch
//		}
//	}
//
//	m_vertex_buffer->Unlock();
//
//	// Its possible that the LAST batch being filled never got rendered because the condition 
//	// (num_particles_in_batch == m_vb_batch_num) would not have been satisfied.  
//	// We draw the last partially filled batch now.	
//	if(num_particles_in_batch)
//		m_device->DrawPrimitive(D3DPT_POINTLIST, m_vb_offset, num_particles_in_batch);
//
//	m_vb_offset += m_vb_batch_num;	// next block
//	post_render();					// reset render states
//}

//////////////////////////////////////////////////////////////////////////////////////////////

bool cParticleSystem::is_empty()
{
	return m_particles.empty();
}

bool cParticleSystem::is_dead()
{
	for(list<sParticleAttribute>::iterator iter = m_particles.begin(); iter != m_particles.end(); iter++)
	{
		// Is there at least one living particle? If yes, the system is not dead.
		if(iter->is_alive)
			return false;
	}

	// No living particles found, the system must be dead.
	return true;
}

void cParticleSystem::remove_dead_particles()
{
	list<sParticleAttribute>::iterator iter = m_particles.begin();

	while(iter != m_particles.end())
	{		
		if(! iter->is_alive)
			// erase returns the next iterator, so no need to increment to the next one ourseleves.
			iter = m_particles.erase(iter);
		else
			iter++;	// next in list
	}
}

//*****************************************************************************
// Snow System
//*****************************************************************************

cSnow::cSnow(cBoundingBox* bounding_box, int num_particles)
{
	m_bounding_box	= *bounding_box;
	m_size			= 1.0f;
	m_vb_num		= 1;
	m_vb_offset		= 0;
	m_vb_batch_num	= 1;

	for(int i = 0; i < num_particles; i++)
		add_particle();
}

void cSnow::reset_particle(sParticleAttribute* attr)
{
	attr->position.y = 0.0f;
	attr->position.x = 0.0f;
	attr->position.z = 0.0f;

	// white snow flake
	attr->color = 0xFFFFFFFF;
}

void cSnow::update(float time_delta)
{
}
