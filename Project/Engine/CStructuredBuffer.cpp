#include "pch.h"
#include "CStructuredBuffer.h"

#include "CDevice.h"

CStructuredBuffer::CStructuredBuffer()
    : m_SBDesc{}, m_ElementSize(0), m_ElementCount(0), 
    m_Type(SB_TYPE::SRV_ONLY), m_SysMemMove(false),
    m_SRV_Register(0), m_UAV_Register(0)
{
}

CStructuredBuffer::~CStructuredBuffer()
{
}

int CStructuredBuffer::Create(UINT elementSize, UINT elementCount, SB_TYPE type, bool bSysMemMove, void* initialData)
{
    m_SB_Main = nullptr;
    m_SB_Write = nullptr;
    m_SB_Read = nullptr;

    m_SRV = nullptr;
    m_UAV = nullptr;

    m_Type = type;
    m_SysMemMove = bSysMemMove;

    assert(!(elementSize % 16));

    m_ElementSize = elementSize;
    m_ElementCount = elementCount;

    m_SBDesc.ByteWidth = elementSize * elementCount;

    m_SBDesc.Usage = D3D11_USAGE_DEFAULT;
    m_SBDesc.CPUAccessFlags = 0;

    if (m_Type == SB_TYPE::SRV_ONLY)
        m_SBDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    else
        m_SBDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;

    m_SBDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    m_SBDesc.StructureByteStride = m_ElementSize;

    HRESULT hr = E_FAIL;

    if (initialData)
    {
        D3D11_SUBRESOURCE_DATA tSub = {};
        tSub.pSysMem = initialData;
        hr = DEVICE->CreateBuffer(&m_SBDesc, &tSub, m_SB_Main.GetAddressOf());
    }
    else
        hr = DEVICE->CreateBuffer(&m_SBDesc, nullptr, m_SB_Main.GetAddressOf());

    if (FAILED(hr))
        return E_FAIL;

    D3D11_SHADER_RESOURCE_VIEW_DESC tSRVDesc = {};
    tSRVDesc.ViewDimension = D3D_SRV_DIMENSION_BUFFER;
    tSRVDesc.BufferEx.NumElements = m_ElementCount;

    hr = DEVICE->CreateShaderResourceView(m_SB_Main.Get(), &tSRVDesc, m_SRV.GetAddressOf());
    if (FAILED(hr)) 
        return E_FAIL;

    if (m_SBDesc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
    {
        D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc = {};
        UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
        UAVDesc.Buffer.NumElements = m_ElementCount;
        hr = DEVICE->CreateUnorderedAccessView(m_SB_Main.Get(), &UAVDesc, m_UAV.GetAddressOf());
        if (FAILED(hr))
            return E_FAIL;
    }

    if (m_SysMemMove)
    {
        D3D11_BUFFER_DESC WriteDesc = m_SBDesc;
        WriteDesc.Usage = D3D11_USAGE_DYNAMIC;
        WriteDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        WriteDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        hr = DEVICE->CreateBuffer(&WriteDesc, nullptr, m_SB_Write.GetAddressOf());
        if (FAILED(hr))
            return E_FAIL;

        D3D11_BUFFER_DESC ReadDesc = m_SBDesc;
        ReadDesc.Usage = D3D11_USAGE_DEFAULT;
        ReadDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
        ReadDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        hr = DEVICE->CreateBuffer(&ReadDesc, nullptr, m_SB_Read.GetAddressOf());
        if (FAILED(hr))
            return E_FAIL;
    }

    return hr;
}


void CStructuredBuffer::SetData(void* pData, UINT elementCount)
{
    if (!elementCount)
        elementCount = m_ElementCount;

    D3D11_MAPPED_SUBRESOURCE tSub = {};
    CONTEXT->Map(m_SB_Write.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);
    memcpy(tSub.pData, pData, elementCount * m_ElementSize);
    CONTEXT->Unmap(m_SB_Write.Get(), 0);

    // Write -> Main
    CONTEXT->CopyResource(m_SB_Main.Get(), m_SB_Write.Get());
}

void CStructuredBuffer::GetData(void* pDst, UINT elementCount)
{
    if (!elementCount)
        elementCount = m_ElementCount;

    // Main -> Read
    CONTEXT->CopyResource(m_SB_Read.Get(), m_SB_Main.Get());

    D3D11_MAPPED_SUBRESOURCE tSub = {};
    CONTEXT->Map(m_SB_Read.Get(), 0, D3D11_MAP_READ, 0, &tSub);
    memcpy(pDst, tSub.pData, elementCount * m_ElementSize);
    CONTEXT->Unmap(m_SB_Read.Get(), 0);
}

void CStructuredBuffer::Bind(UINT registerSlot)
{
    m_SRV_Register = registerSlot;
    CONTEXT->VSSetShaderResources(registerSlot, 1, m_SRV.GetAddressOf());
    CONTEXT->HSSetShaderResources(registerSlot, 1, m_SRV.GetAddressOf());
    CONTEXT->DSSetShaderResources(registerSlot, 1, m_SRV.GetAddressOf());
    CONTEXT->GSSetShaderResources(registerSlot, 1, m_SRV.GetAddressOf());
    CONTEXT->PSSetShaderResources(registerSlot, 1, m_SRV.GetAddressOf());
}

void CStructuredBuffer::Bind_CS_SRV(UINT registerSlot)
{
    m_SRV_Register = registerSlot;
    CONTEXT->CSSetShaderResources(registerSlot, 1, m_SRV.GetAddressOf());
}

void CStructuredBuffer::Bind_CS_UAV(UINT registerSlot)
{
    m_UAV_Register = registerSlot;
    UINT i = -1;
    CONTEXT->CSSetUnorderedAccessViews(registerSlot, 1, m_UAV.GetAddressOf(), &i);
}

void CStructuredBuffer::Clear(UINT registerSlot)
{
    ID3D11ShaderResourceView* pSRV = nullptr;
    CONTEXT->VSSetShaderResources(registerSlot, 1, &pSRV);
    CONTEXT->HSSetShaderResources(registerSlot, 1, &pSRV);
    CONTEXT->DSSetShaderResources(registerSlot, 1, &pSRV);
    CONTEXT->GSSetShaderResources(registerSlot, 1, &pSRV);
    CONTEXT->PSSetShaderResources(registerSlot, 1, &pSRV);
}

void CStructuredBuffer::Clear_CS_SRV(UINT registerSlot)
{
    ID3D11ShaderResourceView* pSRV = nullptr;
    CONTEXT->CSSetShaderResources(registerSlot, 1, &pSRV);
}

void CStructuredBuffer::Clear_CS_UAV(UINT registerSlot)
{
    UINT i = -1;
    ID3D11UnorderedAccessView* pUAV = nullptr;
    CONTEXT->CSSetUnorderedAccessViews(registerSlot, 1, &pUAV, &i);
}
