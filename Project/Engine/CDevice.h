#pragma once

class CConstBuffer;

class CDevice : public CSingleton<CDevice>
{
	SINGLE(CDevice)

public:
	int Init(HWND _hWnd, POINT _Resolution);
	void ClearTarget(float(&_ArrColor)[4]);
	void Present() { m_SwapChain->Present(0, 0); }

private:
	int CreateSwapChain();
	int CreateView();
	int CreateConstBuffer();
	int CreateSamplerState();
	int CreateRasterizerState();

private:
	HWND							m_hMainWnd;
	POINT							m_RenderResolution;

	ComPtr<ID3D11Device>			m_Device; // GPU memory distribute, DX11 object creation
	ComPtr<ID3D11DeviceContext>		m_Context; // GPU Render Command
	ComPtr<IDXGISwapChain>			m_SwapChain; // Create view, upload on screen

	ComPtr<ID3D11Texture2D>			m_RenderTargetTex;
	ComPtr<ID3D11RenderTargetView>	m_RTV;

	ComPtr<ID3D11Texture2D>			m_DepthStencilTex;
	ComPtr<ID3D11DepthStencilView>	m_DSV;

	ComPtr<ID3D11SamplerState>		m_Sampler[2];
	ComPtr<ID3D11RasterizerState>	m_RS[(UINT)RS_TYPE::END];

	CConstBuffer*					m_CB[(UINT)CB_TYPE::END];

public:
	inline ID3D11Device* GetDevice() const { return m_Device.Get(); }
	inline ID3D11DeviceContext* GetContext() const { return m_Context.Get(); }
	inline CConstBuffer* GetConstBuffer(CB_TYPE type) { return m_CB[(UINT)type]; }
	inline ComPtr<ID3D11RasterizerState> GetRasterizerState(RS_TYPE type) const { return m_RS[(UINT)type]; }
	inline Vec2 GetRenderResolution() const { return Vec2((float)m_RenderResolution.x, (float)m_RenderResolution.y); }
	inline float GetAspectRatio() const { return (float)m_RenderResolution.x / (float)m_RenderResolution.y; }
};

