#pragma once
#include "singleton.h"

#include "assets.h"

class CAssetMgr : public CSingleton<CAssetMgr>
{
	SINGLE(CAssetMgr)

public:
	void Init();
	void CreateDefaultMesh();
	void CreateDefaultTexture();
	void CreateDefaultMaterial();
	void CreateDefaultGraphicsShader();
	void CreateDefaultComputeShader();

	template<typename T>
	Ptr<T> FindAsset(const std::wstring& key);

	template<typename T>
	void AddAsset(const std::wstring& key, Ptr<T> pAsset);

private:
	std::map<std::wstring, Ptr<CAsset>> m_AssetMap[(UINT)ASSET_TYPE::END];


};

template<typename T>
ASSET_TYPE GetAssetType()
{
	if constexpr (std::is_same_v<T, CMesh>)
		return ASSET_TYPE::MESH;
	if constexpr (std::is_same_v<T, CGraphicsShader>)
		return ASSET_TYPE::GRAPHICS_SHADER;
	if constexpr (std::is_same_v<T, CComputeShader>)
		return ASSET_TYPE::COMPUTE_SHADER;
}

template<typename T>
inline Ptr<T> CAssetMgr::FindAsset(const std::wstring& key)
{
	ASSET_TYPE type = GetAssetType<T>();

	std::map<std::wstring, Ptr<CAsset>>::iterator iter =
		m_AssetMap[(UINT)type].find(key);

	if (iter == m_AssetMap[(UINT)type].end())
		return nullptr;

#ifdef _DEBUG
	return dynamic_cast<T*>(iter->second.Get());
#else
	return (T*)iter->second.Get();
#endif
}

template<typename T>
inline void CAssetMgr::AddAsset(const std::wstring& key, Ptr<T> pAsset)
{
	Ptr<T> findAsset = FindAsset<T>(key);

	assert(findAsset.Get() == nullptr);

	ASSET_TYPE type = GetAssetType<T>();

	m_AssetMap[(UINT)type].insert(std::make_pair(key, pAsset.Get()));
	pAsset->m_Key = key;
}
