#pragma once
#include "ComponentUI.h"

class CameraUI : public ComponentUI
{
public:
	CameraUI();
	~CameraUI();

	virtual void Render_Tick() override;
};

