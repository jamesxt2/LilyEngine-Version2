#include "pch.h"
#include "CPlayerScript.h"

#include "CMissileScript.h"
#include "CMeshRender.h"
#include "CLevelMgr.h"
#include "CLevel.h"
#include "CCollision2D.h"

#include "CStructuredBuffer.h"

CPlayerScript::CPlayerScript()
	: m_Speed(100.f), m_Target(nullptr)
{

}

CPlayerScript::~CPlayerScript()
{

}

void CPlayerScript::Begin()
{

}

void CPlayerScript::Tick()
{
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
		if (GetOwner()->GetCollision2DComp()->IsActive())
		{
			GetOwner()->GetCollision2DComp()->Deactivate();
			m_Speed = 10.f;
		}
		else
			GetOwner()->GetCollision2DComp()->Activate();

		CGameObject* pNewObj = new CGameObject;
		pNewObj->AddComponent(new CTransform);
		pNewObj->AddComponent(new CMeshRender);
		pNewObj->AddComponent(new CMissileScript);

		Vec3 vPos = GetOwner()->GetTransformComp()->GetRelativePosition();
		vPos.y += GetOwner()->GetTransformComp()->GetRelativeScale().y / 2.f;
		pNewObj->GetTransformComp()->SetRelativePosition(vPos);
		pNewObj->GetTransformComp()->SetRelativeScale(Vec3(40.f, 40.f, 40.f));

		pNewObj->GetMeshRenderComp()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
		pNewObj->GetMeshRenderComp()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMaterial"));

		SpawnObject(0, pNewObj);
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
