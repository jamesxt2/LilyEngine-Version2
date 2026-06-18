#pragma once

class CEntity
{
public:
	CEntity();
	CEntity(const CEntity& _origin);
	virtual ~CEntity();

	virtual CEntity* Clone() = 0;

private:
	static UINT g_NextID;

	std::wstring m_Name;
	UINT m_ID;

public:
	const std::wstring& GetName() { return m_Name; }
	void SetName(const std::wstring& name) { m_Name = name; }
	UINT GetID() { return m_ID; }
};

