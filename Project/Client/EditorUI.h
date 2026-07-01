#pragma once

class EditorUI
{
public:
	EditorUI(const std::string& name, const std::string& ID);
	virtual ~EditorUI();

	virtual void Tick();
	virtual void Render_Tick() = 0;

	virtual void Activate() {}
	virtual void Deactivate() {}

	void SetFocus();

private:
	std::string m_Name;
	const std::string m_ID;

	EditorUI* m_ParentUI;
	std::vector<EditorUI*> m_vecChildUI;

	ImVec2 m_ChildSize;

	bool m_Active;
	bool m_Separate;
	bool m_Modal;

public:
	inline void SetName(const std::string& name) { m_Name = name; }
	inline const std::string& GetName() const { return m_Name; }

	inline EditorUI* GetParentUI() const { return m_ParentUI; }
	inline void AddChildUI(EditorUI* child) 
	{ 
		child->m_ParentUI = this;
		m_vecChildUI.push_back(child); 
	}
	inline const std::vector<EditorUI*>& GetChildUI() { return m_vecChildUI; }
	inline bool IsRootUI() { return !m_ParentUI; }

	inline void SetChildSize(ImVec2 size) { m_ChildSize = size; }

	void SetActive(bool active);
	inline bool IsActive() const { return m_Active; }
	inline void SetSeparate(bool separate) { m_Separate = separate; }
	inline void SetModal(bool modal) { m_Modal = modal; }
};

