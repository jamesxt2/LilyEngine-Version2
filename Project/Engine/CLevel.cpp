#include "pch.h"
#include "CLevel.h"

#include "CLayer.h"
#include "CRenderMgr.h"
#include "CGameObject.h"

CLevel::CLevel()
	: m_arrLayer{}, m_State(LEVEL_STATE::STOP)
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i] = new CLayer;
		m_arrLayer[i]->m_LayerIdx = i;
	}
}

CLevel::~CLevel()
{
	Safe_Del_Array(m_arrLayer);
}

void CLevel::Begin()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i]->Begin();
	}
}

void CLevel::Tick()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i]->Tick();
	}
}

void CLevel::FinalTick()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i]->FinalTick();
	}
}

void CLevel::RegisterClear()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i]->Clear();
	}
}

void CLevel::AddObject(UINT layerIdx, CGameObject* object, bool bChildMove)
{
	m_arrLayer[layerIdx]->AddObject(object, bChildMove);
}

CGameObject* CLevel::FindObjectByName(const std::wstring& name)
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		const std::vector<CGameObject*>& vecObjects = m_arrLayer[i]->GetObjects();
		for (size_t j = 0; j < vecObjects.size(); ++j)
		{
			if (vecObjects[j]->GetName() == name)
				return vecObjects[j];
		}
	}

	return nullptr;
}

void CLevel::ChangeState(LEVEL_STATE nextState)
{
	if (nextState == LEVEL_STATE::STOP || nextState == LEVEL_STATE::PAUSE)
		CRenderMgr::GetInst()->ChangeRenderMode(RENDER_MODE::EDITOR);

	if (m_State == nextState) return;
	assert(!(m_State == LEVEL_STATE::STOP && nextState == LEVEL_STATE::PAUSE));

	if (m_State == LEVEL_STATE::STOP && nextState == LEVEL_STATE::PLAY)
		Begin();

	m_State = nextState;
}
