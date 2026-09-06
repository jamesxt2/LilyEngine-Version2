#include "pch.h"
#include "CRenderMgr.h"

#include "CDevice.h"
#include "CCamera.h"
#include "CStructuredBuffer.h"
#include "CLight2D.h"
#include "CConstBuffer.h"
#include "CAssetMgr.h"
#include "CLevelMgr.h"

CRenderMgr::CRenderMgr()
	: m_EditorCam(nullptr), m_Light2DBuffer(nullptr)
{
	m_Light2DBuffer = new CStructuredBuffer;
	Render_Func = &CRenderMgr::Render_Play;
}

CRenderMgr::~CRenderMgr()
{
	delete m_Light2DBuffer;
}

void CRenderMgr::Init()
{
	Vec2 vRenderResolution = CDevice::GetInst()->GetRenderResolution();
	m_RenderTargetCopyTex = CAssetMgr::GetInst()->CreateTexture(L"RenderTargetCopyTex", (UINT)vRenderResolution.x,
		(UINT)vRenderResolution.y, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE);
}

void CRenderMgr::Tick()
{

}

void CRenderMgr::Render()
{
	if (!CLevelMgr::GetInst()->GetCurrentLevel()) return;

	// Output Merge Set Render Targets
	Ptr<CTexture> pRTTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"RenderTargetTex");
	Ptr<CTexture> pDSTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"DepthStencilTex");
	CONTEXT->OMSetRenderTargets(1, pRTTex->GetRTV().GetAddressOf(), pDSTex->GetDSV().Get());
	
	DataBind();

	// Target Clear
	float ClearColor[4] = { 0.3f, 0.3f, 0.3f, 1.f };
	CDevice::GetInst()->ClearTarget(ClearColor);

	(this->*Render_Func)();

	DataClear();
}

void CRenderMgr::Render_Play()
{
	for (size_t i = 0; i < m_vecCam.size(); ++i)
	{
		if (m_vecCam[i] == nullptr)
			continue;
		m_vecCam[i]->Render();
	}
}

void CRenderMgr::Render_Editor()
{
	if (m_EditorCam != nullptr)
		m_EditorCam->Render();
}

void CRenderMgr::RegisterCamera(CCamera* camera, int priority)
{
	if (priority >= m_vecCam.size())
		m_vecCam.resize(priority + 1);

	assert(!(m_vecCam[priority] && m_vecCam[priority] != camera));

	m_vecCam[priority] = camera;
}

void CRenderMgr::CopyRenderTarget()
{
	Ptr<CTexture> pRenderTargetTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"RenderTargetTex");
	CONTEXT->CopyResource(m_RenderTargetCopyTex->GetTex2D().Get(), pRenderTargetTex->GetTex2D().Get());
}

void CRenderMgr::DataBind()
{
	g_GlobalData.Light2DCount = (int)m_vecLight2D.size();

	CConstBuffer* globalBuffer = CDevice::GetInst()->GetConstBuffer(CB_TYPE::GLOBAL);
	globalBuffer->SetData(&g_GlobalData);
	globalBuffer->Bind();
	globalBuffer->Bind_CS();

	if (m_Light2DBuffer->GetElementCount() < m_vecLight2D.size())
		m_Light2DBuffer->Create(sizeof(TLightInfo), (UINT)m_vecLight2D.size(), SB_TYPE::SRV_ONLY, true);

	std::vector<TLightInfo> vecLightInfo;
	for (size_t i = 0; i < m_vecLight2D.size(); ++i)
	{
		vecLightInfo.push_back(m_vecLight2D[i]->GetLightInfo());
	}

	if (!vecLightInfo.empty())
	{
		m_Light2DBuffer->SetData(vecLightInfo.data(), (UINT)m_vecLight2D.size());
		m_Light2DBuffer->Bind(15);
	}
}

void CRenderMgr::DataClear()
{
	m_vecLight2D.clear();
}