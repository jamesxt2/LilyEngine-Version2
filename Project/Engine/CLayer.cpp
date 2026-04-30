#include "pch.h"
#include "CLayer.h"

#include "CGameObject.h"

CLayer::CLayer()
{
}

CLayer::~CLayer()
{
	Safe_Del_Vector(m_vecObject);
}

void CLayer::Begin()
{
	for (size_t i = 0; i < m_vecObject.size(); ++i)
	{
		m_vecObject[i]->Begin();
	}
}

void CLayer::Tick()
{
	for (size_t i = 0; i < m_vecObject.size(); ++i)
	{
		m_vecObject[i]->Tick();
	}
}

void CLayer::FinalTick()
{
	for (size_t i = 0; i < m_vecObject.size(); ++i)
	{
		m_vecObject[i]->FinalTick();
	}
}

void CLayer::Render()
{
	for (size_t i = 0; i < m_vecObject.size(); ++i)
	{
		m_vecObject[i]->Render();
	}
}