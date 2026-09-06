#pragma once
#include "EditorUI.h"

class CGameObject;
class ComponentUI;
class AssetUI;
class CAsset;

class Inspector : public EditorUI
{
public:
	Inspector();
	~Inspector();

	virtual void Render_Tick() override;

private:
	void CreateComponentUI();
	void CreateAssetUI();

private:
	CGameObject* m_TargetObject;
	ComponentUI* m_arrCompUI[(UINT)COMPONENT_TYPE::END];

	Ptr<CAsset> m_TargetAsset;
	AssetUI* m_arrAssetUI[(UINT)ASSET_TYPE::END];

public:
	void SetTargetObject(CGameObject* obj);
	inline CGameObject* GetTargetObject() const { return m_TargetObject; }
	void SetTargetAsset(Ptr<CAsset> asset);
	inline Ptr<CAsset> GetTargetAsset() const { return m_TargetAsset; }
};

