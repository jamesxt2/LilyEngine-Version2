#pragma once
#include "CComputeShader.h"

class CStructuredBuffer;

class CParticleTickCS : public CComputeShader
{
public:
	CParticleTickCS();
	~CParticleTickCS();

	virtual int Bind() override;
	virtual void Clear() override;
	virtual void CalculateGroupNum() override;

private:
	CStructuredBuffer* m_ParticleBuffer;
	CStructuredBuffer* m_SpawnCountBuffer;

public:
	inline void SetParticleBuffer(CStructuredBuffer* buffer) { m_ParticleBuffer = buffer; }
	inline void SetSpawnCountBuffer(CStructuredBuffer* buffer) { m_SpawnCountBuffer = buffer; }
};

