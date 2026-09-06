#include "pch.h"
#include "AssetUI.h"

AssetUI::AssetUI(const std::string& name, const std::string& ID, ASSET_TYPE type)
	: EditorUI(name, ID), m_Type(type)
{
}

AssetUI::~AssetUI()
{
}

void AssetUI::Render_Title()
{
	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(3.f / 7.0f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(3.f / 7.0f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(3.f / 7.0f, 0.6f, 0.6f));
	ImGui::Button(ASSET_TYPE_STRING[(UINT)GetAssetType()]);
	ImGui::PopStyleColor(3);
	ImGui::PopID();
}
