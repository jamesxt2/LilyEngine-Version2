#include "pch.h"
#include "Collision2DUI.h"

Collision2DUI::Collision2DUI()
	: ComponentUI("Collision2D", "##Collision2DUI", COMPONENT_TYPE::COLLISION2D)
{
	SetChildSize(ImVec2(0.f, 300.f));
}

Collision2DUI::~Collision2DUI()
{
}

void Collision2DUI::Render_Tick()
{
	Render_Title();
}
