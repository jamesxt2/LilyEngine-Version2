#include "pch.h"
#include "ComponentUI.h"

ComponentUI::ComponentUI(const std::string& name, const std::string& ID, COMPONENT_TYPE type)
	: EditorUI(name, ID), m_TargetObject(nullptr), m_Type(type)
{
}

ComponentUI::~ComponentUI()
{
}

void ComponentUI::Render_Title()
{
	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(2.f / 7.0f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(2.f / 7.0f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(2.f / 7.0f, 0.6f, 0.6f));
	ImGui::Button(COMPONENT_TYPE_STRING[(UINT)GetComponentType()]);
	ImGui::PopStyleColor(3);
	ImGui::PopID();
}

void ComponentUI::SetTargetObject(CGameObject* object)
{
	m_TargetObject = object;

	if (m_TargetObject == nullptr)
		SetActive(false);
	else
	{
		if (m_TargetObject->GetComponent(m_Type) == nullptr)
			SetActive(false);
		else
			SetActive(true);
	}
}