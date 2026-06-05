#pragma once
#include "CAsset.h"

class CShader : public CAsset
{
public:
	CShader(ASSET_TYPE type);
	~CShader();

	virtual int Bind() = 0;

	virtual int Load(const std::wstring& filePath) override { return S_OK; }
	virtual int Save(const std::wstring& filePath) override { return S_OK; }

protected:
	ComPtr<ID3DBlob> m_ErrBlob;
};

