#include "pch.h"
#include "CAnimator2D.h"

#include "CAnim2D.h"

CAnimator2D::CAnimator2D()
	: CComponent(COMPONENT_TYPE::ANIMATOR2D),
	m_CurAnim(nullptr), m_Repeat(false)
{
}

CAnimator2D::CAnimator2D(const CAnimator2D& other)
	: CComponent(other),
	m_CurAnim(nullptr),
	m_Repeat(other.m_Repeat)
{
	for (const auto& pair : other.m_mapAnim)
	{
		CAnim2D* pCloneAnim = pair.second->Clone();
		pCloneAnim->m_Animator = this;
		m_mapAnim.insert(std::make_pair(pair.first, pCloneAnim));
	}
	if (other.m_CurAnim != nullptr)
	{
		m_CurAnim = FindAnimation(other.m_CurAnim->GetName());
	}
}

CAnimator2D::~CAnimator2D()
{
	Safe_Del_Map(m_mapAnim);
}

void CAnimator2D::FinalTick()
{
	if (m_CurAnim == nullptr)
		return;

	if (m_Repeat && m_CurAnim->IsFinish())
		m_CurAnim->Reset();

	m_CurAnim->FinalTick();
}

void CAnimator2D::Bind()
{
	if (m_CurAnim == nullptr)
		return; 
	m_CurAnim->Bind();
}

void CAnimator2D::Unbind()
{
	CAnim2D::Unbind();
}

void CAnimator2D::CreateAnimation(const std::wstring& animName, 
	Ptr<CTexture> atlasTex, Vec2 leftTopPixelPos, Vec2 slicePixelSize, 
	Vec2 backgroundSize, int frameCount, UINT fps)
{
	assert(!FindAnimation(animName));

	CAnim2D* pAnim = new CAnim2D;
	pAnim->SetName(animName);
	pAnim->Create(atlasTex, leftTopPixelPos, slicePixelSize, backgroundSize, frameCount, fps);
	pAnim->m_Animator = this;

	m_mapAnim.insert(std::make_pair(animName, pAnim));
}

CAnim2D* CAnimator2D::FindAnimation(const std::wstring& animName)
{
	std::map<std::wstring, CAnim2D*>::iterator iter = m_mapAnim.find(animName);

	if (iter == m_mapAnim.end())
		return nullptr;

	return iter->second;
}

void CAnimator2D::Play(const std::wstring& animName, bool repeat)
{
	m_CurAnim = FindAnimation(animName);
	m_Repeat = repeat;
}

void CAnimator2D::LoadAnimation(const std::wstring& relativePath)
{
	CAnim2D* pAnim = new CAnim2D;
	pAnim->Load(relativePath);

	pAnim->m_Animator = this;
	m_mapAnim.insert(std::make_pair(pAnim->GetName(), pAnim));
}
