#pragma once
#include "ComponentUI.h"

class MeshRenderUI : public ComponentUI
{
public:
	MeshRenderUI();
	~MeshRenderUI();

	virtual void Render_Tick() override;
};

