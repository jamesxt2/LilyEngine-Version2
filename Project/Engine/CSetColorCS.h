#pragma once
#include "CComputeShader.h"

#include "CTexture.h"

class CSetColorCS : public CComputeShader
{
public:
	CSetColorCS();
	~CSetColorCS();

	virtual int Bind() override;
	virtual void Clear() override;
	virtual void CalculateGroupNum() override;

private:
	Ptr<CTexture> m_TargetTex;
	Vec4 m_ClearColor;

public:
	inline void SetTargetTexture(const Ptr<CTexture>& tex) { m_TargetTex = tex; }
	inline void SetClearColor(const Vec4& color) { m_ClearColor = color; }
};

