#pragma once
#include "CEntity.h"

class CConstBuffer : public CEntity
{
public:
	CConstBuffer();
	CConstBuffer(const CConstBuffer& other) = delete;
	~CConstBuffer();
	CLONE_DISABLE(CConstBuffer)

	int Create(size_t bufferSize, CB_TYPE type);
	void SetData(void* pData);
	void Bind();
	void Bind_CS();

private:
	ComPtr<ID3D11Buffer>	m_CB;
	D3D11_BUFFER_DESC		m_CBDesc;
	CB_TYPE					m_Type;
};

