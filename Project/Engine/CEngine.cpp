#include "pch.h"
#include "CEngine.h"

#include "CDevice.h"
#include "CTimeMgr.h"
#include "CPathMgr.h"
#include "CKeyMgr.h"
#include "CAssetMgr.h"
#include "CLevelMgr.h"

CEngine::CEngine()
	: m_hMainHwnd(nullptr), m_Resolution{}
{
}

CEngine::~CEngine()
{
}

int CEngine::Init(HWND _hWnd, POINT _Resolution)
{
	m_hMainHwnd = _hWnd;
	m_Resolution = _Resolution;

	// Window
	RECT rt = { 0, 0, m_Resolution.x, m_Resolution.y };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(m_hMainHwnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);

	// DirectX 11
	if (FAILED(CDevice::GetInst()->Init(m_hMainHwnd, m_Resolution)))
	{
		MessageBox(nullptr, L"Fail to create device!", L"Error", MB_OK);
		return E_FAIL;
	}

	// Manager init
	CPathMgr::GetInst()->Init();
	CTimeMgr::GetInst()->Init();
	CKeyMgr::GetInst()->Init();
	CAssetMgr::GetInst()->Init();
	CLevelMgr::GetInst()->Init();

	return S_OK;
}

void CEngine::Progress()
{
	/**************/
	// Tick
	/**************/
	// Manager Tick
	CTimeMgr::GetInst()->Tick();
	CKeyMgr::GetInst()->Tick();

	// Object Tick
	CLevelMgr::GetInst()->Tick();

	/*************/
	// Rendering
	/*************/
	// Target Clear
	float ClearColor[4] = { 0.3f, 0.3f, 0.3f, 1.f };
	CDevice::GetInst()->ClearTarget(ClearColor);

	// Object Render
	CLevelMgr::GetInst()->Render();

	// Present
	CDevice::GetInst()->Present();
}
