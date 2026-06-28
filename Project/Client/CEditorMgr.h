#pragma once
#include <Engine\singleton.h>
class CEditorMgr : public CSingleton<CEditorMgr>
{
	SINGLE(CEditorMgr)

public:
	void Init();
	void Tick();

private:
	std::vector<CGameObject*> m_vecEditorObj;
};

