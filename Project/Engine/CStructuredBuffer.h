#pragma once
#include "CEntity.h"

class CStructuredBuffer : public CEntity
{
public:
	CStructuredBuffer();
	~CStructuredBuffer();

	int Create(UINT elementSize, UINT elementCount, void* initialData = nullptr);
	void SetData(void* pData, UINT elementCount = 0);
	void Bind(UINT registerSlot);

private:
	ComPtr<ID3D11Buffer>				m_SB;
	D3D11_BUFFER_DESC					m_SBDesc;
	ComPtr<ID3D11ShaderResourceView>	m_SRV;

	UINT								m_ElementSize;
	UINT								m_ElementCount;

public:
	inline UINT GetElementSize() const { return m_ElementSize; }
	inline UINT GetElementCount() const { return m_ElementCount; }
};

