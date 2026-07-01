#include "pch.h"
#include "ListUI.h"

ListUI::ListUI()
	: EditorUI("List", "##List"), m_SelectedIdx(-1), 
	m_Callback(nullptr), m_Delegate(nullptr), m_Inst(nullptr)
{
}

ListUI::~ListUI()
{
}

void ListUI::Render_Tick()
{
	ImVec2 vWinSize = ImGui::GetWindowContentRegionMax();
	vWinSize.y -= 27.f;
	vWinSize.x -= 8.f;

	if (ImGui::BeginListBox("##ListBox", vWinSize))
	{
		for (size_t i = 0; i < m_vecList.size(); ++i)
		{
			bool bSelected = m_SelectedIdx == i;

			if (ImGui::Selectable(m_vecList[i].c_str(), bSelected))
			{
				m_SelectedIdx = (int)i;
			}
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (m_Callback)
					m_Callback();
				if (m_Inst && m_Delegate)
					(m_Inst->*m_Delegate)((DWORD_PTR)&m_vecList[m_SelectedIdx]);
			}
		}
		ImGui::EndListBox();
	}
}

void ListUI::Deactivate()
{
	m_vecList.clear();
	m_SelectedIdx = -1;
	
	m_Callback = nullptr;
	m_Inst = nullptr;
	m_Delegate = nullptr;

	SetModal(false);
}
