#pragma once
#include "CScript.h"

class CMissileScript : public CScript
{
public:
	CMissileScript();
	~CMissileScript();
	CLONE(CMissileScript)

	virtual void Begin() override;
	virtual void Tick() override;

private:
	float m_Speed;

};

