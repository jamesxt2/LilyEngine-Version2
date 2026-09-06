#pragma once
#include "AssetUI.h"

class GraphicsShaderUI : public AssetUI
{
public:
	GraphicsShaderUI();
	~GraphicsShaderUI();

	virtual void Render_Tick() override;
};

