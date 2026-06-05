#pragma once
#include "CAsset.h"

class CTexture : public CAsset
{
public:
	CTexture();
	~CTexture();
	friend class CAssetMgr;

	void Bind(int registerSlot);
	void Bind_CS_SRV(int registerSlot);
	void Bind_CS_UAV(int registerSlot);
	static void Clear(int registerSlot);
	static void Clear_CS_SRV(int registerSlot);
	static void Clear_CS_UAV(int registerSlot);

private:
	virtual int Load(const std::wstring& filePath) override;
	virtual int Save(const std::wstring& filePath) override;

	int Create(UINT width, UINT height, DXGI_FORMAT pixelFormat, UINT bindFlag, D3D11_USAGE usage);
	int Create(ComPtr<ID3D11Texture2D> tex2D);

private:
	ScratchImage						m_Image;
	ComPtr<ID3D11Texture2D>				m_Tex2D;

	ComPtr<ID3D11RenderTargetView>		m_RTV;
	ComPtr<ID3D11DepthStencilView>		m_DSV;
	ComPtr<ID3D11ShaderResourceView>	m_SRV;
	ComPtr<ID3D11UnorderedAccessView>	m_UAV;

	D3D11_TEXTURE2D_DESC				m_Desc;

public:
	inline UINT GetWidth() const { return m_Desc.Width; }
	inline UINT GetHeight() const { return m_Desc.Height; }

	inline ComPtr<ID3D11RenderTargetView> GetRTV() const { return m_RTV; }
	inline ComPtr<ID3D11DepthStencilView> GetDSV() const { return m_DSV; }
	inline ComPtr<ID3D11ShaderResourceView> GetSRV() const { return m_SRV; }
	inline ComPtr<ID3D11UnorderedAccessView> GetUAV() const { return m_UAV; }
};

