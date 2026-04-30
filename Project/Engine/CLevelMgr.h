#pragma once
#include "singleton.h"

class CLevel;

class CLevelMgr : public CSingleton<CLevelMgr>
{
	SINGLE(CLevelMgr)

public:
	void Init();
	void Tick();
	void Render();

private:
	CLevel* m_CurLevel;
};

