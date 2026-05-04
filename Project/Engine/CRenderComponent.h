#pragma once
#include "CComponent.h"

#include "assets.h"

class CRenderComponent : public CComponent
{
public:
	CRenderComponent(COMPONENT_TYPE type);
	virtual ~CRenderComponent();

	virtual void Render() = 0;

private:
	Ptr<CMesh>					m_Mesh;
	Ptr<CGraphicsShader>		m_Shader;
	Ptr<CTexture>				m_Tex;

public:
	inline void SetMesh(Ptr<CMesh> mesh) { m_Mesh = mesh; }
	inline void SetShader(Ptr<CGraphicsShader> shader) { m_Shader = shader; }
	inline Ptr<CMesh> GetMesh() const { return m_Mesh; }
	inline Ptr<CGraphicsShader> GetShader() const { return m_Shader; }
	inline void SetTexture(Ptr<CTexture> tex) { m_Tex = tex; }
	inline Ptr<CTexture> GetTexture() const { return m_Tex; }
};

