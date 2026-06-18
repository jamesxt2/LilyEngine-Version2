#pragma once
#include "CComponent.h"

#include "CTexture.h"

class CAnim2D;

class CAnimator2D : public CComponent
{
public:
	CAnimator2D();
	CAnimator2D(const CAnimator2D& other);
	~CAnimator2D();
	CLONE(CAnimator2D)

	virtual void FinalTick() override;
	void Bind();
	static void Unbind();

	void CreateAnimation(const std::wstring& animName, Ptr<CTexture> atlasTex, Vec2 leftTopPixelPos, Vec2 slicePixelSize, Vec2 backgroundSize, int frameCount, UINT fps);

	CAnim2D* FindAnimation(const std::wstring& animName);

	void Play(const std::wstring& animName, bool repeat);

	void LoadAnimation(const std::wstring& relativePath);

private:
	std::map<std::wstring, CAnim2D*> m_mapAnim;
	CAnim2D* m_CurAnim;
	bool m_Repeat;
};

