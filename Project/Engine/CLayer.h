#pragma once
#include "CEntity.h"

class CGameObject;

class CLayer : public CEntity
{
public:
	CLayer();
	~CLayer();

	void Begin();
	void Tick();
	void FinalTick();
	void Render();

	inline void AddObject(CGameObject* object) { m_vecObject.push_back(object); }

private:
	std::vector<CGameObject*> m_vecObject;
};

