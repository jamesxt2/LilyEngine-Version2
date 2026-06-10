#pragma once
#include "CAsset.h"

class CGameObject;

class CPrefab : public CAsset
{
public:
	CPrefab();
	~CPrefab();

private:
	CGameObject* m_ProtoObj;
};

