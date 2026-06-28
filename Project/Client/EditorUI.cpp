#include "pch.h"
#include "EditorUI.h"

EditorUI::EditorUI(const std::string& name, const std::string& ID)
	: m_Name(name), m_ID(ID), m_ParentUI(nullptr), m_Active(true), m_Separate(false)
{
}

EditorUI::~EditorUI()
{
	Safe_Del_Vector(m_vecChildUI);
}

void EditorUI::Tick()
{
	if (!m_Active) return;

	std::string fullName = m_Name + m_ID;
	if (IsRootUI())
	{
		ImGui::Begin(fullName.c_str(), &m_Active);
		Render_Tick();
		for (size_t i = 0; i < m_vecChildUI.size(); ++i)
		{
			m_vecChildUI[i]->Tick();
		}
		ImGui::End();
	}
	else
	{
		if (m_Separate)
			ImGui::Separator();

		ImGui::BeginChild(fullName.c_str(), m_ChildSize);
		Render_Tick();
		for (size_t i = 0; i < m_vecChildUI.size(); ++i)
		{
			m_vecChildUI[i]->Tick();
		}
		ImGui::EndChild();
	}
}