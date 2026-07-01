#include "pch.h"
#include "EditorUI.h"

EditorUI::EditorUI(const std::string& name, const std::string& ID)
	: m_Name(name), m_ID(ID), m_ParentUI(nullptr), 
	m_Active(true), m_Separate(false), m_Modal(false)
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
		bool bActive = m_Active;
		if (!m_Modal)
		{
			ImGui::Begin(fullName.c_str(), &bActive);
			SetActive(bActive);
			Render_Tick();
			for (size_t i = 0; i < m_vecChildUI.size(); ++i)
			{
				m_vecChildUI[i]->Tick();
			}
			ImGui::End();
		}
		else
		{
			ImGui::OpenPopup(fullName.c_str());
			if (ImGui::BeginPopupModal(fullName.c_str(), &bActive))
			{
				SetActive(bActive);
				Render_Tick();
				for (size_t i = 0; i < m_vecChildUI.size(); ++i)
				{
					m_vecChildUI[i]->Tick();
				}
				ImGui::EndPopup();
			}
			else
				SetActive(bActive);
		}
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

void EditorUI::SetFocus()
{
	std::string fullName = m_Name + m_ID;
	ImGui::SetWindowFocus(fullName.c_str());
}

void EditorUI::SetActive(bool active)
{
	if (m_Active == active) return;

	m_Active = active;

	if (m_Active)
		Activate();
	else
		Deactivate();
}
