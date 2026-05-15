#include "pch.h"
#include "CGameObject.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"

#include "CComponent.h"
#include "CRenderComponent.h"
#include "CScript.h"

CGameObject::CGameObject()
	: m_arrComp{}, m_RenderComp(nullptr), m_Parent(nullptr), m_LayerIdx(-1)
{
}

CGameObject::~CGameObject()
{
	Safe_Del_Array(m_arrComp);
	Safe_Del_Vector(m_vecScript);
	Safe_Del_Vector(m_vecChild);
}

void CGameObject::Begin()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (m_arrComp[i] != nullptr)
			m_arrComp[i]->Begin();
	}
	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->Begin();
	}
	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->Begin();
	}
}

void CGameObject::Tick()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (m_arrComp[i] != nullptr)
			m_arrComp[i]->Tick();
	}
	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->Tick();
	}
	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->Tick();
	}
}

void CGameObject::FinalTick()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (m_arrComp[i] != nullptr)
			m_arrComp[i]->FinalTick();
	}

	CLevelMgr::GetInst()->GetCurrentLevel()->GetLayer(m_LayerIdx)->RegisterObject(this);

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->FinalTick();
	}
}

void CGameObject::Render()
{
	if (m_RenderComp)
	{
		m_RenderComp->Render();
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
		assert(!m_arrComp[(UINT)type]);

		CRenderComponent* pRenderCom = dynamic_cast<CRenderComponent*>(component);
		if (pRenderCom != nullptr)
		{
			assert(!m_RenderComp);
			m_RenderComp = pRenderCom;
		}

		m_arrComp[(UINT)type] = component;
	}

	
	component->m_Owner = this;
}

void CGameObject::AddChild(CGameObject* obj)
{
	obj->m_Parent = this;
	m_vecChild.push_back(obj);
}
