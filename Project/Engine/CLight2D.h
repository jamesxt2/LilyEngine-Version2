#pragma once
#include "CComponent.h"

class CLight2D : public CComponent
{
public:
	CLight2D();
	~CLight2D();

	virtual void FinalTick() override;

private:
	TLightInfo m_Info;
	int m_LightIdx;

public:
	inline void SetDiffuse(const Vec3& diffuse) { m_Info.Light.vDiffuse = diffuse; }
	inline void SetAmbient(const Vec3& ambient) { m_Info.Light.vAmbient = ambient; }
	inline void SetSpecular(const Vec3& specular) { m_Info.Light.vMaxSpecular = specular; }

	inline void SetRange(float range) { m_Info.Range = range; }
	inline void SetAngle(float angle) { m_Info.Angle = angle; }

	void SetLightType(LIGHT_TYPE type);

	inline const TLightInfo& GetLightInfo() const { return m_Info; }
};

