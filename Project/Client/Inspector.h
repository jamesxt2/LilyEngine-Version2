#pragma once
#include "EditorUI.h"

class CGameObject;
class ComponentUI;

class Inspector : public EditorUI
{
public:
	Inspector();
	~Inspector();

	virtual void Render_Tick() override;

private:
	void CreateComponentUI();

private:
	CGameObject* m_TargetObject;
	ComponentUI* m_arrCompUI[(UINT)COMPONENT_TYPE::END];

public:
	void SetTargetObject(CGameObject* obj);
};

