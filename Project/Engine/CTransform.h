#pragma once
#include "CComponent.h"

class CTransform : public CComponent
{
public:
	CTransform();
	~CTransform();

	virtual void FinalTick() override;
	void Bind();

private:
	Vec3 m_RelativePosition;
	Vec3 m_RelativeRotation;
	Vec3 m_RelativeScale;

public:
	inline Vec3 GetRelativePosition() const { return m_RelativePosition; }
	inline Vec3 GetRelativeRotation() const { return m_RelativeRotation; }
	inline Vec3 GetRelativeScale() const { return m_RelativeScale; }

	inline void SetRelativePosition(const Vec3& position) { m_RelativePosition = position; }
	inline void SetRelativeRotation(const Vec3& rotation) { m_RelativeRotation = rotation; }
	inline void SetRelativeScale(const Vec3& scale) { m_RelativeScale = scale; }

	inline void SetRelativePosition(float x, float y, float z) { m_RelativePosition = Vec3(x, y, z); }
	inline void SetRelativeRotation(float x, float y, float z) { m_RelativeRotation = Vec3(x, y, z); }
	inline void SetRelativeScale(float x, float y, float z) { m_RelativeScale = Vec3(x, y, z); }
};

