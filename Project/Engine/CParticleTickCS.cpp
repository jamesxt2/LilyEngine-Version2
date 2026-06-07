#include "pch.h"
#include "CParticleTickCS.h"

#include "CStructuredBuffer.h"
#include "CAssetMgr.h"

CParticleTickCS::CParticleTickCS()
	: CComputeShader(32, 1, 1),
	m_ParticleBuffer(nullptr), m_SpawnCountBuffer(nullptr), m_ModuleBuffer(nullptr)
{
	m_NoiseTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture/noise/noise_03.jpg", L"texture/noise/noise_03.jpg");
}

CParticleTickCS::~CParticleTickCS()
{
}

int CParticleTickCS::Bind()
{
	if (m_ParticleBuffer == nullptr || m_SpawnCountBuffer == nullptr || m_NoiseTex == nullptr) 
		return E_FAIL;

	m_ParticleBuffer->Bind_CS_UAV(0);
	m_SpawnCountBuffer->Bind_CS_UAV(1);
	m_NoiseTex->Bind_CS_SRV(18);
	m_ModuleBuffer->Bind_CS_SRV(19);

	m_Const.iArr[0] = m_ParticleBuffer->GetElementCount();
	m_Const.v4Arr[0] = m_ParticleWorldPos;

	return S_OK;
}

void CParticleTickCS::CalculateGroupNum()
{
	m_GroupX = m_ParticleBuffer->GetElementCount() / m_ThreadPerGroupX;
	if (m_ParticleBuffer->GetElementCount() % m_ThreadPerGroupX)
		++m_GroupX;

	m_GroupY = m_GroupZ = 1;
}

void CParticleTickCS::Clear()
{
	m_ParticleBuffer->Clear_CS_UAV(0);
	m_ParticleBuffer = nullptr;

	m_SpawnCountBuffer->Clear_CS_UAV(1);
	m_SpawnCountBuffer = nullptr;

	m_NoiseTex->Clear_CS_SRV(18);

	m_ModuleBuffer->Clear_CS_SRV(19);
	m_ModuleBuffer = nullptr;
}
