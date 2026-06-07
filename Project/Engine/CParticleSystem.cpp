#include "pch.h"
#include "CParticleSystem.h"

#include "CStructuredBuffer.h"
#include "CDevice.h"
#include "CTransform.h"
#include "CTimeMgr.h"

CParticleSystem::CParticleSystem()
	: CRenderComponent(COMPONENT_TYPE::PARTICLESYSTEM),
	m_ParticleBuffer(nullptr), m_SpawnCountBuffer(nullptr),
	m_Time(0.f), m_MaxParticle(100), m_BurstTime(0.f)
{
	m_TickCS = (CParticleTickCS*)CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"ParticleCS").Get();

	SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"ParticleMaterial"));
	
	Vec2 vResolution = CDevice::GetInst()->GetRenderResolution();
	float term = vResolution.x / (m_MaxParticle + 1);

	TParticle arrParticle[100] = {};
	for (UINT i = 0; i < m_MaxParticle; ++i)
	{
		arrParticle[i].vLocalPosition = Vec3(0.f, 0.f, 0.f);
		arrParticle[i].vWorldPosition = Vec3(0.f, 0.f, 0.f);
		arrParticle[i].vWorldScale = Vec3(20.f, 20.f, 1.f);
		arrParticle[i].vColor = Vec4(1.f, 0.2f, 0.f, 1.f);
		arrParticle[i].Active = 0;
	}

	m_ParticleBuffer = new CStructuredBuffer;
	m_ParticleBuffer->Create(sizeof(TParticle), m_MaxParticle, SB_TYPE::SRV_UAV, false, arrParticle);

	m_SpawnCountBuffer = new CStructuredBuffer;
	m_SpawnCountBuffer->Create(sizeof(TSpawnCount), 1, SB_TYPE::SRV_UAV, true);
	
	// Spawn Module
	m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN] = 1;
	m_Module.SpawnRate = 20;
	m_Module.vSpawnColor = Vec4(1.f, 1.f, 1.f, 1.f);
	m_Module.MinLife = 1.f;
	m_Module.MaxLife = 4.f;
	m_Module.vSpawnMinScale = Vec3(2.f, 2.f, 1.f);
	m_Module.vSpawnMaxScale = Vec3(20.f, 20.f, 1.f);

	// Spawn Burst Module
	m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN_BURST] = 1;
	m_Module.SpawnBurstRepeat = 1;
	m_Module.SpawnBurstCount = 100;
	m_Module.SpawnBurstRepeatTime = 4.f;

	m_ModuleBuffer = new CStructuredBuffer;
	m_ModuleBuffer->Create(sizeof(TParticleModule) + 16 - sizeof(TParticleModule) % 16, 1, SB_TYPE::SRV_UAV, true, &m_Module);
}

CParticleSystem::~CParticleSystem()
{
	delete m_ParticleBuffer;
	delete m_SpawnCountBuffer;
}

void CParticleSystem::FinalTick()
{
	CalculateSpawnCount();

	m_TickCS->SetParticleWorldPos(GetOwner()->GetTransformComp()->GetWorldPosition());
	m_TickCS->SetModuleBuffer(m_ModuleBuffer);
	m_TickCS->SetParticleBuffer(m_ParticleBuffer);
	m_TickCS->SetSpawnCountBuffer(m_SpawnCountBuffer);

	if (FAILED(m_TickCS->Execute())) assert(nullptr);
}

void CParticleSystem::CalculateSpawnCount()
{
	m_Time += DT;
	TSpawnCount count = {};

	if (m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN])
	{
		float term = 1.f / (float)m_Module.SpawnRate;
		int spawnCount = 0;

		if (m_Time > term)
		{
			float value = m_Time / term;
			spawnCount = (int)floor(value);
			m_Time -= (float)spawnCount * term;
		}

		count.SpawnCount += spawnCount;
	}
	
	if (m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN_BURST])
	{
		int burstCount = 0;

		if (m_BurstTime == 0.f)
			burstCount = m_Module.SpawnBurstCount;

		m_BurstTime += DT;

		if (m_Module.SpawnBurstRepeat && m_BurstTime >= m_Module.SpawnBurstRepeatTime)
			m_BurstTime = 0.f;
		
		count.SpawnCount += burstCount;
	}
	
	m_SpawnCountBuffer->SetData(&count);
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

