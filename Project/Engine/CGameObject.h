#pragma once
#include "CEntity.h"

class CComponent;
class CRenderComponent;
class CTransform;
class CMeshRender;
class CScript;

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
	inline CComponent* GetComponent(COMPONENT_TYPE type) { return m_arrCom[(UINT)type]; }
	inline CTransform* GetTransformComp() { return (CTransform*)m_arrCom[(UINT)COMPONENT_TYPE::TRANSFORM]; }
	inline CMeshRender* GetMeshRenderComp() { return (CMeshRender*)m_arrCom[(UINT)COMPONENT_TYPE::MESHRENDER]; }

private:
	CComponent*				m_arrCom[(UINT)COMPONENT_TYPE::END];
	CRenderComponent*		m_RenderCom;
	std::vector<CScript*>	m_vecScript;
};

