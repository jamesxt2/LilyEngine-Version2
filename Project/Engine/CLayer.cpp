#include "pch.h"
#include "CLayer.h"

#include "CGameObject.h"

CLayer::CLayer()
	: m_LayerIdx(0)
{
}

CLayer::~CLayer()
{
	Safe_Del_Vector(m_vecParent);
}

void CLayer::Begin()
{
	for (size_t i = 0; i < m_vecParent.size(); ++i)
	{
		m_vecParent[i]->Begin();
	}
}

void CLayer::Tick()
{
	for (size_t i = 0; i < m_vecParent.size(); ++i)
	{
		m_vecParent[i]->Tick();
	}
}

void CLayer::FinalTick()
{
	std::vector<CGameObject*>::iterator iter = m_vecParent.begin();
	for (; iter != m_vecParent.end(); )
	{
		(*iter)->FinalTick();
		if ((*iter)->IsDead())
			iter = m_vecParent.erase(iter);
		else
			++iter;
	}
}

void CLayer::AddObject(CGameObject* object, bool bChildMove)
{
	if (!object->GetParent())
		m_vecParent.push_back(object);

	static std::list<CGameObject*> queue;
	queue.clear();
	queue.push_back(object);

	while (!queue.empty())
	{
		CGameObject* pObject = queue.front();
		queue.pop_front();

		if (bChildMove)
			pObject->m_LayerIdx = m_LayerIdx;
		else
		{
			if (pObject == object || pObject->m_LayerIdx == -1)
				pObject->m_LayerIdx = m_LayerIdx;
		}

		const std::vector<CGameObject*>& vecChild = pObject->GetChild();
		for (size_t i = 0; i < vecChild.size(); ++i)
		{
			queue.push_back(vecChild[i]);
		}
	}

}
