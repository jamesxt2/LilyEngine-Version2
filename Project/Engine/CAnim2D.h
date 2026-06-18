#pragma once
#include "CEntity.h"

class CTexture;

struct TAnim2DFrm
{
	Vec2 LeftTop;
	Vec2 SliceSize;

	Vec2 Offset;

	float Duration;
	int padding;
};

class CAnim2D : public CEntity
{
public:
	CAnim2D();
	CAnim2D(const CAnim2D& other);
	~CAnim2D();
	friend class CAnimator2D;
	CLONE(CAnim2D)

	void FinalTick();
	void Bind();
	static void Unbind();

	void Create(Ptr<CTexture> atlasTex, Vec2 leftTopPixelPos, Vec2 slicePixelSize,
		Vec2 backgroundSize, int frameCount, UINT fps);

	void Save(const std::wstring& relativePath);
	void Load(const std::wstring& relativePath);

private:
	CAnimator2D*				m_Animator;
	std::vector<TAnim2DFrm>		m_vecFrm;
	Vec2						m_BackgroundSize;
	int							m_CurIdx;
	bool						m_Finish;

	float						m_Time;

	Ptr<CTexture>				m_AtlasTex;

public:
	inline bool IsFinish() const { return m_Finish; }
	void Reset() { m_CurIdx = 0; m_Finish = false; m_Time = 0.f; }
};

