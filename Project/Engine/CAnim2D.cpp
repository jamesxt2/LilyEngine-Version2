#include "pch.h"
#include "CAnim2D.h"

#include "CAnimator2D.h"
#include "CConstBuffer.h"
#include "CDevice.h"
#include "CTimeMgr.h"
#include "CPathMgr.h"
#include "CAssetMgr.h"


CAnim2D::CAnim2D()
	: m_Animator(nullptr), m_CurIdx(0), m_Finish(false), m_Time(0.f)
{
}

CAnim2D::~CAnim2D()
{
}

void CAnim2D::Create(Ptr<CTexture> atlasTex, Vec2 leftTopPixelPos, Vec2 slicePixelSize, Vec2 backgroundSize, int frameCount, UINT fps)
{
	assert(atlasTex.Get());

	m_AtlasTex = atlasTex;

	m_BackgroundSize.x = backgroundSize.x / m_AtlasTex->GetWidth();
	m_BackgroundSize.y = backgroundSize.y / m_AtlasTex->GetHeight();

	for (int i = 0; i < frameCount; ++i)
	{
		TAnim2DFrm frm = {};

		frm.LeftTop.x = (leftTopPixelPos.x + slicePixelSize.x * i) / (float)atlasTex->GetWidth();
		frm.LeftTop.y = leftTopPixelPos.y / (float)atlasTex->GetHeight();

		frm.Duration = 1.f / (float)fps;

		frm.SliceSize.x = slicePixelSize.x / m_AtlasTex->GetWidth();
		frm.SliceSize.y = slicePixelSize.y / m_AtlasTex->GetHeight();
		/*
		if (i == 1)
		{
			frm.Offset.x += 5.f / m_AtlasTex->GetWidth();
			frm.Offset.y = 0.f;
		}
		*/
		m_vecFrm.push_back(frm);
	}
}


void CAnim2D::FinalTick()
{
	if (m_Finish) return;

	m_Time += DT;
	if (m_vecFrm[m_CurIdx].Duration < m_Time)
	{
		m_Time -= m_vecFrm[m_CurIdx].Duration;
		++m_CurIdx;
		
		if (m_vecFrm.size() <= m_CurIdx)
		{
			--m_CurIdx;
			m_Finish = true;
		}
	}
}

void CAnim2D::Bind()
{
	if (m_AtlasTex != nullptr)
		m_AtlasTex->Bind(14);

	static CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::ANIMATION);
	
	TAnim2DInfo info = {};
	info.vLeftTop = m_vecFrm[m_CurIdx].LeftTop;
	info.vSliceSize = m_vecFrm[m_CurIdx].SliceSize;
	info.vOffset = m_vecFrm[m_CurIdx].Offset;
	info.vBackground = m_BackgroundSize;
	info.UseAnim2D = 1;
	
	pCB->SetData(&info); 
	pCB->Bind();
}

void CAnim2D::Unbind()
{
	static CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::ANIMATION);

	TAnim2DInfo info = {};
	info.UseAnim2D = 0;

	pCB->SetData(&info);
	pCB->Bind();
}

void CAnim2D::Save(const std::wstring& relativePath)
{
	std::wstring filePath = CPathMgr::GetInst()->GetContentPath() + relativePath + GetName() + L".anim";

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, filePath.c_str(), L"wb");

	if (pFile == nullptr)
	{
		MessageBox(nullptr, L"Cannot open animation file!", L"Error", MB_OK);
		return;
	}

	SaveWString(GetName(), pFile);

	size_t len = m_vecFrm.size();
	fwrite(&len, sizeof(size_t), 1, pFile);
	fwrite(m_vecFrm.data(), sizeof(TAnim2DFrm), len, pFile);
	fwrite(&m_BackgroundSize, sizeof(Vec2), 1, pFile);

	SaveAssetRef(m_AtlasTex, pFile);

	fclose(pFile);
}

void CAnim2D::Load(const std::wstring& relativePath)
{
	std::wstring filePath = CPathMgr::GetInst()->GetContentPath() + relativePath;

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, filePath.c_str(), L"rb");

	if (pFile == nullptr)
	{
		MessageBox(nullptr, L"Cannot load animation file!", L"Error", MB_OK);
		return;
	}

	std::wstring name;
	LoadWString(name, pFile);
	SetName(name);

	size_t len = 0;
	fread(&len, sizeof(size_t), 1, pFile);

	m_vecFrm.resize(len);
	fread(m_vecFrm.data(), sizeof(TAnim2DFrm), len, pFile);

	fread(&m_BackgroundSize, sizeof(Vec2), 1, pFile);

	LoadAssetRef(m_AtlasTex, pFile);

	fclose(pFile);
}