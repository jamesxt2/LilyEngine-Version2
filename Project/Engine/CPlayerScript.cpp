#include "pch.h"
#include "CPlayerScript.h"

CPlayerScript::CPlayerScript()
	: m_Speed(100.f)
{
}

CPlayerScript::~CPlayerScript()
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
	if (KEY_PRESSED(KEY::M))
	{
		vCurPos.z += DT * XM_PI * 10;
	}
	if (KEY_PRESSED(KEY::N))
	{
		vCurPos.z -= DT * XM_PI * 10;
	}
	GetOwner()->GetTransformComp()->SetRelativePosition(vCurPos);
}
