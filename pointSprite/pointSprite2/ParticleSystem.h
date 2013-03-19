#ifndef __PARTICLE_SYSTEM_H__
#define __PARTICLE_SYSTEM_H__

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>

inline DWORD float_to_dword(float f)
{
	return *((DWORD*)&f);
}

struct ParticlePoint
{
	D3DXVECTOR3 position;
	D3DCOLOR diffuse;

	static const DWORD FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
};

struct ParticlePointArrt
{
	ParticlePointArrt()
	{
		diffuse = 0xFFFFFFFF;
		ZeroMemory(&position, sizeof(D3DXVECTOR3));
		ZeroMemory(&velocity, sizeof(D3DXVECTOR3));
		ZeroMemory(&acceleration, sizeof(D3DXVECTOR3));
		lifeTotal = 0.0f;
		lifeCurrent = 0.0f;
		alive = true;
	}

	D3DCOLOR diffuse;
	D3DXVECTOR3 position;
	D3DXVECTOR3 velocity;
	D3DXVECTOR3 acceleration;
	float lifeTotal;
	float lifeCurrent;
	bool alive;
};

struct Box
{
	D3DXVECTOR3 minP, maxP;

	Box(D3DXVECTOR3& minP, D3DXVECTOR3& maxP)
	{
		this->minP = minP;
		this->maxP = maxP;
	}

	Box(const Box& value)
	{
		minP = value.minP;
		maxP = value.maxP;
	}

	Box()
	{
		ZeroMemory(&minP, sizeof(D3DXVECTOR3));
		ZeroMemory(&maxP, sizeof(D3DXVECTOR3));
	}

	bool contains(const D3DXVECTOR3* lpPoint)
	{
		if(lpPoint == NULL)
		{
			return false;
		}
		else
		{
			return lpPoint->x >= minP.x && lpPoint->y >= minP.y && lpPoint->z >= minP.z && lpPoint->x <= maxP.x && lpPoint->y <= maxP.y && lpPoint->z <= maxP.z;
		}
	}
};

class ParticleSystem
{
public:
	ParticleSystem();
	virtual ~ParticleSystem();

	virtual void update(float timeDelta) = 0;
	virtual void resetParticle(ParticlePointArrt* lpParticleAttr) = 0;
	void render();

protected:
	IDirect3DDevice9* m_lpd3dd;
	D3DXVECTOR3 m_origin;
	Box m_bounds;
	float m_particleSize;
	IDirect3DTexture9* m_lpTexture;
	IDirect3DVertexBuffer9* m_lpVB;
	ParticlePointArrt* m_lpParticles;
	UINT m_numParticles;
	UINT m_batchSize;
	UINT m_batchIndex;

	void init(IDirect3DDevice9* lpd3dd, D3DXVECTOR3& origin, Box& bounds, float particleSize, IDirect3DTexture9* lpTexture, UINT numParticles, UINT batchSize);

private:
	ParticleSystem(const ParticleSystem& value);

	void preRender();
	void postRender();
};

#endif