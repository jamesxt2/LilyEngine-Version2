#include "pch.h"
#include "CKeyMgr.h"


UINT g_KeyValue[(UINT)KEY::KEY_END] =
{
	'W', 'A', 'S', 'D', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
	VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN,
	VK_RETURN, VK_ESCAPE, VK_SPACE, VK_LSHIFT, VK_CONTROL, VK_MENU
};


CKeyMgr::CKeyMgr()
{

}

CKeyMgr::~CKeyMgr()
{

}

void CKeyMgr::Init()
{
	for (int i = 0; i < (UINT)KEY::KEY_END; ++i)
	{
		TKeyInfo info;
		m_VecKey.push_back(info);
	}
}

void CKeyMgr::Tick()
{
	for (size_t i = 0; i < m_VecKey.size(); ++i)
	{
		if (GetAsyncKeyState(g_KeyValue[i]) & 0x8001)
		{
			if (!m_VecKey[i].PrevPressed)
				m_VecKey[i].State = KEY_STATE::TAP;
			else
				m_VecKey[i].State = KEY_STATE::PRESSED;

			m_VecKey[i].PrevPressed = true;
		}
		else
		{
			if (m_VecKey[i].PrevPressed)
				m_VecKey[i].State = KEY_STATE::RELEASED;
			else
				m_VecKey[i].State = KEY_STATE::NONE;

			m_VecKey[i].PrevPressed = false;
		}
	}
}