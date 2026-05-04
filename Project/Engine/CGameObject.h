#pragma once
#include "CEntity.h"

class CComponent;
class CRenderComponent;
class CTransform;
class CMeshRender;
class CScript;
class CCamera;

class CGameObject : public CEntity
{
public:
	CGameObject();
	~CGameObject();

	void Begin();
	void Tick();
	void FinalTick();
	void Render();

	void AddComponent(CComponent* component);
	

private:
	CComponent*				m_arrComp[(UINT)COMPONENT_TYPE::END];
	CRenderComponent*		m_RenderComp;
	std::vector<CScript*>	m_vecScript;

public:
	inline CComponent* GetComponent(COMPONENT_TYPE type) const { return m_arrComp[(UINT)type]; }
	inline CTransform* GetTransformComp() const { return (CTransform*)m_arrComp[(UINT)COMPONENT_TYPE::TRANSFORM]; }
	inline CMeshRender* GetMeshRenderComp() const { return (CMeshRender*)m_arrComp[(UINT)COMPONENT_TYPE::MESHRENDER]; }
	inline CCamera* GetCameraComp() const { return (CCamera*)m_arrComp[(UINT)COMPONENT_TYPE::CAMERA]; }
};

