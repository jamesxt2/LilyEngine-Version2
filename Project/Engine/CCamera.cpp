#include "pch.h"
#include "CCamera.h"

#include "CRenderMgr.h"
#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "CRenderComponent.h"
#include "CTransform.h"
#include "CDevice.h"

CCamera::CCamera()
	: CComponent(COMPONENT_TYPE::CAMERA), 
	m_ProjType(PROJ_TYPE::PERSPECTIVE) , m_CamPriority(-1),
	m_FOV(XM_PI / 3.f), m_Far(10000.f), m_Width(0.f), m_AspectRatio(1.f),
	m_Scale(1.f), m_LayerCheck(0)
{
	m_Width = CDevice::GetInst()->GetRenderResolution().x;
	m_AspectRatio = CDevice::GetInst()->GetAspectRatio();
}

CCamera::CCamera(const CCamera& other)
	: CComponent(other),
	m_ProjType(other.m_ProjType),
	m_CamPriority(-1),
	m_FOV(other.m_FOV),
	m_Far(other.m_Far),
	m_Width(other.m_Width),
	m_AspectRatio(other.m_AspectRatio),
	m_Scale(other.m_Scale),
	m_LayerCheck(other.m_LayerCheck)
{
}

CCamera::~CCamera()
{
}

void CCamera::Begin()
{
	assert(m_CamPriority >= 0);
	CRenderMgr::GetInst()->RegisterCamera(this, m_CamPriority);
}

void CCamera::FinalTick()
{
	// View
	Vec3 vCamWorldPos = GetOwner()->GetTransformComp()->GetRelativePosition();
	Matrix matViewTrans = XMMatrixTranslation(-vCamWorldPos.x, -vCamWorldPos.y, -vCamWorldPos.z);

	Vec3 vR = GetOwner()->GetTransformComp()->GetRelativeDir(DIR_TYPE::RIGHT);
	Vec3 vU = GetOwner()->GetTransformComp()->GetRelativeDir(DIR_TYPE::UP);
	Vec3 vF = GetOwner()->GetTransformComp()->GetRelativeDir(DIR_TYPE::FORWARD);

	Matrix matViewRot = XMMatrixIdentity();
	matViewRot._11 = vR.x; matViewRot._12 = vU.x; matViewRot._13 = vF.x;
	matViewRot._21 = vR.y; matViewRot._22 = vU.y; matViewRot._23 = vF.y;
	matViewRot._31 = vR.z; matViewRot._32 = vU.z; matViewRot._33 = vF.z;

	m_matView = matViewTrans * matViewRot;

	// Projection
	if (m_ProjType == PROJ_TYPE::PERSPECTIVE)
		m_matProj = XMMatrixPerspectiveFovLH(m_FOV, m_AspectRatio, 1.f, m_Far);
	else
		m_matProj = XMMatrixOrthographicLH(m_Scale * m_Width, m_Scale * m_Width / m_AspectRatio, 1.f, m_Far);
}

void CCamera::Render()
{
	g_Trans.matView = m_matView;
	g_Trans.matProj = m_matProj;
	
	SortObject();

	Render_Opaque();
	Render_Masked();
	Render_Transparent();
	Render_Particle();
	Render_PostProcess();
	
	m_vecOpaque.clear();
	m_vecMasked.clear();
	m_vecTransparent.clear();
	m_vecParticle.clear();
	m_vecPostProcess.clear();
}

void CCamera::Render_Opaque()
{
	for (size_t i = 0; i < m_vecOpaque.size(); ++i)
		m_vecOpaque[i]->Render();
}

void CCamera::Render_Masked()
{
	for (size_t i = 0; i < m_vecMasked.size(); ++i)
		m_vecMasked[i]->Render();
}

void CCamera::Render_Transparent()
{
	for (size_t i = 0; i < m_vecTransparent.size(); ++i)
		m_vecTransparent[i]->Render();
}

void CCamera::Render_Particle()
{
	for (size_t i = 0; i < m_vecParticle.size(); ++i)
		m_vecParticle[i]->Render();
}

void CCamera::Render_PostProcess()
{
	for (size_t i = 0; i < m_vecPostProcess.size(); ++i)
	{
		CRenderMgr::GetInst()->CopyRenderTarget();
		m_vecPostProcess[i]->Render();
	}
}

void CCamera::LayerCheck(int layerIdx)
{
	if (m_LayerCheck & (1 << layerIdx))
		m_LayerCheck &= ~(1 << layerIdx);
	else
		m_LayerCheck |= (1 << layerIdx);
}

void CCamera::SortObject()
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (m_LayerCheck & (1 << i))
		{
			CLayer* pLayer = pCurLevel->GetLayer(i);
			const std::vector<CGameObject*>& obj = pLayer->GetObjects();
			for (size_t j = 0; j < obj.size(); ++j)
			{
				if (obj[j]->GetRenderComp() == nullptr ||
					obj[j]->GetRenderComp()->GetMaterial() == nullptr ||
					obj[j]->GetRenderComp()->GetMaterial()->GetShader() == nullptr)
					continue;

				SHADER_DOMAIN domain = obj[j]->GetRenderComp()->GetMaterial()->GetShader()->GetShaderDomain();
				switch (domain)
				{
				case SHADER_DOMAIN::DOMAIN_OPAQUE:
					m_vecOpaque.push_back(obj[j]);
					break;
				case SHADER_DOMAIN::DOMAIN_MASKED:
					m_vecMasked.push_back(obj[j]);
					break;
				case SHADER_DOMAIN::DOMAIN_TRANSPARENT:
					m_vecTransparent.push_back(obj[j]);
					break;
				case SHADER_DOMAIN::DOMAIN_PARTICLE:
					m_vecParticle.push_back(obj[j]);
					break;
				case SHADER_DOMAIN::DOMAIN_POSTPROCESS:
					m_vecPostProcess.push_back(obj[j]);
					break;
				}
			}
		}
	}
}

void CCamera::SetCameraPriority(int priority)
{
	m_CamPriority = priority;
}