#pragma once
#include "singleton.h"

union COL_ID
{
	struct
	{
		UINT LeftID;
		UINT RightID;
	};

	LONGLONG ID;
};

class CCollision2D;

class CCollisionMgr : public CSingleton<CCollisionMgr>
{
	SINGLE(CCollisionMgr)

public:
	void Tick();
	void LayerCheck(UINT layerLeftIdx, UINT layerRightIdx);

private:
	void CollisionBetweenLayers(UINT left, UINT right);
	void CollisionBtwCollision2D(CCollision2D* leftCol, CCollision2D* rightCol);

	bool IsCollision(CCollision2D* leftCol, CCollision2D* rightCol);

private:
	UINT m_Matrix[MAX_LAYER];
	std::map<LONGLONG, bool> m_ColInfo;
};

