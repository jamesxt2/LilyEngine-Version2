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
	inline void ClearEvent() { m_ObjectEvent = false; }

private:
	std::vector<TTask> m_vecTask;
	std::vector<CGameObject*> m_vecDead;

	bool m_ObjectEvent;

public:
	inline bool IsObjectChanged() const { return m_ObjectEvent; }
};

