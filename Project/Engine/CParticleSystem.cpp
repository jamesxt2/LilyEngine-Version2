#include "pch.h"
#include "CParticleSystem.h"

#include "CStructuredBuffer.h"
#include "CDevice.h"
#include "CTransform.h"
#include "CTimeMgr.h"

CParticleSystem::CParticleSystem()
	: CRenderComponent(COMPONENT_TYPE::PARTICLESYSTEM),
	m_ParticleBuffer(nullptr), m_SpawnCountBuffer(nullptr), m_SpawnCount(0),
	m_Time(0.f), m_MaxParticle(100), m_SpawnRate(10)
{
	m_TickCS = (CParticleTickCS*)CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"ParticleCS").Get();

	SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"ParticleMaterial"));
	
	Vec2 vResolution = CDevice::GetInst()->GetRenderResolution();
	float term = vResolution.x / (m_MaxParticle + 1);

	TParticle arrParticle[100] = {};
	for (UINT i = 0; i < m_MaxParticle; ++i)
	{
		arrParticle[i].vWorldPos = Vec3(-(vResolution.x / 2.f) + (i + 1) * term, 0.f, 100.f);
		arrParticle[i].vWorldScale = Vec3(10.f, 10.f, 1.f);
		arrParticle[i].vColor = Vec4(1.f, 0.2f, 0.f, 1.f);
		arrParticle[i].Active = 0;
		//if (i % 3 == 0)
			//arrParticle[i].Active = 0;
	}

	m_ParticleBuffer = new CStructuredBuffer;
	m_ParticleBuffer->Create(sizeof(TParticle), m_MaxParticle, SB_TYPE::SRV_UAV, false, arrParticle);

	m_SpawnCountBuffer = new CStructuredBuffer();
	m_SpawnCountBuffer->Create(sizeof(TSpawnCount), 1, SB_TYPE::SRV_UAV, true);
}

CParticleSystem::~CParticleSystem()
{
	delete m_ParticleBuffer;
	delete m_SpawnCountBuffer;
}

void CParticleSystem::FinalTick()
{
	CalculateSpawnCount();

	m_TickCS->SetParticleBuffer(m_ParticleBuffer);
	m_TickCS->SetSpawnCountBuffer(m_SpawnCountBuffer);

	if (FAILED(m_TickCS->Execute())) assert(nullptr);
}

void CParticleSystem::CalculateSpawnCount()
{
	float term = 1.f / (float)m_SpawnRate;
	m_Time += DT;
	m_SpawnCount = 0;

	if (m_Time > term)
	{
		float value = m_Time / term;
		m_SpawnCount = (int)floor(value);
		m_Time -= (float)m_SpawnCount * term;
	}

	TSpawnCount spawnCount = { m_SpawnCount, };
	m_SpawnCountBuffer->SetData(&spawnCount);
}


void CParticleSystem::Render()
{
	m_ParticleBuffer->Bind(17);
	GetOwner()->GetTransformComp()->Bind();

	GetMaterial()->SetTexParam(TEX_1, m_ParticleTex);
	GetMaterial()->Bind();

	GetMesh()->Render_Particle(m_MaxParticle);

	m_ParticleBuffer->Clear(17);
}

