#pragma once


enum class KEY
{
	W, A, S, D, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, 
	LEFT, RIGHT, UP, DOWN, 
	ENTER, ESC, SPACE, LSHIFT, CTRL, ALT, 

	KEY_END
};

enum class KEY_STATE
{
	TAP, PRESSED, RELEASED, NONE
};

struct TKeyInfo
{
	KEY_STATE	State;
	bool		PrevPressed;

	TKeyInfo()
		: State(KEY_STATE::NONE), PrevPressed(false)
	{
	}
};

class CKeyMgr : public CSingleton<CKeyMgr>
{
	SINGLE(CKeyMgr)

public:
	void Init();
	void Tick();

private:
	std::vector<TKeyInfo> m_VecKey;

public:
	inline KEY_STATE GetKeyState(KEY _Key) { return m_VecKey[(UINT)_Key].State; }
};

