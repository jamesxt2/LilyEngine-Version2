#pragma once
#include "AssetUI.h"

class ComputeShaderUI : public AssetUI
{
public:
	ComputeShaderUI();
	~ComputeShaderUI();

	virtual void Render_Tick() override;
};

