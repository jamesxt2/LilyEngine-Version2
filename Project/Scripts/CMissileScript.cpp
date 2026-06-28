#include "pch.h"
#include "CMissileScript.h"

#include <Engine/CRenderComponent.h>

CMissileScript::CMissileScript()
	: m_Speed(500.f)
{
}

CMissileScript::~CMissileScript()
{
}

void CMissileScript::Begin()
{
	GetOwner()->GetRenderComp()->GetDynamicMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"texture/Effect.png", L"texture/Effect.png"));
}

void CMissileScript::Tick()
{
	Vec3 vPos = GetOwner()->GetTransformComp()->GetRelativePosition();
	vPos.y += m_Speed * DT;
	GetOwner()->GetTransformComp()->SetRelativePosition(vPos);
}
