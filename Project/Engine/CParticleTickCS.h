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
	CStructuredBuffer* m_ModuleBuffer;

	Ptr<CTexture> m_NoiseTex;
	Vec3 m_ParticleWorldPos;

public:
	inline void SetParticleBuffer(CStructuredBuffer* buffer) { m_ParticleBuffer = buffer; }
	inline void SetSpawnCountBuffer(CStructuredBuffer* buffer) { m_SpawnCountBuffer = buffer; }
	inline void SetModuleBuffer(CStructuredBuffer* buffer) { m_ModuleBuffer = buffer; }
	inline void SetNoiseTexture(const Ptr<CTexture>& tex) { m_NoiseTex = tex; }
	inline void SetParticleWorldPos(const Vec3& pos) { m_ParticleWorldPos = pos; }
};

