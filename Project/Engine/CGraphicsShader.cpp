#include "pch.h"
#include "CGraphicsShader.h"

#include "CDevice.h"

CGraphicsShader::CGraphicsShader()
	: CShader(ASSET_TYPE::GRAPHICS_SHADER),
	m_Topology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST),
	m_RSType(RS_TYPE::CULL_BACK), m_DSType(DS_TYPE::LESS), m_BSType(BS_TYPE::DEFAULT),
	m_Domain(SHADER_DOMAIN::DOMAIN_MASKED)
{
}

CGraphicsShader::~CGraphicsShader()
{
}

int CGraphicsShader::CreateVertexShader(const std::wstring& filePath, const std::string& funcName)
{
	if (FAILED(D3DCompileFromFile(filePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		funcName.c_str(), "vs_5_0", D3DCOMPILE_DEBUG, 0, m_VSBlob.GetAddressOf(),
		m_ErrBlob.GetAddressOf())))
	{
		if (m_ErrBlob != nullptr)
		{
			MessageBoxA(nullptr, (char*)m_ErrBlob->GetBufferPointer()
				, "Vertex Shader Compile Error!", MB_OK);
		}
		else
		{
			MessageBox(nullptr, L"Invalid File Path!"
				, L"Vertex Shader Compile Error!", MB_OK);
		}

		return E_FAIL;
	}

	if (FAILED(DEVICE->CreateVertexShader(m_VSBlob->GetBufferPointer(),
		m_VSBlob->GetBufferSize(),
		nullptr, m_VS.GetAddressOf())))
	{
		return E_FAIL;
	}

	// Layout
	D3D11_INPUT_ELEMENT_DESC LayoutDesc[3] = {};

	LayoutDesc[0].AlignedByteOffset = 0;
	LayoutDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	LayoutDesc[0].InputSlot = 0;
	LayoutDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	LayoutDesc[0].InstanceDataStepRate = 0;
	LayoutDesc[0].SemanticName = "POSITION";
	LayoutDesc[0].SemanticIndex = 0;

	LayoutDesc[1].AlignedByteOffset = 12;
	LayoutDesc[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	LayoutDesc[1].InputSlot = 0;
	LayoutDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	LayoutDesc[1].InstanceDataStepRate = 0;
	LayoutDesc[1].SemanticName = "COLOR";
	LayoutDesc[1].SemanticIndex = 0;

	LayoutDesc[2].AlignedByteOffset = 28;
	LayoutDesc[2].Format = DXGI_FORMAT_R32G32_FLOAT;
	LayoutDesc[2].InputSlot = 0;
	LayoutDesc[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	LayoutDesc[2].InstanceDataStepRate = 0;
	LayoutDesc[2].SemanticName = "TEXCOORD";
	LayoutDesc[2].SemanticIndex = 0;

	if (FAILED(DEVICE->CreateInputLayout(LayoutDesc, 3,
		m_VSBlob->GetBufferPointer(), m_VSBlob->GetBufferSize(),
		m_Layout.GetAddressOf())))
	{
		return E_FAIL;
	}

	return S_OK;
}

int CGraphicsShader::CreateHullShader(const std::wstring& filePath, const std::string& funcName)
{
	if (FAILED(D3DCompileFromFile(filePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		funcName.c_str(), "hs_5_0", D3DCOMPILE_DEBUG, 0, m_HSBlob.GetAddressOf(),
		m_ErrBlob.GetAddressOf())))
	{
		if (m_ErrBlob != nullptr)
		{
			MessageBoxA(nullptr, (char*)m_ErrBlob->GetBufferPointer()
				, "Hull Shader Compile Error!", MB_OK);
		}
		else
		{
			MessageBox(nullptr, L"Invalid File Path!"
				, L"Hull Shader Compile Error!", MB_OK);
		}

		return E_FAIL;
	}

	if (FAILED(DEVICE->CreateHullShader(m_HSBlob->GetBufferPointer(),
		m_HSBlob->GetBufferSize(),
		nullptr, m_HS.GetAddressOf())))
	{
		return E_FAIL;
	}

	return S_OK;
}

int CGraphicsShader::CreateDomainShader(const std::wstring& filePath, const std::string& funcName)
{
	if (FAILED(D3DCompileFromFile(filePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		funcName.c_str(), "ds_5_0", D3DCOMPILE_DEBUG, 0, m_DSBlob.GetAddressOf(),
		m_ErrBlob.GetAddressOf())))
	{
		if (m_ErrBlob != nullptr)
		{
			MessageBoxA(nullptr, (char*)m_ErrBlob->GetBufferPointer()
				, "Domain Shader Compile Error!", MB_OK);
		}
		else
		{
			MessageBox(nullptr, L"Invalid File Path!"
				, L"Domain Shader Compile Error!", MB_OK);
		}

		return E_FAIL;
	}

	if (FAILED(DEVICE->CreateDomainShader(m_DSBlob->GetBufferPointer(),
		m_DSBlob->GetBufferSize(),
		nullptr, m_DS.GetAddressOf())))
	{
		return E_FAIL;
	}

	return S_OK;
}

int CGraphicsShader::CreateGeometryShader(const std::wstring& filePath, const std::string& funcName)
{
	if (FAILED(D3DCompileFromFile(filePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		funcName.c_str(), "gs_5_0", D3DCOMPILE_DEBUG, 0, m_GSBlob.GetAddressOf(),
		m_ErrBlob.GetAddressOf())))
	{
		if (m_ErrBlob != nullptr)
		{
			MessageBoxA(nullptr, (char*)m_ErrBlob->GetBufferPointer()
				, "Geometry Shader Compile Error!", MB_OK);
		}
		else
		{
			MessageBox(nullptr, L"Invalid File Path!"
				, L"Geometry Shader Compile Error!", MB_OK);
		}

		return E_FAIL;
	}

	if (FAILED(DEVICE->CreateGeometryShader(m_GSBlob->GetBufferPointer(),
		m_GSBlob->GetBufferSize(),
		nullptr, m_GS.GetAddressOf())))
	{
		return E_FAIL;
	}

	return S_OK;
}

int CGraphicsShader::CreatePixelShader(const std::wstring& filePath, const std::string& funcName)
{
	if (FAILED(D3DCompileFromFile(filePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		funcName.c_str(), "ps_5_0", D3DCOMPILE_DEBUG, 0, m_PSBlob.GetAddressOf(),
		m_ErrBlob.GetAddressOf())))
	{
		if (m_ErrBlob != nullptr)
		{
			MessageBoxA(nullptr, (char*)m_ErrBlob->GetBufferPointer()
				, "Pixel Shader Compile Error!", MB_OK);
		}
		else
		{
			MessageBox(nullptr, L"Invalid File Path!"
				, L"Pixel Shader Compile Error!", MB_OK);
		}

		return E_FAIL;
	}

	if (FAILED(DEVICE->CreatePixelShader(m_PSBlob->GetBufferPointer(),
		m_PSBlob->GetBufferSize(),
		nullptr, m_PS.GetAddressOf())))
	{
		return E_FAIL;
	}

	return S_OK;
}

int CGraphicsShader::Bind()
{
	CONTEXT->IASetInputLayout(m_Layout.Get());
	CONTEXT->IASetPrimitiveTopology(m_Topology);

	CONTEXT->VSSetShader(m_VS.Get(), nullptr, 0);
	CONTEXT->HSSetShader(m_HS.Get(), nullptr, 0);
	CONTEXT->DSSetShader(m_DS.Get(), nullptr, 0);
	CONTEXT->GSSetShader(m_GS.Get(), nullptr, 0);

	CONTEXT->RSSetState(CDevice::GetInst()->GetRasterizerState(m_RSType).Get());

	CONTEXT->PSSetShader(m_PS.Get(), nullptr, 0);

	CONTEXT->OMSetDepthStencilState(CDevice::GetInst()->GetDS(m_DSType).Get(), 0);
	CONTEXT->OMSetBlendState(CDevice::GetInst()->GetBS(m_BSType).Get(), Vec4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

	return S_OK;
}
