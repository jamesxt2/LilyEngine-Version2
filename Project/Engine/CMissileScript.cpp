#include "pch.h"
#include "CMissileScript.h"

CMissileScript::CMissileScript()
	: m_Speed(500.f)
{
}

CMissileScript::~CMissileScript()
{
}

void CMissileScript::Begin()
{
}

void CMissileScript::Tick()
{
	Vec3 vPos = GetOwner()->GetTransformComp()->GetRelativePosition();
	vPos.y += m_Speed * DT;
	GetOwner()->GetTransformComp()->SetRelativePosition(vPos);
}
