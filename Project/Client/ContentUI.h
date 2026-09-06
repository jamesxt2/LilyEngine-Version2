#pragma once
#include "EditorUI.h"

class TreeUI;

class ContentUI : public EditorUI
{
public:
	ContentUI();
	~ContentUI();

	void RenewContent();

	virtual void Render_Tick() override;

private:
	UINT SelectAsset(DWORD_PTR data);

private:
	TreeUI* m_Tree;
};

