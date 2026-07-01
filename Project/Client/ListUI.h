#pragma once
#include "EditorUI.h"

#include "CImGuiMgr.h"

class ListUI : public EditorUI
{
public:
	ListUI();
	~ListUI();

	virtual void Render_Tick() override;

	virtual void Deactivate() override;

	inline void RegisterDBClickCallback(UI_CALLBACK callback) { m_Callback = callback; }
	inline void RegisterDBClickDelegate(EditorUI* inst, UI_DELEGATE_PARAM_1 func)
	{
		m_Inst = inst;
		m_Delegate = func;
	}

private:
	std::vector<std::string> m_vecList;
	int m_SelectedIdx;

	UI_CALLBACK m_Callback;
	UI_DELEGATE_PARAM_1 m_Delegate;

	EditorUI* m_Inst;

public:
	inline void AddItem(const std::string& item) { m_vecList.push_back(item); }
	inline void AddItem(const std::vector<std::string>& vecItem)
	{
		m_vecList.insert(m_vecList.end(), vecItem.begin(), vecItem.end());
	}

	inline const std::string& GetSelected() const { return m_vecList[m_SelectedIdx]; }
};

