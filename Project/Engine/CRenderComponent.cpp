#include "pch.h"
#include "CRenderComponent.h"

CRenderComponent::CRenderComponent(COMPONENT_TYPE type)
	: CComponent(type)
{
}

CRenderComponent::CRenderComponent(const CRenderComponent& other)
	: CComponent(other),
	m_Mesh(other.m_Mesh),
	m_CurMtrl(nullptr),
	m_SharedMtrl(other.m_SharedMtrl),
	m_DynamicMtrl(nullptr)
{
	if (other.m_DynamicMtrl != nullptr && other.m_DynamicMtrl == other.m_CurMtrl)
	{
		GetDynamicMaterial();
		m_DynamicMtrl = other.m_DynamicMtrl;
	}
	else if (other.m_SharedMtrl != nullptr && other.m_SharedMtrl == other.m_CurMtrl)
	{
		m_CurMtrl = m_SharedMtrl;
	}
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
	{
		m_CurMtrl = m_DynamicMtrl;
		return m_DynamicMtrl;
	}

	m_CurMtrl = m_DynamicMtrl = m_SharedMtrl->GetDynamicMaterial();
	return m_DynamicMtrl;
}