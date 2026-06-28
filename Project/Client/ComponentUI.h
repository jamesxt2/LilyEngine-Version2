#pragma once
#include "EditorUI.h"

#include <Engine/CGameObject.h>

class ComponentUI : public EditorUI
{
public:
	ComponentUI(const std::string& name, const std::string& ID, COMPONENT_TYPE type);
	~ComponentUI();

	virtual void Render_Tick() = 0;

protected:
	void Render_Title();

private:
	CGameObject* m_TargetObject;
	const COMPONENT_TYPE m_Type;

public:
	void SetTargetObject(CGameObject* object);
	inline CGameObject* GetTargetObject() const { return m_TargetObject; }

	inline COMPONENT_TYPE GetComponentType() const { return m_Type; }
};

