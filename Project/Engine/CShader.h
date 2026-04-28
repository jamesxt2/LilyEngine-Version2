#pragma once
#include "CAsset.h"

class CShader : public CAsset
{
public:
	CShader(ASSET_TYPE type);
	~CShader();

	virtual void Bind() = 0;

protected:
	ComPtr<ID3DBlob> m_ErrBlob;
};

