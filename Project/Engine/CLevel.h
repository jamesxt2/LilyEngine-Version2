#pragma once
#include "CEntity.h"

class CLayer;
class CGameObject;

class CLevel : public CEntity
{
public:
	CLevel();
	~CLevel();
	CLevel(const CLevel& other) = delete;
	CLONE_DISABLE(CLevel)

	void Begin();
	void Tick();
	void FinalTick();
	void RegisterClear();

	void AddObject(UINT layerIdx, CGameObject* object, bool bChildMove = false);

	CGameObject* FindObjectByName(const std::wstring& name);

private:
	CLayer* m_arrLayer[MAX_LAYER];
	LEVEL_STATE m_State;

public:
	inline CLayer* GetLayer(int layerIndex) const { return m_arrLayer[layerIndex]; }
	inline LEVEL_STATE GetState() const { return m_State; }
	void ChangeState(LEVEL_STATE nextState);
};

