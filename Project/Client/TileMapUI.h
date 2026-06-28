#pragma once
#include "ComponentUI.h"

class TileMapUI : public ComponentUI
{
public:
	TileMapUI();
	~TileMapUI();

	virtual void Render_Tick() override;
};

