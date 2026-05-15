#pragma once
#include "CComponent.h"

class CCollision2D : public CComponent
{
public:
	CCollision2D();
	~CCollision2D();

	virtual void FinalTick() override;

	void BeginOverlap(CCollision2D* otherCollision);
	void Overlap(CCollision2D* otherCollision);
	void EndOverlap(CCollision2D* otherCollision);

private:
	Vec3 m_Offset;
	Vec3 m_FinalPos;

	Vec3 m_Scale;

	Vec3 m_Rotation;

	bool m_Absolute;

	Matrix m_matWorld;

public:
	inline void SetOffset(Vec3 offset) { m_Offset = offset; }
	inline void SetScale(Vec3 scale) { m_Scale = scale; }
	inline void SetAbsolute(bool absolute) { m_Absolute = absolute; }
	inline void SetRotationZ(float angle) { m_Rotation.z = angle; }

	inline Vec3 GetOffset() const { return m_Offset; }
	inline Vec3 GetFinalPos() const { return m_FinalPos; }
	inline Vec3 GetScale() const { return m_Scale; }
	inline bool IsAbsolute() const { return m_Absolute; }
	inline float GetRotationZ() const { return m_Rotation.z; }

	inline const Matrix& GetWorldMat() const { return m_matWorld; }
};

