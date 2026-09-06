#pragma once
#include "AssetUI.h"

class SoundUI : public AssetUI
{
public:
	SoundUI();
	~SoundUI();

	virtual void Render_Tick() override;
};

