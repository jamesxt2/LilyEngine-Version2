#pragma once
#include "CRenderComponent.h"

class CMeshRender : public CRenderComponent
{
public:
	CMeshRender();
	~CMeshRender();

	virtual void FinalTick() override;
	virtual void Render() override;
};

