#include "pch.h"
#include "CPlayerScript.h"

#include "CMissileScript.h"
#include <Engine/CMeshRender.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CCollision2D.h>
#include <Engine/CStructuredBuffer.h>

CPlayerScript::CPlayerScript()
	: m_Speed(100.f), m_Target(nullptr)
{

}

CPlayerScript::~CPlayerScript()
{

}

void CPlayerScript::Begin()
{
	m_ParticlePrefab = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"ParticlePrefab");
	m_MissilePrefab = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"MissilePrefab");
}

void CPlayerScript::Tick()
{
	if (KEY_TAP(KEY::_1))
	{
		Ptr<CMaterial> pMtrl = GetOwner()->GetRenderComp()->GetDynamicMaterial();
		pMtrl->SetScalarParam(INT_0, 1);
	}
	if (KEY_TAP(KEY::_2))
	{
		GetOwner()->GetRenderComp()->RestoreMaterial();
	}

	Vec3 vCurPos = GetOwner()->GetTransformComp()->GetRelativePosition();
	if (KEY_PRESSED(KEY::UP))
	{
		vCurPos.y += DT * m_Speed;
	}
	if (KEY_PRESSED(KEY::DOWN))
	{
		vCurPos.y -= DT * m_Speed;
	}
	if (KEY_PRESSED(KEY::LEFT))
	{
		vCurPos.x -= DT * m_Speed;
	}
	if (KEY_PRESSED(KEY::RIGHT))
	{
		vCurPos.x += DT * m_Speed;
	}
	/*
	if (IsValid(m_Target))
	{
		Vec3 vDir = m_Target->GetTransformComp()->GetWorldPosition() - GetOwner()->GetTransformComp()->GetWorldPosition();
		vDir.Normalize();
		vCurPos += vDir * m_Speed * DT;
	}
	*/
	GetOwner()->GetTransformComp()->SetRelativePosition(vCurPos);

	Vec3 vCurRot = GetOwner()->GetTransformComp()->GetRelativeRotation();
	if (KEY_PRESSED(KEY::M))
	{
		vCurRot.z += DT * m_Speed / 10.f;
	}
	if (KEY_PRESSED(KEY::N))
	{
		vCurRot.z -= DT * m_Speed / 10.f;
	}
	GetOwner()->GetTransformComp()->SetRelativeRotation(vCurRot);

	if (KEY_TAP(KEY::SPACE))
	{
		Instantiate(m_MissilePrefab, 0, GetOwner()->GetTransformComp()->GetRelativePosition());
	}

}

void CPlayerScript::BeginOverlap(CCollision2D* ownCollision, CGameObject* otherObj, CCollision2D* otherCollision)
{

}

void CPlayerScript::Overlap(CCollision2D* ownCollision, CGameObject* otherObj, CCollision2D* otherCollision)
{
}

void CPlayerScript::EndOverlap(CCollision2D* ownCollision, CGameObject* otherObj, CCollision2D* otherCollision)
{
}
