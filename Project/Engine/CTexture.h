#pragma once
#include "CAsset.h"

class CTexture : public CAsset
{
public:
	CTexture();
	~CTexture();

	void Bind(int registerSlot);
	static void Clear(int registerSlot);

private:
	virtual int Load(const std::wstring& filePath) override;
	virtual int Save(const std::wstring& filePath) override;

private:
	ScratchImage						m_Image;
	ComPtr<ID3D11Texture2D>				m_Tex2D;
	ComPtr<ID3D11ShaderResourceView>	m_SRV;

	D3D11_TEXTURE2D_DESC				m_Desc;

public:
	inline UINT GetWidth() const { return m_Desc.Width; }
	inline UINT GetHeight() const { return m_Desc.Height; }
};

