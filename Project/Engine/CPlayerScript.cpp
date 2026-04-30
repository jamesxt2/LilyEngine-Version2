#include "pch.h"
#include "CPlayerScript.h"

CPlayerScript::CPlayerScript()
	: m_Speed(1.f)
{
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::Tick()
{
	Vec3 vCurPos = GetOwner()->GetTransformComp()->GetRelativePosition();

	if (KEY_PRESSED(KEY::W))
	{
		vCurPos.y += DT * m_Speed;
	}
	if (KEY_PRESSED(KEY::S))
	{
		vCurPos.y -= DT * m_Speed;
	}
	if (KEY_PRESSED(KEY::A))
	{
		vCurPos.x -= DT * m_Speed;
	}
	if (KEY_PRESSED(KEY::D))
	{
		vCurPos.x += DT * m_Speed;
	}

	GetOwner()->GetTransformComp()->SetRelativePosition(vCurPos);
}
