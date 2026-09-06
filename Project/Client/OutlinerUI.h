#pragma once
#include "EditorUI.h"

class TreeUI;
class TreeNode;

class OutlinerUI : public EditorUI
{
public:
	OutlinerUI();
	~OutlinerUI();

	void RenewGameObject();

	virtual void Render_Tick() override;

private:
	void AddGameObject(TreeNode* node, CGameObject* object);
	UINT SelectGameObject(DWORD_PTR data);

private:
	TreeUI* m_Tree;
};

