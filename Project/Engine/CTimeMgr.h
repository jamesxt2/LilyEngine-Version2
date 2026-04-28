#pragma once

class CTimeMgr : public CSingleton<CTimeMgr>
{
	SINGLE(CTimeMgr)

public:
	void Init();
	void Tick();

private:
	LARGE_INTEGER		m_Frequency; // count per second
	LARGE_INTEGER		m_CurCount;
	LARGE_INTEGER		m_PreCount; // last frame

	float				m_DT; // 1 frame delta time
	float				m_Time; // since begin
	float				m_AccTime; // accumulate time

	UINT				m_FrmCount;

public:
	inline float GetDeltaTime() { return m_DT; }
};

