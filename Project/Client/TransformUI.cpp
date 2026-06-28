#include "pch.h"
#include "TransformUI.h"

TransformUI::TransformUI()
	: ComponentUI("Transform", "##TransformUI", COMPONENT_TYPE::TRANSFORM)
{
	SetChildSize(ImVec2(0.f, 200.f));
}

TransformUI::~TransformUI()
{
}

void TransformUI::Render_Tick()
{
	Render_Title();
}
