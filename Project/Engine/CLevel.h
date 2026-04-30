#pragma once
#include "CEntity.h"

class CLayer;
class CGameObject;

class CLevel : public CEntity
{
public:
	CLevel();
	~CLevel();

	void Begin();
	void Tick();
	void FinalTick();
	void Render();

	void AddObject(UINT layerIdx, CGameObject* object);

private:
	CLayer* m_arrLayer[MAX_LAYER];
};

