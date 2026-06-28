#pragma once
#include "CEntity.h"

class CGameObject;

class CLayer : public CEntity
{
public:
	CLayer();
	~CLayer();
	CLayer(const CLayer& other) = delete;
	friend class CLevel;
	CLONE_DISABLE(CLayer)

	void Begin();
	void Tick();
	void FinalTick();

	void AddObject(CGameObject* object, bool bChildMove = false);
	inline void RegisterObject(CGameObject* object) { m_vecObject.push_back(object); }

	inline void Clear() { m_vecObject.clear(); }

private:
	std::vector<CGameObject*> m_vecParent;
	std::vector<CGameObject*> m_vecObject;
	int m_LayerIdx;

public:
	inline const std::vector<CGameObject*>& GetParentObjects() const { return m_vecParent; }
	inline const std::vector<CGameObject*>& GetObjects() const { return m_vecObject; }
};

