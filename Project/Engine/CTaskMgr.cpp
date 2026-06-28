#include "pch.h"
#include "CTaskMgr.h"

#include "CGameObject.h"
#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CCollision2D.h"

CTaskMgr::CTaskMgr()
{

}

CTaskMgr::~CTaskMgr()
{

}

void CTaskMgr::Tick()
{
	ClearDeadObject();
	for (size_t i = 0; i < m_vecTask.size(); ++i)
	{
		ExecuteTask(m_vecTask[i]);
	}
	m_vecTask.clear();
}

void CTaskMgr::ClearDeadObject()
{
	for (size_t i = 0; i < m_vecDead.size(); ++i)
		delete m_vecDead[i];
	m_vecDead.clear();
}

void CTaskMgr::ExecuteTask(TTask& task)
{
	switch (task.type)
	{
	case TASK_TYPE::SPAWN_OBJECT:
	{
		int LayerIdx = (int)task.dwParam_0;
		CGameObject* pSpawnObj = (CGameObject*)task.dwParam_1;
		CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
		pCurLevel->AddObject(LayerIdx, pSpawnObj);
		if (pCurLevel->GetState() == LEVEL_STATE::PLAY)
			pSpawnObj->Begin();
	}
		break;
	case TASK_TYPE::DESTROY_OBJECT:
	{
		CGameObject* pObject = (CGameObject*)task.dwParam_0;
		if (!pObject->IsDead())
		{
			pObject->m_Dead = true;
			m_vecDead.push_back(pObject);
		}
	}
		break;
	case TASK_TYPE::COLLISION2D_SEMI_DEACTIVATE:
	{
		CCollision2D* pCol = (CCollision2D*)task.dwParam_0;
		pCol->m_SemiActive = true;
	}
		break;
	case TASK_TYPE::COLLISION2D_DEACTIVATE:
	{
		CCollision2D* pCol = (CCollision2D*)task.dwParam_0;
		pCol->m_SemiActive = false;
		pCol->m_Active = false;
	}
		break;
	case TASK_TYPE::CHANGE_LEVEL:
	{
		CLevel* pNextLevel = (CLevel*)task.dwParam_0;
		LEVEL_STATE nextLevelState = (LEVEL_STATE)task.dwParam_1;

		CLevelMgr::GetInst()->ChangeLevel(pNextLevel);
		pNextLevel->ChangeState(nextLevelState);
	}
		break;
	}
}
