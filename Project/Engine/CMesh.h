#pragma once
#include "CAsset.h"

class CMesh : public CAsset
{

public:
	CMesh();
	~CMesh();

	int Create(Vtx* vtxSysMem, size_t vtxCount, UINT* idxSysMem, size_t idxCount);
	void Render();

	virtual int Load(const std::wstring& filePath) override { return S_OK; }
	virtual int Save(const std::wstring& filePath) override { return S_OK; }

private:
	void Bind();

private:
	ComPtr<ID3D11Buffer>	m_VB;
	D3D11_BUFFER_DESC		m_VBDesc;
	UINT					m_VtxCount;
	Vtx*					m_VtxSysMem;

	ComPtr<ID3D11Buffer>	m_IB;
	D3D11_BUFFER_DESC		m_IBDesc;
	UINT					m_IdxCount;
	UINT*					m_IdxSysMem;
};

