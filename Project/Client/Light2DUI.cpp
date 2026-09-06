#include "pch.h"
#include "Light2DUI.h"

#include <Engine/CLight2D.h>

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

	CLight2D* pLight2D = GetTargetObject()->GetLight2DComp();
	const TLightInfo& info = pLight2D->GetLightInfo();

	// Light Type
	ImGui::Text("Light2D Type");
	ImGui::SameLine(120);
	const char* arrLightType[3] = { "Directional", "Point", "Spot" };

	if (ImGui::BeginCombo("##Light2DType", arrLightType[info.LightType]))
	{
		for (int i = 0; i < 3; ++i)
		{
			const bool bSelected = info.LightType == i;
			if (ImGui::Selectable(arrLightType[i], bSelected))
				pLight2D->SetLightType((LIGHT_TYPE)i);
		}
		ImGui::EndCombo();
	}

	// Light Color
	ImGui::Text("Light2D Color");
	ImGui::SameLine(120);
	ImGui::ColorEdit3("##Light2DColor", (float*)&info.Light.vDiffuse);

	// Light Ambient
	ImGui::Text("Light2D Ambient");
	ImGui::SameLine(120);
	ImGui::ColorEdit3("##Light2DAmbient", (float*)&info.Light.vAmbient);

	// Range
	ImGui::BeginDisabled(info.LightType == (UINT)LIGHT_TYPE::DIRECTIONAL);
	ImGui::Text("Light2D Range");
	ImGui::SameLine(120);
	float range = info.Range;
	if (ImGui::DragFloat("##Light2DRange", &range, 0.1f))
	{
		if (range < 0.f) range = 0.f;
		pLight2D->SetRange(range);
	}
	ImGui::EndDisabled();

	// Angle
	ImGui::BeginDisabled(info.LightType != (UINT)LIGHT_TYPE::SPOT);
	ImGui::Text("Light2D Angle");
	ImGui::SameLine(120);
	float angle = info.Angle;
	if (ImGui::DragFloat("##SpotLightAngle", &angle))
	{
		pLight2D->SetAngle(angle);
	}
	ImGui::EndDisabled();
}
