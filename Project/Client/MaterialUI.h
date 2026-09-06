#pragma once
#include "AssetUI.h"

class MaterialUI : public AssetUI
{
public:
	MaterialUI();
	~MaterialUI();

	virtual void Render_Tick() override;
};

