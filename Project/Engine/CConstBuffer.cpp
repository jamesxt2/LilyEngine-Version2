#include "pch.h"
#include "CConstBuffer.h"

#include "CDevice.h"

CConstBuffer::CConstBuffer()
	: m_CBDesc{}, m_Type(CB_TYPE::END)
{
}

CConstBuffer::~CConstBuffer()
{
}

int CConstBuffer::Create(size_t bufferSize, CB_TYPE type)
{
	m_Type = type;

	m_CBDesc.ByteWidth = bufferSize;
	m_CBDesc.MiscFlags = 0;
	m_CBDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	m_CBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_CBDesc.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(DEVICE->CreateBuffer(&m_CBDesc, nullptr, m_CB.GetAddressOf())))
		return E_FAIL;
}

void CConstBuffer::SetData(void* pData)
{
	D3D11_MAPPED_SUBRESOURCE tSub = {};
	CONTEXT->Map(m_CB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);

	memcpy(tSub.pData, pData, m_CBDesc.ByteWidth);

	CONTEXT->Unmap(m_CB.Get(), 0);
}

void CConstBuffer::Bind()
{
	CONTEXT->VSSetConstantBuffers((UINT)m_Type, 1, m_CB.GetAddressOf());
}
