#pragma once
#include "singleton.h"

class CTaskMgr : public CSingleton<CTaskMgr>
{
	SINGLE(CTaskMgr)

public:
	void Tick();
	void AddTask(const TTask& task) { m_vecTask.push_back(task); }

private:
	void ClearDeadObject();
	void ExecuteTask(TTask& task);

private:
	std::vector<TTask> m_vecTask;
	std::vector<CGameObject*> m_vecDead;
};

