#pragma once
#include "CEntity.h"

class CComponent;
class CRenderComponent;
class CTransform;
class CMeshRender;
class CScript;
class CCamera;
class CCollision2D;
class CAnimator2D;
class CLight2D;

class CGameObject : public CEntity
{
public:
	CGameObject();
	~CGameObject();
	friend class CLayer;
	friend class CTaskMgr;

	void Begin();
	void Tick();
	void FinalTick();
	void Render();

	void AddComponent(CComponent* component);

	void AddChild(CGameObject* obj);

	void Destroy();

private:
	CComponent*					m_arrComp[(UINT)COMPONENT_TYPE::END];
	CRenderComponent*			m_RenderComp;
	std::vector<CScript*>		m_vecScript;

	CGameObject* m_Parent;
	std::vector<CGameObject*>	m_vecChild;

	int							m_LayerIdx;

	bool						m_Dead;

private:
	inline void SetLayerIdx(int idx) { m_LayerIdx = idx; }

public:
	inline CComponent* GetComponent(COMPONENT_TYPE type) const { return m_arrComp[(UINT)type]; }
	inline CTransform* GetTransformComp() const { return (CTransform*)m_arrComp[(UINT)COMPONENT_TYPE::TRANSFORM]; }
	inline CRenderComponent* GetRenderComp() const { return m_RenderComp; }
	inline CMeshRender* GetMeshRenderComp() const { return (CMeshRender*)m_arrComp[(UINT)COMPONENT_TYPE::MESHRENDER]; }
	inline CCamera* GetCameraComp() const { return (CCamera*)m_arrComp[(UINT)COMPONENT_TYPE::CAMERA]; }
	inline CCollision2D* GetCollision2DComp() const { return (CCollision2D*)m_arrComp[(UINT)COMPONENT_TYPE::COLLISION2D]; }
	inline CAnimator2D* GetAnimator2DComp() const { return (CAnimator2D*)m_arrComp[(UINT)COMPONENT_TYPE::ANIMATOR2D]; }
	inline CLight2D* GetLight2DComp() const { return (CLight2D*)m_arrComp[(UINT)COMPONENT_TYPE::LIGHT2D]; }

	inline const std::vector<CGameObject*>& GetChild() const { return m_vecChild; }
	inline CGameObject* GetParent() const { return m_Parent; }

	inline const std::vector<CScript*>& GetScript() const { return m_vecScript; }

	inline bool IsDead() const { return m_Dead; }

	template<typename T>
	T* GetScript();
};

template<typename T>
inline T* CGameObject::GetScript()
{
	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		if (dynamic_cast<T*>(m_vecScript[i]))
			return (T*)m_vecScript[i];
	}

	return nullptr;
}
