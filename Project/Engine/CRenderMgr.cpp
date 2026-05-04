#include "pch.h"
#include "CRenderMgr.h"

#include "CDevice.h"
#include "CCamera.h"

CRenderMgr::CRenderMgr()
{

}

CRenderMgr::~CRenderMgr()
{

}

void CRenderMgr::Init()
{

}

void CRenderMgr::Tick()
{

}

void CRenderMgr::Render()
{
	// Target Clear
	float ClearColor[4] = { 0.3f, 0.3f, 0.3f, 1.f };
	CDevice::GetInst()->ClearTarget(ClearColor);

	for (size_t i = 0; i < m_vecCam.size(); ++i)
	{
		m_vecCam[i]->Render();
	}

	// Present
	CDevice::GetInst()->Present();
}

void CRenderMgr::RegisterCamera(CCamera* camera, int priority)
{
	for (size_t i = 0; i < m_vecCam.size(); ++i)
	{
		if (m_vecCam[i] == camera)
		{
			if (priority == i)
				return;
			else
				m_vecCam[i] = nullptr;
			break;
		}
	}

	if (priority >= m_vecCam.size())
		m_vecCam.resize(priority + 1);
	m_vecCam[priority] = camera;
}
