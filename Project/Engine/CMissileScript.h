#pragma once
#include "CScript.h"

class CMissileScript : public CScript
{
public:
	CMissileScript();
	~CMissileScript();

	virtual void Tick() override;

private:
	float m_Speed;

};

