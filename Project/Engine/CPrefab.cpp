#include "pch.h"
#include "CPrefab.h"

#include "CGameObject.h"
#include "CTransform.h"

CPrefab::CPrefab()
	: CAsset(ASSET_TYPE::PREFAB), m_ProtoObj(nullptr)
{
}

CPrefab::CPrefab(CGameObject* proto)
	: CAsset(ASSET_TYPE::PREFAB),
	m_ProtoObj(proto)
{
	m_ProtoObj->GetTransformComp()->SetRelativePosition(0.f, 0.f, 0.f);
}

CPrefab::CPrefab(const CPrefab& other)
	: CAsset(other),
	m_ProtoObj(nullptr)
{
	if (other.m_ProtoObj != nullptr)
	{
		m_ProtoObj = other.m_ProtoObj->Clone();
		m_ProtoObj->GetTransformComp()->SetRelativePosition(0.f, 0.f, 0.f);
	}
}

CPrefab::~CPrefab()
{
	if (m_ProtoObj != nullptr)
		delete m_ProtoObj;
}

CGameObject* CPrefab::Instantiate()
{
	return m_ProtoObj->Clone();
}
