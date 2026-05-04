#pragma once
#include "CScript.h"

class CCameraMoveScript : public CScript
{
public:
	CCameraMoveScript();
	~CCameraMoveScript();
	virtual void Tick() override;

private:
	float m_Speed;
};

