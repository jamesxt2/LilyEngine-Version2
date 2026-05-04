#pragma once
#include "singleton.h"

class CCamera;

class CRenderMgr : public CSingleton<CRenderMgr>
{
	SINGLE(CRenderMgr)

public:
	void Init();
	void Tick();
	void Render();

	void RegisterCamera(CCamera* camera, int priority);

private:
	std::vector<CCamera*> m_vecCam;
};

