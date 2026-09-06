#pragma once
#include "AssetUI.h"

class MeshUI : public AssetUI
{
public:
	MeshUI();
	~MeshUI();

	virtual void Render_Tick() override;
};

