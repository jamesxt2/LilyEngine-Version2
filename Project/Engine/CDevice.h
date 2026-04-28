#pragma once

class CConstBuffer;

class CDevice : public CSingleton<CDevice>
{
	SINGLE(CDevice)

public:
	int Init(HWND _hWnd, POINT _Resolution);
	void ClearTarget(float(&_ArrColor)[4]);
	void Present() { m_SwapChain->Present(0, 0); }

	ID3D11Device* GetDevice() { return m_Device.Get(); }
	ID3D11DeviceContext* GetContext() { return m_Context.Get(); }

private:
	int CreateSwapChain();
	int CreateView();
	int CreateConstBuffer();

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

	CConstBuffer*					m_CB[(UINT)CB_TYPE::END];

public:
	inline CConstBuffer* GetConstBuffer(CB_TYPE type) { return m_CB[(UINT)type]; }
};

