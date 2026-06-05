#include "pch.h"
#include "CMesh.h"

#include "CDevice.h"
#include "CGameObject.h"

CMesh::CMesh()
	: CAsset(ASSET_TYPE::MESH),
	m_VBDesc{}, m_VtxCount(0), m_VtxSysMem(nullptr),
	m_IBDesc{}, m_IdxCount(0), m_IdxSysMem(nullptr)
{
}

CMesh::~CMesh()
{
	if (m_VtxSysMem != nullptr)
		delete[] m_VtxSysMem;

	if (m_IdxSysMem != nullptr)
		delete[] m_IdxSysMem;
}

int CMesh::Create(Vtx* vtxSysMem, size_t vtxCount, UINT* idxSysMem, size_t idxCount)
{
	m_VtxCount = (UINT)vtxCount;
	m_IdxCount = (UINT)idxCount;

	// SysMem maintain
	m_VtxSysMem = new Vtx[vtxCount];
	memcpy(m_VtxSysMem, vtxSysMem, sizeof(Vtx) * vtxCount);

	m_IdxSysMem = new UINT[idxCount];
	memcpy(m_IdxSysMem, idxSysMem, sizeof(UINT) * idxCount);

	// Create vertex buffer
	m_VBDesc.ByteWidth = sizeof(Vtx) * m_VtxCount;
	m_VBDesc.MiscFlags = 0;

	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	m_VBDesc.CPUAccessFlags = 0;
	m_VBDesc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA VBSubDesc = {};
	VBSubDesc.pSysMem = m_VtxSysMem;

	if (FAILED(DEVICE->CreateBuffer(&m_VBDesc, &VBSubDesc, m_VB.GetAddressOf())))
		return E_FAIL;

	// Create idex buffer
	m_IBDesc.ByteWidth = sizeof(UINT) * m_IdxCount;
	m_IBDesc.MiscFlags = 0;
	m_IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	// Allow changing from CPU like move the triangle
	m_IBDesc.CPUAccessFlags = 0;
	m_IBDesc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA IBSubDesc = {};
	IBSubDesc.pSysMem = m_IdxSysMem;

	if (FAILED(DEVICE->CreateBuffer(&m_IBDesc, &IBSubDesc, m_IB.GetAddressOf())))
		return E_FAIL;

	return S_OK;
}

void CMesh::Bind()
{
	UINT Stride = sizeof(Vtx);
	UINT Offset = 0;
	CONTEXT->IASetVertexBuffers(0, 1, m_VB.GetAddressOf(), &Stride, &Offset);
	CONTEXT->IASetIndexBuffer(m_IB.Get(), DXGI_FORMAT_R32_UINT, 0);
}

void CMesh::Render()
{
	Bind();
	CONTEXT->DrawIndexed(m_IdxCount, 0, 0);
}

void CMesh::Render_Particle(UINT instanceCount)
{
	Bind();
	CONTEXT->DrawIndexedInstanced(m_IdxCount, instanceCount, 0, 0, 0);
}
