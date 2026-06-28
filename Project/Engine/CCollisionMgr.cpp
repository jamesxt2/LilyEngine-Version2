#include "pch.h"
#include "CCollisionMgr.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "CCollision2D.h"

CCollisionMgr::CCollisionMgr()
	: m_Matrix{}
{

}

CCollisionMgr::~CCollisionMgr()
{

}

void CCollisionMgr::Tick()
{
	if (!CLevelMgr::GetInst()->GetCurrentLevel())
		return;

	for (UINT row = 0; row < MAX_LAYER; ++row)
	{
		for (UINT col = row; col < MAX_LAYER; ++col)
		{
			if (m_Matrix[row] & (1 << col))
			{
				CollisionBetweenLayers(row, col);
			}
		}
	}
}

void CCollisionMgr::LayerCheck(UINT layerLeftIdx, UINT layerRightIdx)
{
	UINT col, row;
	if (layerLeftIdx > layerRightIdx)
	{
		col = layerLeftIdx;
		row = layerRightIdx;
	}
	else
	{
		col = layerRightIdx;
		row = layerLeftIdx;
	}

	UINT checkBit = (1 << col);

	if (m_Matrix[row] & checkBit)
		m_Matrix[row] &= ~checkBit;
	else
		m_Matrix[row] |= checkBit;
}

void CCollisionMgr::CollisionBetweenLayers(UINT left, UINT right)
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	CLayer* pLeftLayer = pCurLevel->GetLayer(left);
	CLayer* pRightLayer = pCurLevel->GetLayer(right);

	const std::vector<CGameObject*>& leftObj = pLeftLayer->GetObjects();
	const std::vector<CGameObject*>& rightObj = pRightLayer->GetObjects();

	if (left != right)
	{
		for (size_t i = 0; i < leftObj.size(); ++i)
		{
			if (leftObj[i]->GetCollision2DComp() == nullptr)
				continue;
			for (size_t j = 0; j < rightObj.size(); ++j)
			{
				if (rightObj[j]->GetCollision2DComp() == nullptr)
					continue;
				CollisionBtwCollision2D(leftObj[i]->GetCollision2DComp(), rightObj[j]->GetCollision2DComp());
			}
		}
	}
	else
	{
		for (size_t i = 0; i < leftObj.size(); ++i)
		{
			if (leftObj[i]->GetCollision2DComp() == nullptr)
				continue;
			for (size_t j = i + 1; j < rightObj.size(); ++j)
			{
				if (rightObj[j]->GetCollision2DComp() == nullptr)
					continue;
				CollisionBtwCollision2D(leftObj[i]->GetCollision2DComp(), rightObj[j]->GetCollision2DComp());
			}
		}
	}
}

void CCollisionMgr::CollisionBtwCollision2D(CCollision2D* leftCol, CCollision2D* rightCol)
{
	COL_ID id;
	id.LeftID = leftCol->GetID();
	id.RightID = rightCol->GetID();

	std::map<LONGLONG, bool>::iterator iter = m_ColInfo.find(id.ID);
	if (iter == m_ColInfo.end())
	{
		m_ColInfo.insert(std::make_pair(id.ID, false));
		iter = m_ColInfo.find(id.ID);
	}

	bool bDead = leftCol->GetOwner()->IsDead() || rightCol->GetOwner()->IsDead();
	bool bActive = leftCol->IsActive() && rightCol->IsActive();
	bool bSemiDeactive = leftCol->IsSemiDeactive() || rightCol->IsSemiDeactive();

	if (!bActive) return;

	if (IsCollision(leftCol, rightCol))
	{
		if (iter->second)
		{
			// Overlap
			if (bDead || bSemiDeactive)
			{
				leftCol->EndOverlap(rightCol);
				rightCol->EndOverlap(leftCol);
				iter->second = false;
			}
			else
			{
				leftCol->Overlap(rightCol);
				rightCol->Overlap(leftCol);
			}
		}
		else
		{
			// Begin Overlap
			if (!bDead && !bSemiDeactive)
			{
				leftCol->BeginOverlap(rightCol);
				rightCol->BeginOverlap(leftCol);
				iter->second = true;
			}
		}
	}
	else
	{
		if (iter->second)
		{
			// End Overlap
			leftCol->EndOverlap(rightCol);
			rightCol->EndOverlap(leftCol);
			iter->second = false;
		}
	}
}

bool CCollisionMgr::IsCollision(CCollision2D* leftCol, CCollision2D* rightCol)
{
	// OBB
	static Vec3 vLocal[4] =
	{
		Vec3(-0.5f, 0.5f, 0.f),
		Vec3(0.5f, 0.5f, 0.f),
		Vec3(0.5f, -0.5f, 0.f),
		Vec3(-0.5f, -0.5f, 0.f)
	};

	Vec3 vLeftCol[3] = {};
	Vec3 vRightCol[3] = {};

	for (int i = 0; i < 3; ++i)
	{
		vLeftCol[i] = XMVector3TransformCoord(vLocal[i], leftCol->GetWorldMat());
		vRightCol[i] = XMVector3TransformCoord(vLocal[i], rightCol->GetWorldMat());
	}

	Vec3 vLeftCenter = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), leftCol->GetWorldMat());
	Vec3 vRightCenter = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), rightCol->GetWorldMat());
	Vec3 vCenter = vRightCenter - vLeftCenter;

	Vec3 arrProj[4] =
	{
		vLeftCol[1] - vLeftCol[0],
		vLeftCol[2] - vLeftCol[1],
		vRightCol[1] - vRightCol[0],
		vRightCol[2] - vRightCol[1]
	};

	for (int i = 0; i < 4; ++i)
	{
		Vec3 vProj = arrProj[i];
		vProj.Normalize();

		float projLen = 0.f;
		for (int j = 0; j < 4; ++j)
		{
			projLen += fabs(arrProj[j].Dot(vProj));
		}

		projLen *= 0.5f;
		float centerLen = fabs(vCenter.Dot(vProj));

		if (centerLen > projLen)
			return false;
	}

	return true;
}
