#include "pch.h"
#include "TransformUI.h"

#include <Engine/CTransform.h>

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

	Vec3 vPosition = GetTargetObject()->GetTransformComp()->GetRelativePosition();
	Vec3 vRotation = GetTargetObject()->GetTransformComp()->GetRelativeRotation();
	Vec3 vScale = GetTargetObject()->GetTransformComp()->GetRelativeScale();

	ImGui::Text("Position  ");
	ImGui::SameLine();
	if (ImGui::DragFloat3("##Position", vPosition))
		GetTargetObject()->GetTransformComp()->SetRelativePosition(vPosition);

	ImGui::Text("Rotation  ");
	ImGui::SameLine();
	vRotation = (vRotation / XM_PI) * 180.f;
	if (ImGui::DragFloat3("##Rotation", vRotation))
	{
		vRotation = (vRotation / 180.f) * XM_PI;
		GetTargetObject()->GetTransformComp()->SetRelativeRotation(vRotation);
	}

	ImGui::Text("Scale     ");
	ImGui::SameLine();
	if (ImGui::DragFloat3("##Scale", vScale))
		GetTargetObject()->GetTransformComp()->SetRelativeScale(vScale);

	bool bAbsolute = GetTargetObject()->GetTransformComp()->IsAbsolute();
	ImGui::Text("Absolute  ");
	ImGui::SameLine();
	if (ImGui::Checkbox("##Absolute", &bAbsolute))
		GetTargetObject()->GetTransformComp()->SetAbsolute(bAbsolute);
}
