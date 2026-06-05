#include "pch.h"
#include "CAssetMgr.h"

CAssetMgr::CAssetMgr()
{
}

CAssetMgr::~CAssetMgr()
{
}

Ptr<CTexture> CAssetMgr::CreateTexture(const std::wstring& key, UINT width, UINT height, DXGI_FORMAT format, UINT bindFlag, D3D11_USAGE usage)
{
	Ptr<CTexture> pTex = FindAsset<CTexture>(key);
	if (pTex != nullptr)
		return pTex;

	pTex = new CTexture;
	pTex->Create(width, height, format, bindFlag, usage);

	pTex->m_Key = key;
	m_AssetMap[(UINT)ASSET_TYPE::TEXTURE].insert(std::make_pair(key, pTex.Get()));

	return pTex;
}

Ptr<CTexture> CAssetMgr::CreateTexture(const std::wstring& key, ComPtr<ID3D11Texture2D> tex2D)
{
	Ptr<CTexture> pTex = FindAsset<CTexture>(key);
	if (pTex != nullptr)
		return pTex;

	pTex = new CTexture;
	pTex->Create(tex2D);

	pTex->m_Key = key;
	m_AssetMap[(UINT)ASSET_TYPE::TEXTURE].insert(std::make_pair(key, pTex.Get()));

	return pTex;
}
