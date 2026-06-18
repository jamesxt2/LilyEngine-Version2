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
	CParticleSystem(const CParticleSystem& other);
	~CParticleSystem();
	CLONE(CParticleSystem)

	virtual void FinalTick() override;
	virtual void Render() override;

private:
	void CalculateSpawnCount();

private:
	CStructuredBuffer*		m_ParticleBuffer;
	CStructuredBuffer*		m_SpawnCountBuffer;
	CStructuredBuffer*		m_ModuleBuffer;

	Ptr<CTexture>			m_ParticleTex;

	Ptr<CParticleTickCS>	m_TickCS;

	float					m_Time;
	float					m_BurstTime;

	UINT					m_MaxParticle;

	TParticleModule			m_Module;

public:
	inline void SetParticleTexture(const Ptr<CTexture>& tex) { m_ParticleTex = tex; }
};

