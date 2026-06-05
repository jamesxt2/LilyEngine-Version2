#include "pch.h"
#include "CSetColorCS.h"

CSetColorCS::CSetColorCS()
	: CComputeShader(32, 32, 1)
{
}

CSetColorCS::~CSetColorCS()
{
}

int CSetColorCS::Bind()
{
	if (m_TargetTex == nullptr || m_TargetTex->GetUAV() == nullptr) return E_FAIL;

	m_TargetTex->Bind_CS_UAV(0);
	m_Const.v4Arr[0] = m_ClearColor;
	m_Const.iArr[0] = (int)m_TargetTex->GetWidth();
	m_Const.iArr[1] = (int)m_TargetTex->GetHeight();

	return S_OK;
}

void CSetColorCS::Clear()
{
	m_TargetTex->Clear_CS_UAV(0);
}

void CSetColorCS::CalculateGroupNum()
{
	m_GroupX = m_TargetTex->GetWidth() / m_ThreadPerGroupX;
	m_TargetTex->GetWidth() % m_ThreadPerGroupX ? ++m_GroupX : 1;

	m_GroupY = m_TargetTex->GetHeight() / m_ThreadPerGroupY;
	m_TargetTex->GetHeight() % m_ThreadPerGroupY ? ++m_GroupY : 1;

	m_GroupZ = 1;
}
