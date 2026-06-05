#include "pch.h"
#include "CDevice.h"

#include "CConstBuffer.h"

CDevice::CDevice()
	: m_hMainWnd(nullptr),
	  m_RenderResolution(),
	  m_CB{},
	  m_Sampler{}
{
}

CDevice::~CDevice()
{
	Safe_Del_Array(m_CB);
}

int CDevice::Init(HWND _hWnd, POINT _Resolution)
{
	m_hMainWnd = _hWnd;
	m_RenderResolution = _Resolution;

	g_GlobalData.vResolution = GetRenderResolution();

	UINT iFlag = 0;
#ifdef _DEBUG
	iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL level = D3D_FEATURE_LEVEL_11_0;

	if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
		iFlag, nullptr, 0, D3D11_SDK_VERSION,
		m_Device.GetAddressOf(), &level, m_Context.GetAddressOf())))
	{
		return E_FAIL;
	}

	// Create SwapChain
	if (FAILED(CreateSwapChain()))
		return E_FAIL;

	// View
	if (FAILED(CreateView()))
		return E_FAIL;

	// Set Viewport
	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (FLOAT)m_RenderResolution.x;
	viewport.Height = (FLOAT)m_RenderResolution.y;

	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;

	m_Context->RSSetViewports(1, &viewport);

	// Create necessary const buffer
	if (FAILED(CreateConstBuffer()))
		return E_FAIL;

	// For texture
	if (FAILED(CreateSamplerState()))
		return E_FAIL;

	if (FAILED(CreateRasterizerState()))
		return E_FAIL;

	if (FAILED(CreateDepthStencilState()))
		return E_FAIL;

	if (FAILED(CreateBlendState()))
		return E_FAIL;

	return S_OK;
}

void CDevice::ClearTarget(float(&_ArrColor)[4])
{
	m_Context->ClearRenderTargetView(m_RTTex->GetRTV().Get(), _ArrColor);
	m_Context->ClearDepthStencilView(m_DSTex->GetDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

int CDevice::CreateSwapChain()
{
	DXGI_SWAP_CHAIN_DESC Desc = {};

	Desc.OutputWindow = m_hMainWnd;
	Desc.Windowed = true;

	Desc.BufferCount = 1;
	Desc.BufferDesc.Width = (UINT)m_RenderResolution.x;
	Desc.BufferDesc.Height = (UINT)m_RenderResolution.y;
	Desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	Desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	Desc.BufferDesc.RefreshRate.Denominator = 1;
	Desc.BufferDesc.RefreshRate.Numerator = 60;
	Desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	Desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	Desc.SampleDesc.Count = 1;
	Desc.SampleDesc.Quality = 0;

	Desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	ComPtr<IDXGIDevice> pDXGIDevice = nullptr;
	ComPtr<IDXGIAdapter> pAdapter = nullptr;
	ComPtr<IDXGIFactory> pFactory = nullptr;

	m_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)pDXGIDevice.GetAddressOf());
	pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)pAdapter.GetAddressOf());
	pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)pFactory.GetAddressOf());

	if (FAILED(pFactory->CreateSwapChain(m_Device.Get(), &Desc, m_SwapChain.GetAddressOf())))
		return E_FAIL;

	return S_OK;
}

int CDevice::CreateView()
{
	// RenderTarget Texture, RenderTargetView
	ComPtr<ID3D11Texture2D> tex2D = nullptr;
	m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)tex2D.GetAddressOf());
	
	m_RTTex = CAssetMgr::GetInst()->CreateTexture(L"RenderTargetTex", tex2D);

	// Create texture for DepthStencil
	m_DSTex = CAssetMgr::GetInst()->CreateTexture(L"DepthStencilTex", (UINT)m_RenderResolution.x,
		(UINT)m_RenderResolution.y, DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_BIND_DEPTH_STENCIL);

	// Output Merge Set Render Targets
	m_Context->OMSetRenderTargets(1, m_RTTex->GetRTV().GetAddressOf(), m_DSTex->GetDSV().Get());

	return S_OK;
}

int CDevice::CreateConstBuffer()
{
	m_CB[(UINT)CB_TYPE::TRANSFORM] = new CConstBuffer;
	m_CB[(UINT)CB_TYPE::TRANSFORM]->Create(sizeof(TTransform), CB_TYPE::TRANSFORM);

	m_CB[(UINT)CB_TYPE::MATERIAL] = new CConstBuffer;
	m_CB[(UINT)CB_TYPE::MATERIAL]->Create(sizeof(TMaterialConst), CB_TYPE::MATERIAL);

	m_CB[(UINT)CB_TYPE::ANIMATION] = new CConstBuffer;
	m_CB[(UINT)CB_TYPE::ANIMATION]->Create(sizeof(TAnim2DInfo), CB_TYPE::ANIMATION);

	m_CB[(UINT)CB_TYPE::GLOBAL] = new CConstBuffer;
	m_CB[(UINT)CB_TYPE::GLOBAL]->Create(sizeof(TGlobalData), CB_TYPE::GLOBAL);

	return S_OK;
}

int CDevice::CreateSamplerState()
{
	D3D11_SAMPLER_DESC Desc[2] = {};

	Desc[0].AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	Desc[0].AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	Desc[0].AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	Desc[0].Filter = D3D11_FILTER_ANISOTROPIC;

	if (FAILED(DEVICE->CreateSamplerState(Desc, m_Sampler[0].GetAddressOf())))
		return E_FAIL;

	CONTEXT->VSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
	CONTEXT->HSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
	CONTEXT->DSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
	CONTEXT->GSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
	CONTEXT->PSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
	CONTEXT->CSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());

	Desc[1].AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	Desc[1].AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	Desc[1].AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	Desc[1].Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;

	if (FAILED(DEVICE->CreateSamplerState(Desc + 1, m_Sampler[1].GetAddressOf())))
		return E_FAIL;

	CONTEXT->VSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
	CONTEXT->HSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
	CONTEXT->DSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
	CONTEXT->GSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
	CONTEXT->PSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
	CONTEXT->CSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());

	return 0;
}

int CDevice::CreateRasterizerState()
{
	// CULL_BACK is default choice, so set it as nullptr
	m_RS[(UINT)RS_TYPE::CULL_BACK] = nullptr;

	// CULL_FRONT
	D3D11_RASTERIZER_DESC Desc = {};
	Desc.CullMode = D3D11_CULL_FRONT;
	Desc.FillMode = D3D11_FILL_SOLID;
	DEVICE->CreateRasterizerState(&Desc, m_RS[(UINT)RS_TYPE::CULL_FRONT].GetAddressOf());

	// CULL_NONE
	Desc.CullMode = D3D11_CULL_NONE;
	Desc.FillMode = D3D11_FILL_SOLID;
	DEVICE->CreateRasterizerState(&Desc, m_RS[(UINT)RS_TYPE::CULL_NONE].GetAddressOf());

	// WIRE_FRAME
	Desc.CullMode = D3D11_CULL_NONE;
	Desc.FillMode = D3D11_FILL_WIREFRAME;
	DEVICE->CreateRasterizerState(&Desc, m_RS[(UINT)RS_TYPE::WIRE_FRAME].GetAddressOf());

	return S_OK;
}

int CDevice::CreateDepthStencilState()
{
	m_DS[(UINT)DS_TYPE::LESS] = nullptr;

	D3D11_DEPTH_STENCIL_DESC Desc = {};

	// LESS_EQUAL
	Desc.DepthEnable = true;
	Desc.StencilEnable = false;
	Desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;

	DEVICE->CreateDepthStencilState(&Desc, m_DS[(UINT)DS_TYPE::LESS_EQUAL].GetAddressOf());

	// GREATER
	Desc.DepthEnable = true;
	Desc.StencilEnable = false;
	Desc.DepthFunc = D3D11_COMPARISON_GREATER;
	Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;

	DEVICE->CreateDepthStencilState(&Desc, m_DS[(UINT)DS_TYPE::GREATER].GetAddressOf());

	// NO_WRITE
	Desc.DepthEnable = true;
	Desc.StencilEnable = false;
	Desc.DepthFunc = D3D11_COMPARISON_LESS;
	Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

	DEVICE->CreateDepthStencilState(&Desc, m_DS[(UINT)DS_TYPE::NO_WRITE].GetAddressOf());

	// NO_TEST
	Desc.DepthEnable = true;
	Desc.StencilEnable = false;
	Desc.DepthFunc = D3D11_COMPARISON_ALWAYS;
	Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;

	DEVICE->CreateDepthStencilState(&Desc, m_DS[(UINT)DS_TYPE::NO_TEST].GetAddressOf());

	// NO_TEST_NO_WRITE
	Desc.DepthEnable = false;
	Desc.StencilEnable = false;
	Desc.DepthFunc = D3D11_COMPARISON_ALWAYS;
	Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

	DEVICE->CreateDepthStencilState(&Desc, m_DS[(UINT)DS_TYPE::NO_TEST_NO_WRITE].GetAddressOf());

	return S_OK;
}

int CDevice::CreateBlendState()
{
	m_BS[(UINT)BS_TYPE::DEFAULT] = nullptr;

	D3D11_BLEND_DESC Desc = {};

	// ALPHA_BLEND
	Desc.AlphaToCoverageEnable = false;
	Desc.IndependentBlendEnable = false;

	Desc.RenderTarget[0].BlendEnable = true;
	Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	Desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

	if (FAILED(DEVICE->CreateBlendState(&Desc, m_BS[(UINT)BS_TYPE::ALPHA_BLEND].GetAddressOf())))
		return E_FAIL;

	// ALPHA_BLEND_COVERAGE
	Desc.AlphaToCoverageEnable = true;
	Desc.IndependentBlendEnable = false;

	Desc.RenderTarget[0].BlendEnable = true;
	Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	Desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

	if (FAILED(DEVICE->CreateBlendState(&Desc, m_BS[(UINT)BS_TYPE::ALPHA_BLEND_COVERAGE].GetAddressOf())))
		return E_FAIL;
	
	// ONE_ONE
	Desc.AlphaToCoverageEnable = false;
	Desc.IndependentBlendEnable = false;

	Desc.RenderTarget[0].BlendEnable = true;
	Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	Desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

	if (FAILED(DEVICE->CreateBlendState(&Desc, m_BS[(UINT)BS_TYPE::ONE_ONE].GetAddressOf())))
		return E_FAIL;

	return S_OK;
}
