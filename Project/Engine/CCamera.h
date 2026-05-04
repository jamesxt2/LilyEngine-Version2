#pragma once
#include "CComponent.h"

class CCamera : public CComponent
{
public:
	CCamera();
	~CCamera();

	virtual void FinalTick() override;
	void Render();

private:
	int				m_CamPriority;
	float			m_Far;

	Matrix			m_matView;
	Matrix			m_matProj;

public:
	void SetCameraPriority(int priority);
};

