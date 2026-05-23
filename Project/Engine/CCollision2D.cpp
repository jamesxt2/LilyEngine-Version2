#include "pch.h"
#include "CCollision2D.h"

#include "CTransform.h"
#include "CScript.h"

#include "CTaskMgr.h"

CCollision2D::CCollision2D()
	: CComponent(COMPONENT_TYPE::COLLISION2D),
	m_Scale(Vec3(1.f, 1.f, 1.f)),
	m_Absolute(false), m_OverlapCount(0),
	m_Active(true), m_SemiActive(false)
{
}

CCollision2D::~CCollision2D()
{
}

void CCollision2D::FinalTick()
{
	if (m_SemiActive)
		CTaskMgr::GetInst()->AddTask(TTask{ TASK_TYPE::COLLISION2D_DEACTIVATE, (DWORD_PTR)this });
	else if (!m_Active)
		return;

	m_FinalPos = GetOwner()->GetTransformComp()->GetRelativePosition() + m_Offset;
	
	Matrix matScale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	Matrix matRotation = XMMatrixRotationZ(m_Rotation.z);
	Matrix matTranslation = XMMatrixTranslation(m_Offset.x, m_Offset.y, m_Offset.z);

	m_matWorld = matScale * matRotation * matTranslation;

	if (!m_Absolute)
	{
		m_matWorld *= GetOwner()->GetTransformComp()->GetWorldMat();
	}
	else
	{
		Vec3 objScale = GetOwner()->GetTransformComp()->GetRelativeScale();
		Matrix matObjScale = XMMatrixScaling(objScale.x, objScale.y, objScale.z);

		m_matWorld *= ((Matrix)XMMatrixInverse(nullptr, matObjScale) * GetOwner()->GetTransformComp()->GetWorldMat());
	}

	if (m_OverlapCount == 0)
		DrawDebugRect(m_matWorld, Vec4(0.f, 1.f, 0.f, 1.f), 0.f);
	else if (m_OverlapCount >= 1)
		DrawDebugRect(m_matWorld, Vec4(1.f, 0.f, 0.f, 1.f), 0.f);
}

void CCollision2D::BeginOverlap(CCollision2D* otherCollision)
{
	++m_OverlapCount;
	const std::vector<CScript*>& script = GetOwner()->GetScript();
	for (size_t i = 0; i < script.size(); ++i)
	{
		script[i]->BeginOverlap(this, otherCollision->GetOwner(), otherCollision);
	}
}

void CCollision2D::Overlap(CCollision2D* otherCollision)
{
	const std::vector<CScript*>& script = GetOwner()->GetScript();
	for (size_t i = 0; i < script.size(); ++i)
	{
		script[i]->Overlap(this, otherCollision->GetOwner(), otherCollision);
	}
}

void CCollision2D::EndOverlap(CCollision2D* otherCollision)
{
	--m_OverlapCount;
	const std::vector<CScript*>& script = GetOwner()->GetScript();
	for (size_t i = 0; i < script.size(); ++i)
	{
		script[i]->EndOverlap(this, otherCollision->GetOwner(), otherCollision);
	}
}

void CCollision2D::Activate()
{
	m_Active = true;
}

void CCollision2D::Deactivate()
{
	TTask task = {};
	task.type = TASK_TYPE::COLLISION2D_SEMI_DEACTIVATE;
	task.dwParam_0 = (DWORD_PTR)this;
	CTaskMgr::GetInst()->AddTask(task);
}
