#include "pch.h"
#include "CCameraMoveScript.h"

CCameraMoveScript::CCameraMoveScript()
	: m_Speed(100.f)
{
}

CCameraMoveScript::~CCameraMoveScript()
{
}

void CCameraMoveScript::Tick()
{
	Vec3 vCurPos = GetOwner()->GetTransformComp()->GetRelativePosition();
	Vec3 vForward = GetOwner()->GetTransformComp()->GetRelativeDir(DIR_TYPE::FORWARD);
	Vec3 vRight = GetOwner()->GetTransformComp()->GetRelativeDir(DIR_TYPE::RIGHT);

	if (KEY_PRESSED(KEY::W))
	{
		vCurPos += vForward * m_Speed * DT;
	}
	if (KEY_PRESSED(KEY::S))
	{
		vCurPos += -vForward * m_Speed * DT;
	}
	if (KEY_PRESSED(KEY::A))
	{
		vCurPos += -vRight * m_Speed * DT;
	}
	if (KEY_PRESSED(KEY::D))
	{
		vCurPos += vRight * m_Speed * DT;
	}
	GetOwner()->GetTransformComp()->SetRelativePosition(vCurPos);

	Vec3 vCurRot = GetOwner()->GetTransformComp()->GetRelativeRotation();
	if (KEY_PRESSED(KEY::RBUTTON))
	{
		vCurRot.y += CKeyMgr::GetInst()->GetMouseMoveDir().x * XM_PI * DT * 10.f;
		vCurRot.x -= CKeyMgr::GetInst()->GetMouseMoveDir().y * XM_PI * DT * 10.f;
	}
	GetOwner()->GetTransformComp()->SetRelativeRotation(vCurRot);
}
