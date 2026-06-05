#pragma once
#include "CRenderComponent.h"

#include "CParticleTickCS.h"

class CStructuredBuffer;

struct TSpawnCount
{
	int SpawnCount;
	UINT padding[3];
};

class CParticleSystem : public CRenderComponent
{
public:
	CParticleSystem();
	~CParticleSystem();

	virtual void FinalTick() override;
	virtual void Render() override;

private:
	void CalculateSpawnCount();

private:
	CStructuredBuffer* m_ParticleBuffer;
	CStructuredBuffer* m_SpawnCountBuffer;

	Ptr<CTexture> m_ParticleTex;

	Ptr<CParticleTickCS> m_TickCS;

	int m_SpawnCount; // Spawn Count per tick
	float m_Time;

	UINT m_MaxParticle;
	UINT m_SpawnRate; // spawn per second
public:
	inline void SetParticleTexture(const Ptr<CTexture>& tex) { m_ParticleTex = tex; }
};

