#include "pch.h"
#include "CameraUI.h"

CameraUI::CameraUI()
	: ComponentUI("Camera", "##CameraUI", COMPONENT_TYPE::CAMERA)
{
	SetChildSize(ImVec2(0.f, 300.f));
}

CameraUI::~CameraUI()
{
}

void CameraUI::Render_Tick()
{
	Render_Title();
}
