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
	//Bind();
}

void CTransform::Bind()
{
	// System memory -> GPU
	CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);

	TTransform trans = {};
	trans.Position = m_RelativePosition;
	trans.Scale = m_RelativeScale;

	pCB->SetData(&trans);
	pCB->Bind();
}
