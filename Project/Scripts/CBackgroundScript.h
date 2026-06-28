#pragma once
#include <Engine/CScript.h>

class CBackgroundScript : public CScript
{
public:
	CBackgroundScript();
	~CBackgroundScript();
	CLONE(CBackgroundScript)

	virtual void Tick() override;

private:
	float m_Intense;
};

