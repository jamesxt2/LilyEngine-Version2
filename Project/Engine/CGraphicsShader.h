#pragma once
#include "CShader.h"

class CGraphicsShader : public CShader
{
public:
	CGraphicsShader();
	~CGraphicsShader();

	int CreateVertexShader(const std::wstring& filePath, const std::string& funcName);
	int CreatePixelShader(const std::wstring& filePath, const std::string& funcName);

	virtual void Bind() override;

private:
	ComPtr<ID3DBlob>			m_VSBlob;
	ComPtr<ID3DBlob>			m_PSBlob;

	ComPtr<ID3D11VertexShader>	m_VS;
	ComPtr<ID3D11PixelShader>	m_PS;

	ComPtr<ID3D11InputLayout>	m_Layout;

	D3D11_PRIMITIVE_TOPOLOGY	m_Topology;

public:
	inline void SetTopology(D3D11_PRIMITIVE_TOPOLOGY topology) { m_Topology = topology; }
};

