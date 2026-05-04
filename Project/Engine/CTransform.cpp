#include "pch.h"
#include "CTransform.h"

#include "CDevice.h"
#include "CConstBuffer.h"

CTransform::CTransform()
	: CComponent(COMPONENT_TYPE::TRANSFORM),
	m_RelativeScale(Vec3(1.f, 1.f, 1.f))
{
}

CTransform::~CTransform()
{
}

void CTransform::FinalTick()
{
	m_matWorld = XMMatrixIdentity();

	Matrix matScale = XMMatrixScaling(m_RelativeScale.x, m_RelativeScale.y, m_RelativeScale.z);

	Matrix matRot = XMMatrixRotationX(m_RelativeRotation.x)
		* XMMatrixRotationY(m_RelativeRotation.y)
		* XMMatrixRotationZ(m_RelativeRotation.z);

	Matrix matTranslation = XMMatrixTranslation(m_RelativePosition.x, m_RelativePosition.y, m_RelativePosition.z);

	m_matWorld = matScale * matRot * matTranslation;

	m_RelativeDir[(UINT)DIR_TYPE::RIGHT] = XAxis;
	m_RelativeDir[(UINT)DIR_TYPE::UP] = YAxis;
	m_RelativeDir[(UINT)DIR_TYPE::FORWARD] = ZAxis;

	for (int i = 0; i < 3; ++i)
	{
		m_RelativeDir[i] = XMVector3TransformNormal(m_RelativeDir[i], matRot);
		m_RelativeDir[i].Normalize();
	}
}

void CTransform::Bind()
{
	// System memory -> GPU
	CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);

	g_Trans.matModel = m_matWorld;

	pCB->SetData(&g_Trans);
	pCB->Bind();
}
