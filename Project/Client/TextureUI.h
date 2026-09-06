#pragma once
#include "AssetUI.h"

class TextureUI : public AssetUI
{
public:
	TextureUI();
	~TextureUI();

	virtual void Render_Tick() override;
};

