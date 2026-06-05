#pragma once
#include "CShader.h"

class CGraphicsShader : public CShader
{
public:
	CGraphicsShader();
	~CGraphicsShader();

	int CreateVertexShader(const std::wstring& filePath, const std::string& funcName);
	int CreateHullShader(const std::wstring& filePath, const std::string& funcName);
	int CreateDomainShader(const std::wstring& filePath, const std::string& funcName);
	int CreateGeometryShader(const std::wstring& filePath, const std::string& funcName);
	int CreatePixelShader(const std::wstring& filePath, const std::string& funcName);

	virtual int Bind() override;

private:
	ComPtr<ID3DBlob>				m_VSBlob;
	ComPtr<ID3DBlob>				m_HSBlob;
	ComPtr<ID3DBlob>				m_DSBlob;
	ComPtr<ID3DBlob>				m_GSBlob;
	ComPtr<ID3DBlob>				m_PSBlob;

	ComPtr<ID3D11VertexShader>		m_VS;
	ComPtr<ID3D11HullShader>		m_HS;
	ComPtr<ID3D11DomainShader>		m_DS;
	ComPtr<ID3D11GeometryShader>	m_GS;
	ComPtr<ID3D11PixelShader>		m_PS;

	ComPtr<ID3D11InputLayout>		m_Layout;

	D3D11_PRIMITIVE_TOPOLOGY		m_Topology;

	RS_TYPE							m_RSType;
	DS_TYPE							m_DSType;
	BS_TYPE							m_BSType;

	SHADER_DOMAIN					m_Domain;

public:
	inline void SetTopology(D3D11_PRIMITIVE_TOPOLOGY topology) { m_Topology = topology; }
	inline void SetRSType(RS_TYPE type) { m_RSType = type; }
	inline void SetDSType(DS_TYPE type) { m_DSType = type; }
	inline void SetBSType(BS_TYPE type) { m_BSType = type; }

	inline void SetShaderDomain(SHADER_DOMAIN domain) { m_Domain = domain; }
	inline SHADER_DOMAIN GetShaderDomain() const { return m_Domain; }
};

