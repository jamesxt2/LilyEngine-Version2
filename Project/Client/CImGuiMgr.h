#pragma once
#include <Engine\singleton.h>

class EditorUI;

class CImGuiMgr : public CSingleton<CImGuiMgr>
{
	SINGLE(CImGuiMgr)

public:
	int Init(HWND _hWnd);
	void Tick();

private:
	void CreateEditorUI();

private:
	HWND m_hMainWnd;
	std::map<std::string, EditorUI*> m_mapUI;
	bool m_bDemo;
};

