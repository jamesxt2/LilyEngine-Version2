#pragma once


template<typename T, int size>
void Safe_Del_Array(T* (&Array)[size])
{
	for (int i = 0; i < size; ++i)
	{
		if (Array[i] != nullptr)
		{
			delete Array[i];
			Array[i] = nullptr;
		}
	}
}

template<typename T>
void Safe_Del_Vector(std::vector<T*>& vec)
{
	for (size_t i = 0; i < vec.size(); ++i)
	{
		if (vec[i] != nullptr)
			delete vec[i];
	}
	vec.clear();
}

template<typename T1, typename T2>
void Safe_Del_Map(std::map<T1, T2>& _map)
{
	for (const auto& pair : _map)
	{
		if (pair.second != nullptr)
			delete pair.second;
	}
	_map.clear();
}

void DrawDebugRect(Vec3 worldPos, Vec3 worldRot, Vec3 worldScale, Vec4 color, float duration);
void DrawDebugRect(Matrix matWorld, Vec4 color, float duration);

void DrawDebugCircle(Vec3 worldPos, float radius, Vec4 color, float duration);

void SaveWString(const std::wstring& str, FILE* file);
void LoadWString(std::wstring& str, FILE* file);

template<typename T>
class Ptr;

template<typename T>
void SaveAssetRef(_In_ Ptr<T>& asset, FILE* file)
{
	bool bUse = asset.Get();
	fwrite(&bUse, sizeof(bool), 1, file);

	if (bUse)
	{
		SaveWString(asset->GetKey(), file);
		SaveWString(asset->GetRelativePath(), file);
	}
}

#include "CAssetMgr.h"

template<typename T>
void LoadAssetRef(_Out_ Ptr<T>& asset, FILE* file)
{
	bool bUse = false;
	fread(&bUse, sizeof(bool), 1, file);

	if (bUse)
	{
		std::wstring key, relativePath;
		LoadWString(key, file);
		LoadWString(relativePath, file);
		asset = CAssetMgr::GetInst()->Load<T>(key, relativePath);
	}
}

class CGameObject;
void SpawnObject(int layerIdx, CGameObject* object);

bool IsValid(CGameObject*& object);

