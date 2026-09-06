#pragma once
#include "AssetUI.h"

class PrefabUI : public AssetUI
{
public:
	PrefabUI();
	~PrefabUI();

	virtual void Render_Tick() override;
};

