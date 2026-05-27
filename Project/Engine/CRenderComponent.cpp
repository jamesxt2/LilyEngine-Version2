#include "pch.h"
#include "CRenderComponent.h"

CRenderComponent::CRenderComponent(COMPONENT_TYPE type)
	: CComponent(type)
{
}

CRenderComponent::~CRenderComponent()
{
}

void CRenderComponent::SetMaterial(Ptr<CMaterial> material)
{
	assert(!material->IsDynamic());

	m_CurMtrl = m_SharedMtrl = material;
}

Ptr<CMaterial> CRenderComponent::GetDynamicMaterial()
{
	if (m_SharedMtrl == nullptr)
		return nullptr;

	if (m_DynamicMtrl != nullptr)
		return m_DynamicMtrl;

	m_CurMtrl = m_DynamicMtrl = m_SharedMtrl->GetDynamicMaterial();
	return m_DynamicMtrl;
}