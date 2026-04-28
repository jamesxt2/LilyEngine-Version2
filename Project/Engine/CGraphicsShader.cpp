#include "pch.h"
#include "CGraphicsShader.h"

#include "CDevice.h"

CGraphicsShader::CGraphicsShader()
	: CShader(ASSET_TYPE::GRAPHICS_SHADER),
	m_Topology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
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
	D3D11_INPUT_ELEMENT_DESC LayoutDesc[2] = {};

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

	if (FAILED(DEVICE->CreateInputLayout(LayoutDesc, 2,
		m_VSBlob->GetBufferPointer(), m_VSBlob->GetBufferSize(),
		m_Layout.GetAddressOf())))
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
				, L"Vertex Shader Compile Error!", MB_OK);
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

void CGraphicsShader::Bind()
{
	CONTEXT->IASetInputLayout(m_Layout.Get());
	CONTEXT->IASetPrimitiveTopology(m_Topology);

	CONTEXT->VSSetShader(m_VS.Get(), nullptr, 0);
	CONTEXT->PSSetShader(m_PS.Get(), nullptr, 0);
}
