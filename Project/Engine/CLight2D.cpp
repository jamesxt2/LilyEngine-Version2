#include "pch.h"
#include "CLight2D.h"

#include "CTransform.h"

#include "CRenderMgr.h"

CLight2D::CLight2D()
	: CComponent(COMPONENT_TYPE::LIGHT2D), m_LightIdx(0)
{
}

CLight2D::~CLight2D()
{
}

void CLight2D::FinalTick()
{
	m_Info.WorldPos = GetOwner()->GetTransformComp()->GetWorldPosition();
	m_Info.WorldDir = GetOwner()->GetTransformComp()->GetWorldDir(DIR_TYPE::RIGHT);

	m_LightIdx = CRenderMgr::GetInst()->RegisterLight2D(this);
}

void CLight2D::SetLightType(LIGHT_TYPE type)
{
	m_Info.LightType = (UINT)type;
}
