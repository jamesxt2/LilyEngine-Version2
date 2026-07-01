#include "pch.h"
#include "CameraUI.h"

#include <Engine/CCamera.h>

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

	CCamera* pCam = GetTargetObject()->GetCameraComp();
	PROJ_TYPE projType = pCam->GetProjType();

	const char* items[2] = { "ORTHOGRAPHIC", "PERSPECTIVE" };

	if (ImGui::BeginCombo("##CameraProjType", items[(UINT)projType]))
	{
		for (int i = 0; i < 2; ++i)
		{
			const bool bSelected = (UINT)projType == i;
			if (ImGui::Selectable(items[i], bSelected))
				pCam->SetProjType((PROJ_TYPE)i);
		}
		ImGui::EndCombo();
	}
}
