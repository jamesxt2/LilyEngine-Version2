#include "pch.h"
#include "CameraUI.h"

#include <Engine/CCamera.h>
#include <Engine/CRenderMgr.h>

#include <sstream>

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

	// Projection Type
	CCamera* pCam = GetTargetObject()->GetCameraComp();
	PROJ_TYPE projType = pCam->GetProjType();

	ImGui::Text("Projection Type");
	ImGui::SameLine(120);

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

	// Camera Priority
	std::vector<CCamera*>& vecCam = CRenderMgr::GetInst()->GetRegisteredCamera();

	ImGui::Text("Camera Priority");
	ImGui::SameLine(120);
	int priority = pCam->GetCameraPriority();
	if (ImGui::InputInt("##CameraPriority", &priority))
	{
		int curPriority = pCam->GetCameraPriority();
		vecCam[curPriority] = nullptr;
		pCam->SetCameraPriority(priority);
	}

	std::vector<std::string> vecCamName;
	for (size_t i = 0; i < vecCam.size(); ++i)
	{
		std::stringstream buffer;
		buffer << "Priority " << i << " : ";
		if (vecCam[i] == nullptr)
			vecCamName.push_back(buffer.str() + "None");
		else
			vecCamName.push_back(buffer.str() + WStringToString(vecCam[i]->GetOwner()->GetName()));
	}

	ImGui::Text("");
	ImGui::SameLine(120);
	std::stringstream buffer;
	buffer << "Priority " << pCam->GetCameraPriority() << " : ";
	std::string curCamName = buffer.str() + WStringToString(pCam->GetOwner()->GetName());
	if (ImGui::BeginCombo("##RegisteredCamera", curCamName.c_str()))
	{
		for (size_t i = 0; i < vecCamName.size(); ++i)
		{
			const bool bSelected = curCamName == vecCamName[i];
			if (ImGui::Selectable(vecCamName[i].c_str(), bSelected))
			{

			}
		}
		ImGui::EndCombo();
	}

	// FOV
	float FOV = pCam->GetFOV();
	FOV = (FOV * 180.f) / XM_PI;

	ImGui::BeginDisabled(pCam->GetProjType() == PROJ_TYPE::ORTHOGRAPHIC);
	ImGui::Text("FOV");
	ImGui::SameLine(120);
	if (ImGui::DragFloat("##CameraFOV", &FOV, 0.1f, 0.1f, 179.9f))
	{
		FOV = std::clamp(FOV, 0.1f, 179.9f);
		FOV = (FOV * XM_PI) / 180.f;
		pCam->SetFOV(FOV);
	}
	ImGui::EndDisabled();

	// Far
	float Far = pCam->GetFar();

	ImGui::Text("Far");
	ImGui::SameLine(120);
	if (ImGui::DragFloat("##CameraFar", &Far, 1.f))
	{
		if (Far < 2.f) Far = 2.f;
		pCam->SetFar(Far);
	}

	// Width
	float width = pCam->GetWidth();

	ImGui::Text("Width");
	ImGui::SameLine(120);
	if (ImGui::DragFloat("##CameraWidth", &width))
	{
		if (width < 1.f) width = 1.f;
		pCam->SetWidth(width);
	}

	// Aspect Ratio
	float aspectRatio = pCam->GetAspectRatio();

	ImGui::Text("AspectRatio");
	ImGui::SameLine(120);
	if (ImGui::DragFloat("##CameraAspectRatio", &aspectRatio))
	{
		if (aspectRatio < 0.f) aspectRatio = 0.f;
		pCam->SetAspectRatio(aspectRatio);
	}

	// Scale
	float scale = pCam->GetScale();

	ImGui::BeginDisabled(pCam->GetProjType() == PROJ_TYPE::PERSPECTIVE);
	ImGui::Text("Scale");
	ImGui::SameLine(120);
	if (ImGui::DragFloat("##CameraScale", &scale, 0.1f))
	{
		if (scale < 0.1f) scale = 0.1f;
		pCam->SetScale(scale);
	}
	ImGui::EndDisabled();
}
