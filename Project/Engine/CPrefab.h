#pragma once
#include "CAsset.h"

class CGameObject;

class CPrefab : public CAsset
{
public:
	CPrefab();
	CPrefab(CGameObject* proto);
	CPrefab(const CPrefab& other);
	~CPrefab();
	CLONE(CPrefab)

	virtual int Load(const std::wstring& filePath) override { return S_OK; }
	virtual int Save(const std::wstring& filePath) override { return S_OK; }

	CGameObject* Instantiate();

private:
	CGameObject* m_ProtoObj;
};

