#pragma once
#include "CComponent.h"

class CTransform : public CComponent
{
public:
	CTransform();
	~CTransform();
	CLONE(CTransform)

	virtual void FinalTick() override;
	void Bind();

private:
	Vec3		m_RelativePosition;
	Vec3		m_RelativeRotation;
	Vec3		m_RelativeScale;
	Matrix		m_matWorld;

	Vec3		m_WorldDir[3];
	Vec3		m_RelativeDir[3];

	bool		m_Absolute;

public:
	inline Vec3 GetRelativePosition() const { return m_RelativePosition; }
	inline Vec3 GetRelativeRotation() const { return m_RelativeRotation; }
	inline Vec3 GetRelativeScale() const { return m_RelativeScale; }

	Vec3 GetWorldPosition() const;
	Vec3 GetWorldRotation() const;
	Vec3 GetWorldScale();

	inline void SetRelativePosition(const Vec3& position) { m_RelativePosition = position; }
	inline void SetRelativeRotation(const Vec3& rotation) { m_RelativeRotation = rotation; }
	inline void SetRelativeScale(const Vec3& scale) { m_RelativeScale = scale; }

	inline void SetRelativePosition(float x, float y, float z) { m_RelativePosition = Vec3(x, y, z); }
	inline void SetRelativeRotation(float x, float y, float z) { m_RelativeRotation = Vec3(x, y, z); }
	inline void SetRelativeScale(float x, float y, float z) { m_RelativeScale = Vec3(x, y, z); }

	inline void SetWorldMat(const Matrix& mat) { m_matWorld = mat; }
	inline const Matrix& GetWorldMat() const { return m_matWorld; }

	inline Vec3 GetRelativeDir(DIR_TYPE type) const { return m_RelativeDir[(UINT)type]; }
	inline Vec3 GetWorldDir(DIR_TYPE type) const { return m_WorldDir[(UINT)type]; }

	inline void SetAbsolute(bool bAbsolute) { m_Absolute = bAbsolute; }
	inline bool IsAbsolute() const { return m_Absolute; }
};

