#include "pch.h"
#include "CRenderMgr.h"

#include "CDevice.h"
#include "CCamera.h"
#include "CStructuredBuffer.h"
#include "CLight2D.h"
#include "CConstBuffer.h"

CRenderMgr::CRenderMgr()
	: m_Light2DBuffer(nullptr)
{
	m_Light2DBuffer = new CStructuredBuffer;
}

CRenderMgr::~CRenderMgr()
{
	delete m_Light2DBuffer;
}

void CRenderMgr::Init()
{

}

void CRenderMgr::Tick()
{

}

void CRenderMgr::Render()
{
	DataBind();

	// Target Clear
	float ClearColor[4] = { 0.3f, 0.3f, 0.3f, 1.f };
	CDevice::GetInst()->ClearTarget(ClearColor);

	for (size_t i = 0; i < m_vecCam.size(); ++i)
	{
		m_vecCam[i]->Render();
	}

	DataClear();
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

void CRenderMgr::DataBind()
{
	g_GlobalData.Light2DCount = (int)m_vecLight2D.size();

	static CConstBuffer* globalBuffer = CDevice::GetInst()->GetConstBuffer(CB_TYPE::GLOBAL);
	globalBuffer->SetData(&g_GlobalData);
	globalBuffer->Bind();

	if (m_Light2DBuffer->GetElementCount() < m_vecLight2D.size())
		m_Light2DBuffer->Create(sizeof(TLightInfo), (UINT)m_vecLight2D.size());

	static std::vector<TLightInfo> vecLightInfo;
	for (size_t i = 0; i < m_vecLight2D.size(); ++i)
	{
		vecLightInfo.push_back(m_vecLight2D[i]->GetLightInfo());
	}
	m_Light2DBuffer->SetData(vecLightInfo.data(), (UINT)m_vecLight2D.size());
	m_Light2DBuffer->Bind(15);
}

void CRenderMgr::DataClear()
{
	m_vecLight2D.clear();
}
