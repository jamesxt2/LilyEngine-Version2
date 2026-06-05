#include "pch.h"
#include "CBackgroundScript.h"

#include "CRenderComponent.h"

CBackgroundScript::CBackgroundScript()
	: m_Intense(0.f)
{
}

CBackgroundScript::~CBackgroundScript()
{
}

void CBackgroundScript::Tick()
{
	m_Intense += 0.1f * DT;
	GetOwner()->GetRenderComp()->GetMaterial()->SetScalarParam(FLOAT_0, m_Intense);
}
