#pragma once
#include "EditorUI.h"

#include "CImGuiMgr.h"

class TreeNode
{
public:
	TreeNode();
	TreeNode(const std::string& name, DWORD_PTR data);
	~TreeNode();
	friend class TreeUI;

	void Render_Tick();

private:
	TreeUI* m_Owner;
	TreeNode* m_Parent;
	std::vector<TreeNode*> m_vecChildNode;
	std::string m_Name;
	DWORD_PTR m_Data;
	bool m_bFrame;
	bool m_bSelected;

public:
	inline void SetName(const std::string& name) { m_Name = name; }
	inline const std::string& GetName() const { return m_Name; }

	inline void SetFrame(bool frame) { m_bFrame = frame; }

	inline void AddChild(TreeNode* child)
	{
		m_vecChildNode.push_back(child);
		child->m_Parent = this;
	}
	inline const std::vector<TreeNode*>& GetChildNode() const { return m_vecChildNode; }

	inline void SetData(DWORD_PTR data) { m_Data = data; }
	inline DWORD_PTR GetData() const { return m_Data; }
};

class TreeUI : public EditorUI
{
public:
	TreeUI(const std::string& name);
	~TreeUI();
	friend TreeNode;

	virtual void Render_Tick() override;

	TreeNode* AddTreeNode(TreeNode* parent, const std::string& nodeName, DWORD_PTR data = 0);

	void Clear();

private:
	TreeNode* m_RootNode;
	TreeNode* m_SelectedNode;

	bool m_bShowRoot;
	bool m_bShowFileName;

	UI_CALLBACK m_SelectedCallback;
	EditorUI* m_SelectedInst;
	UI_DELEGATE_PARAM_1 m_SelectedDelegate;

private:
	void SetSelectedNode(TreeNode* node);

public:
	inline void ShowRoot(bool show) { m_bShowRoot = show; }
	inline void ShowFileName(bool show) { m_bShowFileName = show; }

	inline void RegisterSelectedCallback(UI_CALLBACK callback) { m_SelectedCallback = callback; }
	inline void RegisterSelectedDelegate(EditorUI* inst, UI_DELEGATE_PARAM_1 func)
	{
		m_SelectedInst = inst;
		m_SelectedDelegate = func;
	}
};

