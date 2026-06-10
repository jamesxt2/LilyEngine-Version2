#include "pch.h"
#include "CPrefab.h"

#include "CGameObject.h"

CPrefab::CPrefab()
	: CAsset(ASSET_TYPE::PREFAB), m_ProtoObj(nullptr)
{
}

CPrefab::~CPrefab()
{
	if (m_ProtoObj != nullptr)
		delete m_ProtoObj;
}
