#pragma once
#include "singleton.h"

class CGameObject;

class CDebugRenderMgr : public CSingleton<CDebugRenderMgr>
{
	SINGLE(CDebugRenderMgr)

public:
	void Render();

	void AddDebugShapeInfo(const TDebugShapeInfo& info) { m_ShapeInfo.push_back(info); }

private:
	std::list<TDebugShapeInfo> m_ShapeInfo;
	CGameObject* m_DebugRenderObj;
};

