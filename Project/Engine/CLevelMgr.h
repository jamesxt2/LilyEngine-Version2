#pragma once
#include "singleton.h"

class CLevel;

class CLevelMgr : public CSingleton<CLevelMgr>
{
	SINGLE(CLevelMgr)

public:
	void Init();
	void Tick();

private:
	CLevel* m_CurLevel;

public:
	inline CLevel* GetCurrentLevel() const { return m_CurLevel; }
};

