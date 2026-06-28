#include "pch.h"
#include "CEditorCameraScript.h"

#include <Engine/CCamera.h>

CEditorCameraScript::CEditorCameraScript()
	: m_Speed(100.f)
{
}

CEditorCameraScript::~CEditorCameraScript()
{
}

void CEditorCameraScript::Tick()
{
	CCamera* camera = GetOwner()->GetCameraComp();

	if (KEY_TAP(KEY::P))
	{
		PROJ_TYPE type = camera->GetProjType();
		camera->SetProjType(type == PROJ_TYPE::ORTHOGRAPHIC ? PROJ_TYPE::PERSPECTIVE : PROJ_TYPE::ORTHOGRAPHIC);

		if (type == PROJ_TYPE::PERSPECTIVE)
		{
			CTransform* transComp = GetOwner()->GetTransformComp();
			Vec3 pos = transComp->GetRelativePosition();
			pos.z = 0.f;
			transComp->SetRelativePosition(pos);
			transComp->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
		}
	}

	if (camera->GetProjType() == PROJ_TYPE::PERSPECTIVE)
		MoveByPerspective();
	else
		MoveByOrthographic();
}

void CEditorCameraScript::MoveByPerspective()
{
	CTransform* transComp = GetOwner()->GetTransformComp();
	if (!transComp) return;

	Vec3 vCurPos = transComp->GetRelativePosition();
	Vec3 vForward = transComp->GetRelativeDir(DIR_TYPE::FORWARD);
	Vec3 vRight = transComp->GetRelativeDir(DIR_TYPE::RIGHT);
	Vec3 vUp = transComp->GetRelativeDir(DIR_TYPE::UP);

	if (KEY_PRESSED(KEY::W))
	{
		vCurPos += vForward * m_Speed * DT_ENGINE;
	}
	if (KEY_PRESSED(KEY::S))
	{
		vCurPos += -vForward * m_Speed * DT_ENGINE;
	}
	if (KEY_PRESSED(KEY::A))
	{
		vCurPos += -vRight * m_Speed * DT_ENGINE;
	}
	if (KEY_PRESSED(KEY::D))
	{
		vCurPos += vRight * m_Speed * DT_ENGINE;
	}
	if (KEY_PRESSED(KEY::E))
	{
		vCurPos += vUp * m_Speed * DT_ENGINE;
	}
	if (KEY_PRESSED(KEY::Q))
	{
		vCurPos += -vUp * m_Speed * DT_ENGINE;
	}
	transComp->SetRelativePosition(vCurPos);

	Vec3 vCurRot = transComp->GetRelativeRotation();
	if (KEY_PRESSED(KEY::RBUTTON))
	{
		vCurRot.y += CKeyMgr::GetInst()->GetMouseMoveDir().x * XM_PI * DT_ENGINE * 10.f;
		vCurRot.x -= CKeyMgr::GetInst()->GetMouseMoveDir().y * XM_PI * DT_ENGINE * 10.f;
	}
	transComp->SetRelativeRotation(vCurRot);
}

void CEditorCameraScript::MoveByOrthographic()
{
	CTransform* transComp = GetOwner()->GetTransformComp();
	if (!transComp) return;

	Vec3 vCurPos = transComp->GetRelativePosition();

	if (KEY_PRESSED(KEY::W))
	{
		vCurPos.y += m_Speed * DT_ENGINE;
	}
	if (KEY_PRESSED(KEY::S))
	{
		vCurPos.y -= m_Speed * DT_ENGINE;
	}
	if (KEY_PRESSED(KEY::A))
	{
		vCurPos.x -= m_Speed * DT_ENGINE;
	}
	if (KEY_PRESSED(KEY::D))
	{
		vCurPos.x += m_Speed * DT_ENGINE;
	}
	if (KEY_PRESSED(KEY::E))
	{
		CCamera* camera = GetOwner()->GetCameraComp();
		float scale = camera->GetScale() - DT_ENGINE;
		camera->SetScale(scale < 0.01f ? 0.01f : scale);
	}
	if (KEY_PRESSED(KEY::Q))
	{
		CCamera* camera = GetOwner()->GetCameraComp();
		camera->SetScale(camera->GetScale() + DT_ENGINE);
	}
	transComp->SetRelativePosition(vCurPos);
}
