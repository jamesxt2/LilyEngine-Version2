#pragma once
#include <Engine\CScript.h>

class CEditorCameraScript : public CScript
{
public:
	CEditorCameraScript();
	~CEditorCameraScript();
	CLONE(CEditorCameraScript)

	virtual void Tick() override;

private:
	void MoveByPerspective();
	void MoveByOrthographic();

private:
	float m_Speed;
};

