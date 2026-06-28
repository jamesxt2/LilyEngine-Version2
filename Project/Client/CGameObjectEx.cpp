#include "pch.h"
#include "CGameObjectEx.h"

#include <Engine/CComponent.h>

CGameObjectEx::CGameObjectEx()
{
}

CGameObjectEx::~CGameObjectEx()
{
}

void CGameObjectEx::FinalTick()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (GetComponent(COMPONENT_TYPE(i)) != nullptr)
			GetComponent(COMPONENT_TYPE(i))->FinalTick();
	}

	const std::vector<CGameObject*>& vecChild = GetChild();
	for (size_t i = 0; i < vecChild.size(); ++i)
	{
		vecChild[i]->FinalTick();
	}
}
