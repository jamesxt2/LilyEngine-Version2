#include "pch.h"
#include "CGameObject.h"

#include "CComponent.h"
#include "CRenderComponent.h"
#include "CScript.h"

CGameObject::CGameObject()
	: m_arrCom{}, m_RenderCom(nullptr)
{
}

CGameObject::~CGameObject()
{
	Safe_Del_Array(m_arrCom);
}

void CGameObject::Begin()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (m_arrCom[i] != nullptr)
			m_arrCom[i]->Begin();
	}
	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->Begin();
	}
}

void CGameObject::Tick()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (m_arrCom[i] != nullptr)
			m_arrCom[i]->Tick();
	}
	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->Tick();
	}
}

void CGameObject::FinalTick()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (m_arrCom[i] != nullptr)
			m_arrCom[i]->FinalTick();
	}
}

void CGameObject::Render()
{
	if (m_RenderCom)
	{
		m_RenderCom->Render();
	}
}

void CGameObject::AddComponent(CComponent* component)
{
	COMPONENT_TYPE type = component->GetComponentType();

	if (type == COMPONENT_TYPE::SCRIPT)
	{
		m_vecScript.push_back((CScript*)component);
	}
	else
	{
		assert(!m_arrCom[(UINT)type]);

		CRenderComponent* pRenderCom = dynamic_cast<CRenderComponent*>(component);
		if (pRenderCom != nullptr)
		{
			assert(!m_RenderCom);
			m_RenderCom = pRenderCom;
		}

		m_arrCom[(UINT)type] = component;
	}

	
	component->m_Owner = this;
}
