#pragma once
#include "ComponentUI.h"

class Animator2DUI : public ComponentUI
{
public:
	Animator2DUI();
	~Animator2DUI();

	virtual void Render_Tick() override;
};

