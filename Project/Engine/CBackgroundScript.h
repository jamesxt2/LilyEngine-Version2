#pragma once
#include "CScript.h"

class CBackgroundScript : public CScript
{
public:
	CBackgroundScript();
	~CBackgroundScript();

	virtual void Tick() override;

private:
	float m_Intense;
};

