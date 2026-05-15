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

	void AddObject(UINT layerIdx, CGameObject* object, bool bChildMove = false);

private:
	CLayer* m_arrLayer[MAX_LAYER];

public:
	inline CLayer* GetLayer(int layerIndex) const { return m_arrLayer[layerIndex]; }
};

