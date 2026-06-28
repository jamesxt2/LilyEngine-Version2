#pragma once
#include "ComponentUI.h"

class Collision2DUI : public ComponentUI
{
public:
	Collision2DUI();
	~Collision2DUI();

	virtual void Render_Tick() override;
};

