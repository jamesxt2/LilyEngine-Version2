#pragma once
#include "CComponent.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CAssetMgr.h"

#include "CTransform.h"

class CCollision2D;
class CGameObject;

class CScript : public CComponent
{
public:
	CScript();
	~CScript();

	virtual void Tick() = 0;
	virtual void FinalTick() final {}

	CGameObject* Instantiate(Ptr<CPrefab> prefab, int layerIdx, const Vec3& worldPos);

	virtual void BeginOverlap(CCollision2D* ownCollision, CGameObject* otherObj, CCollision2D* otherCollision) {}
	virtual void Overlap(CCollision2D* ownCollision, CGameObject* otherObj, CCollision2D* otherCollision) {}
	virtual void EndOverlap(CCollision2D* ownCollision, CGameObject* otherObj, CCollision2D* otherCollision) {}
};

