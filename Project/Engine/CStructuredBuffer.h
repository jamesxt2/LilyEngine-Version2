#pragma once
#include "CEntity.h"


enum class SB_TYPE
{
	SRV_ONLY,
	SRV_UAV
};


class CStructuredBuffer : public CEntity
{
public:
	CStructuredBuffer();
	~CStructuredBuffer();

	int Create(UINT elementSize, UINT elementCount, SB_TYPE type, bool bSysMemMove, void* initialData = nullptr);
	
	void SetData(void* pData, UINT elementCount = 0);
	void GetData(void* pDst, UINT elementCount = 0);

	void Bind(UINT registerSlot);
	void Bind_CS_SRV(UINT registerSlot);
	void Bind_CS_UAV(UINT registerSlot);

	static void Clear(UINT registerSlot);
	static void Clear_CS_SRV(UINT registerSlot);
	static void Clear_CS_UAV(UINT registerSlot);

private:
	ComPtr<ID3D11Buffer>				m_SB_Main;  // Bind
	ComPtr<ID3D11Buffer>				m_SB_Write; // System Memory -> GPU
	ComPtr<ID3D11Buffer>				m_SB_Read;  // GPU -> System Memory

	D3D11_BUFFER_DESC					m_SBDesc;

	ComPtr<ID3D11ShaderResourceView>	m_SRV;
	ComPtr<ID3D11UnorderedAccessView>   m_UAV;

	UINT								m_ElementSize;
	UINT								m_ElementCount;

	SB_TYPE								m_Type;

	bool								m_SysMemMove;

	UINT								m_SRV_Register;
	UINT								m_UAV_Register;

public:
	inline UINT GetElementSize() const { return m_ElementSize; }
	inline UINT GetElementCount() const { return m_ElementCount; }
};

