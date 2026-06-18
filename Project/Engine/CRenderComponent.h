#pragma once
#include "CComponent.h"

#include "assets.h"

class CRenderComponent : public CComponent
{
public:
	CRenderComponent(COMPONENT_TYPE type);
	CRenderComponent(const CRenderComponent& other);
	virtual ~CRenderComponent();

	virtual void Render() = 0;

private:
	Ptr<CMesh>					m_Mesh;

	Ptr<CMaterial>				m_CurMtrl;
	Ptr<CMaterial>				m_SharedMtrl;
	Ptr<CMaterial>				m_DynamicMtrl;

public:
	inline void SetMesh(Ptr<CMesh> mesh) { m_Mesh = mesh; }
	inline Ptr<CMesh> GetMesh() const { return m_Mesh; }

	void SetMaterial(Ptr<CMaterial> material);
	inline Ptr<CMaterial> GetMaterial() const { return m_CurMtrl; }

	Ptr<CMaterial> GetDynamicMaterial();
	inline void RestoreMaterial() { m_CurMtrl = m_SharedMtrl; m_DynamicMtrl = nullptr; }

};

