#include "pch.h"
#include "CTexture.h"

#include "CDevice.h"

CTexture::CTexture()
    : CAsset(ASSET_TYPE::TEXTURE)
{
}

CTexture::~CTexture()
{
}

void CTexture::Bind(int registerSlot)
{
    CONTEXT->PSSetShaderResources(registerSlot, 1, m_SRV.GetAddressOf());
}

void CTexture::Clear(int registerSlot)
{
    ID3D11ShaderResourceView* pSRV = nullptr;
    CONTEXT->PSSetShaderResources(registerSlot, 1, &pSRV);
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

    return hr;
}

int CTexture::Save(const std::wstring& filePath)
{
    return 0;
}
