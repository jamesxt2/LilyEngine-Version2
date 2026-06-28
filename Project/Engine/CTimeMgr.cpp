#include "pch.h"
#include "CTimeMgr.h"

#include "CEngine.h"
#include "CLevelMgr.h"
#include "CLevel.h"

CTimeMgr::CTimeMgr()
	: m_Frequency{}, m_CurCount{}, m_PreCount{}, 
	m_DT(0.f), m_EngineDT(0.f), m_Time(0.f), m_EngineTime(0.f),
	m_AccTime(0.f), m_FrmCount(0)
{

}

CTimeMgr::~CTimeMgr()
{

}

void CTimeMgr::Init()
{
	// count per second
	QueryPerformanceFrequency(&m_Frequency);
	QueryPerformanceCounter(&m_CurCount);
	m_PreCount = m_CurCount;
}

void CTimeMgr::Tick()
{
	QueryPerformanceCounter(&m_CurCount);

	m_EngineDT = m_DT = (float)(m_CurCount.QuadPart - m_PreCount.QuadPart) / (float)m_Frequency.QuadPart;
	
	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	if (pLevel == nullptr || pLevel->GetState() != LEVEL_STATE::PLAY)
		m_DT = 0.f;

	m_PreCount = m_CurCount;

	m_Time += m_DT;
	m_EngineTime += m_EngineDT;
	++m_FrmCount;
	m_AccTime += m_DT;
	if (m_AccTime > 1.f)
	{
		HWND hMainWnd = CEngine::GetInst()->GetMainWnd();
		wchar_t szText[255] = {};
		swprintf_s(szText, L"FPS : %d Delta Time : %f", m_FrmCount, m_DT);
		SetWindowText(hMainWnd, szText);

		m_AccTime -= 1.f;
		m_FrmCount = 0;
	}

	g_GlobalData.DeltaTime = m_DT;
	g_GlobalData.Time = m_Time;
	g_GlobalData.DeltaTimeEngine = m_EngineDT;
	g_GlobalData.TimeEngine = m_EngineTime;
}
