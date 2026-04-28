#include "pch.h"
#include "CPathMgr.h"

CPathMgr::CPathMgr()
	: m_ContentPath{}
{

}

CPathMgr::~CPathMgr()
{

}

void CPathMgr::Init()
{
	GetCurrentDirectory(255, m_ContentPath);

	size_t len = wcslen(m_ContentPath);

	for (size_t i = len - 1; i > 0; --i)
	{
		if (m_ContentPath[i] == '\\')
		{
			m_ContentPath[i] = '\0';
			break;
		}
	}

	wcscat_s(m_ContentPath, L"\\content\\");
}
