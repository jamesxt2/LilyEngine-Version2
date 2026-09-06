#pragma once
#include "EditorUI.h"

class AssetUI : public EditorUI
{
public:
	AssetUI(const std::string& name, const std::string& ID, ASSET_TYPE type);
	~AssetUI();

	virtual void Render_Tick() = 0;

protected:
	void Render_Title();

private:
	Ptr<CAsset> m_TargetAsset;
	const ASSET_TYPE m_Type;

public:
	inline void SetTarget(Ptr<CAsset> target) { m_TargetAsset = target; }
	inline Ptr<CAsset> GetTarget() const { return m_TargetAsset; }

	inline ASSET_TYPE GetAssetType() const { return m_Type; }
};

