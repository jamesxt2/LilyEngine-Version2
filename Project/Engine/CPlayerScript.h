#pragma once
#include "CScript.h"

class CCollision2D;
class CGameObject;

class CStructuredBuffer;

class CPlayerScript : public CScript
{
public:
	CPlayerScript();
	~CPlayerScript();
	CLONE(CPlayerScript)

	virtual void Begin() override;
	virtual void Tick() override;

	virtual void BeginOverlap(CCollision2D* ownCollision, CGameObject* otherObj, CCollision2D* otherCollision) override;
	virtual void Overlap(CCollision2D* ownCollision, CGameObject* otherObj, CCollision2D* otherCollision) override;
	virtual void EndOverlap(CCollision2D* ownCollision, CGameObject* otherObj, CCollision2D* otherCollision) override;

private:
	float m_Speed;
	CGameObject* m_Target;
	Ptr<CPrefab> m_Particle;
	CGameObject* m_ParticleObj;

public:
	inline void SetTarget(CGameObject* target) { m_Target = target; }
};

