#pragma once
#include <Engine\singleton.h>

typedef void (*UI_CALLBACK)(void);

class EditorUI;
typedef UINT(EditorUI::* UI_DELEGATE)(void);
typedef UINT(EditorUI::* UI_DELEGATE_PARAM_1)(DWORD_PTR);
typedef UINT(EditorUI::* UI_DELEGATE_PARAM_2)(DWORD_PTR, DWORD_PTR);

class CImGuiMgr : public CSingleton<CImGuiMgr>
{
	SINGLE(CImGuiMgr)

public:
	int Init(HWND _hWnd);
	void Tick();

	template<typename T>
	T* FindEditorUI(const std::string& name);

private:
	void CreateEditorUI();

private:
	HWND m_hMainWnd;
	std::map<std::string, EditorUI*> m_mapUI;
	bool m_bDemo;
};

template<typename T>
inline T* CImGuiMgr::FindEditorUI(const std::string& name)
{
	std::map<std::string, EditorUI*>::iterator iter = m_mapUI.find(name);

	if (iter == m_mapUI.end())
		return nullptr;

	T* pUI = dynamic_cast<T*>(iter->second);

	return pUI;
}
