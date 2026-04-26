#include "pch.h"
#include "CEngine.h"

#include "CDevice.h"

#include "Temp.h"

CEngine::CEngine()
	: m_hMainHwnd(nullptr), m_Resolution{}
{
}

CEngine::~CEngine()
{
	TempRelease();
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

	if (FAILED(TempInit()))
	{
		MessageBox(nullptr, L"Fail to init temp!", L"Error", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

void CEngine::Progress()
{
	// Level->Tick();
	TempTick();

	float ClearColor[4] = { 0.3f, 0.3f, 0.3f, 1.f };
	CDevice::GetInst()->ClearTarget(ClearColor);

	// Level->Render();
	TempRender();

	// SwapChain->Present();
	CDevice::GetInst()->Present();
}
