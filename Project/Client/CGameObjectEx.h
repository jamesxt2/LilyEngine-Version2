#pragma once
#include <Engine\CGameObject.h>

class CGameObjectEx : public CGameObject
{
public:
	CGameObjectEx();
	~CGameObjectEx();

	virtual void FinalTick() override;
};

