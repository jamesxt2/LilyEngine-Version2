#include "pch.h"
#include "CComputeShader.h"

#include "CDevice.h"
#include "CConstBuffer.h"

CComputeShader::CComputeShader(UINT threadPerGroupX, UINT threadPerGroupY, UINT threadPerGroupZ)
	: CShader(ASSET_TYPE::COMPUTE_SHADER),
	m_ThreadPerGroupX(threadPerGroupX),
	m_ThreadPerGroupY(threadPerGroupY),
	m_ThreadPerGroupZ(threadPerGroupZ),
	m_GroupX(1),
	m_GroupY(1),
	m_GroupZ(1)
{
}

CComputeShader::~CComputeShader()
{
}

int CComputeShader::CreateComputeShader(const std::wstring& filePath, const std::string& funcName)
{
	if (FAILED(D3DCompileFromFile(filePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		funcName.c_str(), "cs_5_0", D3DCOMPILE_DEBUG, 0, m_CSBlob.GetAddressOf(),
		m_ErrBlob.GetAddressOf())))
	{
		if (m_ErrBlob != nullptr)
		{
			MessageBoxA(nullptr, (char*)m_ErrBlob->GetBufferPointer()
				, "Compute Shader Compile Error!", MB_OK);
		}
		else
		{
			MessageBox(nullptr, L"Invalid File Path!"
				, L"Compute Shader Compile Error!", MB_OK);
		}

		return E_FAIL;
	}

	if (FAILED(DEVICE->CreateComputeShader(m_CSBlob->GetBufferPointer(),
		m_CSBlob->GetBufferSize(),
		nullptr, m_CS.GetAddressOf())))
	{
		return E_FAIL;
	}

	return S_OK;
}

int CComputeShader::Execute()
{
	if (FAILED(Bind())) return E_FAIL;

	CalculateGroupNum();

	static CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::MATERIAL);
	pCB->SetData(&m_Const);
	pCB->Bind_CS();

	CONTEXT->CSSetShader(m_CS.Get(), nullptr, 0);
	CONTEXT->Dispatch(m_GroupX, m_GroupY, m_GroupZ);

	Clear();

	return S_OK;
}
