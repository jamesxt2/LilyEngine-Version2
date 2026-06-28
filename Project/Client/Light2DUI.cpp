#include "pch.h"
#include "Light2DUI.h"

Light2DUI::Light2DUI()
	: ComponentUI("Light2D", "##Light2DUI", COMPONENT_TYPE::LIGHT2D)
{
	SetChildSize(ImVec2(0.f, 300.f));
}

Light2DUI::~Light2DUI()
{
}

void Light2DUI::Render_Tick()
{
	Render_Title();
}
