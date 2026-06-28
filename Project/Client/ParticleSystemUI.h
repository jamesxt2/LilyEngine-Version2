#pragma once
#include "ComponentUI.h"

class ParticleSystemUI : public ComponentUI
{
public:
	ParticleSystemUI();
	~ParticleSystemUI();

	virtual void Render_Tick() override;
};

