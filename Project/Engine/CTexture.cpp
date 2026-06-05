#include "pch.h"
#include "CTexture.h"

#include "CDevice.h"

CTexture::CTexture()
    : CAsset(ASSET_TYPE::TEXTURE), m_Desc{}
{
}

CTexture::~CTexture()
{
}

void CTexture::Bind(int registerSlot)
{
    CONTEXT->PSSetShaderResources(registerSlot, 1, m_SRV.GetAddressOf());
}

void CTexture::Bind_CS_SRV(int registerSlot)
{
    CONTEXT->CSSetShaderResources(registerSlot, 1, m_SRV.GetAddressOf());
}

void CTexture::Bind_CS_UAV(int registerSlot)
{
    UINT i = -1;
    CONTEXT->CSSetUnorderedAccessViews(registerSlot, 1, m_UAV.GetAddressOf(), &i);
}

void CTexture::Clear(int registerSlot)
{
    ID3D11ShaderResourceView* pSRV = nullptr;
    CONTEXT->PSSetShaderResources(registerSlot, 1, &pSRV);
}

void CTexture::Clear_CS_SRV(int registerSlot)
{
    ID3D11ShaderResourceView* pSRV = nullptr;
    CONTEXT->CSSetShaderResources(registerSlot, 1, &pSRV);
}

void CTexture::Clear_CS_UAV(int registerSlot)
{
    ID3D11UnorderedAccessView* pUAV = nullptr;
    UINT i = -1;
    CONTEXT->CSSetUnorderedAccessViews(registerSlot, 1, &pUAV, &i);
}

int CTexture::Load(const std::wstring& filePath)
{
    wchar_t Ext[50] = {};
    _wsplitpath_s(filePath.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, Ext, 50);

    std::wstring strExt = Ext;
    HRESULT hr = E_FAIL;
    if (strExt == L".dds" || strExt == L".DDS")
    {
        hr = LoadFromDDSFile(filePath.c_str(), DDS_FLAGS::DDS_FLAGS_NONE, nullptr, m_Image);
    }
    else if (strExt == L".tga" || strExt == L".TGA")
    {
        hr = LoadFromTGAFile(filePath.c_str(), nullptr, m_Image);
    }
    else
    {
        // .jpg, .png, .jpeg, .bmp
        hr = LoadFromWICFile(filePath.c_str(), WIC_FLAGS::WIC_FLAGS_NONE, nullptr, m_Image);
    }

    if (FAILED(hr))
    {
        MessageBox(nullptr, L"Fail to load image from file!", L"Error", MB_OK);
        return E_FAIL;
    }
    
    hr = CreateShaderResourceView(DEVICE, m_Image.GetImages(), 
        m_Image.GetImageCount(), m_Image.GetMetadata(), 
        m_SRV.GetAddressOf());

    if (FAILED(hr))
    {
        MessageBox(nullptr, L"Fail to create shader resource view!", L"Error", MB_OK);
        return E_FAIL;
    }

    m_SRV->GetResource((ID3D11Resource**)m_Tex2D.GetAddressOf());
    m_Tex2D->GetDesc(&m_Desc);

    return hr;
}

int CTexture::Save(const std::wstring& filePath)
{
    return 0;
}

int CTexture::Create(UINT width, UINT height, DXGI_FORMAT pixelFormat, UINT bindFlag, D3D11_USAGE usage)
{
    m_Desc.Format = pixelFormat;
    m_Desc.Width = width;
    m_Desc.Height = height;
    m_Desc.ArraySize = 1;

    m_Desc.Usage = usage;
    if (usage == D3D11_USAGE_DYNAMIC)
        m_Desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
    else
        m_Desc.CPUAccessFlags = 0;

    m_Desc.BindFlags = bindFlag;
    m_Desc.MipLevels = 1;
    m_Desc.SampleDesc.Count = 1;
    m_Desc.SampleDesc.Quality = 0;

    if (FAILED(DEVICE->CreateTexture2D(&m_Desc, nullptr, m_Tex2D.GetAddressOf())))
        return E_FAIL;

    if (m_Desc.BindFlags & D3D11_BIND_DEPTH_STENCIL)
        DEVICE->CreateDepthStencilView(m_Tex2D.Get(), nullptr, m_DSV.GetAddressOf());
    else
    {
        if (m_Desc.BindFlags & D3D11_BIND_RENDER_TARGET)
            DEVICE->CreateRenderTargetView(m_Tex2D.Get(), nullptr, m_RTV.GetAddressOf());
        if (m_Desc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
        {
            D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
            SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            SRVDesc.Texture2D.MipLevels = 1;
            SRVDesc.Texture2D.MostDetailedMip = 0;

            DEVICE->CreateShaderResourceView(m_Tex2D.Get(), &SRVDesc, m_SRV.GetAddressOf());
        }
        if (m_Desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
        {
            D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc = {};
            UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;

            DEVICE->CreateUnorderedAccessView(m_Tex2D.Get(), &UAVDesc, m_UAV.GetAddressOf());
        }
    }

    return S_OK;
}

int CTexture::Create(ComPtr<ID3D11Texture2D> tex2D)
{
    m_Tex2D = tex2D;
    m_Tex2D->GetDesc(&m_Desc);

    if (m_Desc.BindFlags & D3D11_BIND_DEPTH_STENCIL)
        DEVICE->CreateDepthStencilView(m_Tex2D.Get(), nullptr, m_DSV.GetAddressOf());
    else
    {
        if (m_Desc.BindFlags & D3D11_BIND_RENDER_TARGET)
            DEVICE->CreateRenderTargetView(m_Tex2D.Get(), nullptr, m_RTV.GetAddressOf());
        if (m_Desc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
        {
            D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
            SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            SRVDesc.Texture2D.MipLevels = 1;
            SRVDesc.Texture2D.MostDetailedMip = 0;

            DEVICE->CreateShaderResourceView(m_Tex2D.Get(), &SRVDesc, m_SRV.GetAddressOf());
        }
        if (m_Desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
        {
            D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc = {};
            UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;

            DEVICE->CreateUnorderedAccessView(m_Tex2D.Get(), &UAVDesc, m_UAV.GetAddressOf());
        }
    }

    return 0;
}
