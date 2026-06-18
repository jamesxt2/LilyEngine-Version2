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
	
	m_ParticleBuffer = new CStructuredBuffer;
	m_ParticleBuffer->Create(sizeof(TParticle), m_MaxParticle, SB_TYPE::SRV_UAV, false, arrParticle);

	m_SpawnCountBuffer = new CStructuredBuffer;
	m_SpawnCountBuffer->Create(sizeof(TSpawnCount), 1, SB_TYPE::SRV_UAV, true);
	
	// Spawn Module
	m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN] = 1;
	m_Module.SpawnRate = 20;
	m_Module.vSpawnColor = Vec4(1.f, 0.8f, 0.2f, 1.f);
	m_Module.MinLife = 5.f;
	m_Module.MaxLife = 5.f;
	m_Module.Mass = 1.f;
	m_Module.vSpawnMinScale = Vec3(120.f, 10.f, 1.f);
	m_Module.vSpawnMaxScale = Vec3(120.f, 10.f, 1.f);

	m_Module.SpawnShape = 1;
	m_Module.SpawnShapeScale.x = 100.f;

	m_Module.BlockSpawnShape = 1;
	m_Module.BlockSpawnShapeScale.x = 0.f;

	m_Module.SpaceType = 0;

	// Spawn Burst Module
	m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN_BURST] = 1;
	m_Module.SpawnBurstRepeat = 1;
	m_Module.SpawnBurstCount = 100;
	m_Module.SpawnBurstRepeatTime = 4.f;

	// Add Velocity Module
	m_Module.Module[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = 1;
	m_Module.AddVelocityType = 1;
	m_Module.AddVelocityFixedDir = Vec3(0.f, 1.f, 0.f);
	m_Module.AddMinSpeed = 50.f;
	m_Module.AddMaxSpeed = 50.f;

	// Scale Module
	m_Module.Module[(UINT)PARTICLE_MODULE::SCALE] = 0;
	m_Module.StartScale = 0.5f;
	m_Module.EndScale = 3.f;

	// Drag Module
	m_Module.Module[(UINT)PARTICLE_MODULE::DRAG] = 0;
	m_Module.DstNormalizedAge = 0.5f;
	m_Module.LimitSpeed = 0.f;

	// Noise Force Module
	m_Module.Module[(UINT)PARTICLE_MODULE::NOISE_FORCE] = 0;
	m_Module.NoiseForceTerm = 0.3f;
	m_Module.NoiseForceScale = 400.f;

	// Render Module
	m_Module.Module[(UINT)PARTICLE_MODULE::RENDER] = 1;
	m_Module.EndColor = Vec3(1.f, 0.2f, 0.8f);
	m_Module.FadeOut = 1;
	m_Module.StartRatio = 0.5f;
	m_Module.VelocityAlignment = 1;

	m_ModuleBuffer = new CStructuredBuffer;
	m_ModuleBuffer->Create(sizeof(TParticleModule), 1, SB_TYPE::SRV_UAV, true, &m_Module);
}

CParticleSystem::CParticleSystem(const CParticleSystem& other)
	: CRenderComponent(other),
	m_ParticleBuffer(nullptr),
	m_SpawnCountBuffer(nullptr),
	m_ModuleBuffer(nullptr),
	m_ParticleTex(other.m_ParticleTex),
	m_TickCS(other.m_TickCS),
	m_Time(0.f),
	m_BurstTime(0.f),
	m_MaxParticle(other.m_MaxParticle),
	m_Module(other.m_Module)
{
	assert(other.m_ParticleBuffer && other.m_SpawnCountBuffer && other.m_ModuleBuffer);

	m_ParticleBuffer = new CStructuredBuffer(*other.m_ParticleBuffer);
	m_SpawnCountBuffer = new CStructuredBuffer(*other.m_SpawnCountBuffer);
	m_ModuleBuffer = new CStructuredBuffer(*other.m_ModuleBuffer);
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
	m_ModuleBuffer->Bind(18);
	GetOwner()->GetTransformComp()->Bind();

	GetMaterial()->SetTexParam(TEX_1, m_ParticleTex);
	GetMaterial()->Bind();

	GetMesh()->Render_Particle(m_MaxParticle);

	m_ParticleBuffer->Clear(17);
	m_ModuleBuffer->Clear(18);
}

