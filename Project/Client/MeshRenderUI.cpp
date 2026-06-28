#include "pch.h"
#include "MeshRenderUI.h"

MeshRenderUI::MeshRenderUI()
	: ComponentUI("MeshRenderUI", "##MeshRenderUI", COMPONENT_TYPE::MESHRENDER)
{
	SetChildSize(ImVec2(0.f, 300.f));
}

MeshRenderUI::~MeshRenderUI()
{
}

void MeshRenderUI::Render_Tick()
{
	Render_Title();
}
