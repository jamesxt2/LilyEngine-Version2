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
	: CComponent(COMPONENT_TYPE::CAMERA), m_CamPriority(-1), m_Far(1000.f)
{
}

CCamera::~CCamera()
{
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
	m_matProj = XMMatrixPerspectiveFovLH((XM_PI / 3.f), CDevice::GetInst()->GetAspectRatio(), 1.f, m_Far);
}

void CCamera::Render()
{
	g_Trans.matView = m_matView;
	g_Trans.matProj = m_matProj;

	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = pCurLevel->GetLayer(i);
		pLayer->Render();
	}
}

void CCamera::SetCameraPriority(int priority)
{
	m_CamPriority = priority;
	if (m_CamPriority >= 0)
	{
		CRenderMgr::GetInst()->RegisterCamera(this, m_CamPriority);
	}
}
