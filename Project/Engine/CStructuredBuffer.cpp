#include "pch.h"
#include "CStructuredBuffer.h"

#include "CDevice.h"

CStructuredBuffer::CStructuredBuffer()
    : m_SBDesc{}, m_ElementSize(0), m_ElementCount(0)
{
}

CStructuredBuffer::~CStructuredBuffer()
{
}

int CStructuredBuffer::Create(UINT elementSize, UINT elementCount, void* initialData)
{
    m_SB = nullptr;
    m_SRV = nullptr;

    assert(!(elementSize % 16));

    m_ElementSize = elementSize;
    m_ElementCount = elementCount;

    m_SBDesc.ByteWidth = elementSize * elementCount;

    m_SBDesc.Usage = D3D11_USAGE_DYNAMIC;
    m_SBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    m_SBDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    m_SBDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    m_SBDesc.StructureByteStride = m_ElementSize;

    HRESULT hr = E_FAIL;

    if (initialData)
    {
        D3D11_SUBRESOURCE_DATA tSub = {};
        tSub.pSysMem = initialData;
        hr = DEVICE->CreateBuffer(&m_SBDesc, &tSub, m_SB.GetAddressOf());
    }
    else
        hr = DEVICE->CreateBuffer(&m_SBDesc, nullptr, m_SB.GetAddressOf());

    if (FAILED(hr))
        return E_FAIL;

    D3D11_SHADER_RESOURCE_VIEW_DESC tSRVDesc = {};
    tSRVDesc.ViewDimension = D3D_SRV_DIMENSION_BUFFER;
    tSRVDesc.BufferEx.NumElements = m_ElementCount;

    hr = DEVICE->CreateShaderResourceView(m_SB.Get(), &tSRVDesc, m_SRV.GetAddressOf());

    return hr;
}


void CStructuredBuffer::SetData(void* pData, UINT elementCount)
{
    if (!elementCount)
        elementCount = m_ElementCount;

    D3D11_MAPPED_SUBRESOURCE tSub = {};
    CONTEXT->Map(m_SB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);

    memcpy(tSub.pData, pData, elementCount * m_ElementSize);

    CONTEXT->Unmap(m_SB.Get(), 0);
}

void CStructuredBuffer::Bind(UINT registerSlot)
{
    CONTEXT->VSSetShaderResources(registerSlot, 1, m_SRV.GetAddressOf());
    CONTEXT->HSSetShaderResources(registerSlot, 1, m_SRV.GetAddressOf());
    CONTEXT->DSSetShaderResources(registerSlot, 1, m_SRV.GetAddressOf());
    CONTEXT->GSSetShaderResources(registerSlot, 1, m_SRV.GetAddressOf());
    CONTEXT->PSSetShaderResources(registerSlot, 1, m_SRV.GetAddressOf());
}
