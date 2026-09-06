#pragma once
#include "AssetUI.h"

class MeshDataUI : public AssetUI
{
public:
	MeshDataUI();
	~MeshDataUI();

	virtual void Render_Tick() override;
};

