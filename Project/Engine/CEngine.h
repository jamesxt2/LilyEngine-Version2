#pragma once

/*
class CEngine
{
public:
	static CEngine* GetInst()
	{
		if (g_This == nullptr)
		{
			g_This = new CEngine;
		}
		return g_This;
	}

	static void Destroy()
	{
		if (g_This != nullptr)
		{
			delete g_This;
			g_This = nullptr;
		}
	}

private:		
	CEngine();

private:
	static CEngine* g_This;
};

*/

class CEngine
{

public:
	~CEngine();

	static CEngine* GetInst()
	{
		static CEngine mgr;
		return &mgr;
	}

	int Init(HWND _hWnd, POINT _Resolution);

	void Progress();

private:
	CEngine();
	CEngine(const CEngine& _other) = delete;

private:
	HWND	m_hMainHwnd;
	POINT   m_Resolution;

};
