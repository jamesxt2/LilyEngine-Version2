#pragma once
#include "singleton.h"

class CCamera;
class CLight2D;
class CStructuredBuffer;

class CRenderMgr : public CSingleton<CRenderMgr>
{
	SINGLE(CRenderMgr)

public:
	void Init();
	void Tick();
	void Render();

	void RegisterCamera(CCamera* camera, int priority);
	int RegisterLight2D(CLight2D* light)
	{
		m_vecLight2D.push_back(light);
		return (int)m_vecLight2D.size() - 1;
	}

private:
	void DataBind();
	void DataClear();

private:
	std::vector<CCamera*> m_vecCam;
	std::vector<CLight2D*> m_vecLight2D;
	CStructuredBuffer* m_Light2DBuffer;
};

