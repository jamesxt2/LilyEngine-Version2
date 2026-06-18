#pragma once
#include "CRenderComponent.h"

class CStructuredBuffer;

struct TTileInfo
{
	UINT ImgIdx{ 0 };
	Vec3 vPadding;
};

class CTileMap : public CRenderComponent
{
public:
	CTileMap();
	CTileMap(const CTileMap& other);
	~CTileMap();
	CLONE(CTileMap)

	virtual void Begin() override;
	virtual void FinalTick() override;
	virtual void Render() override;

private:
	UINT						m_Row;
	UINT						m_Col;
	Vec2						m_TileEachSize;

	Ptr<CTexture>				m_Atlas;

	UINT						m_AtlasMaxRow;
	UINT						m_AtlasMaxCol;
	Vec2						m_AtlasTileEachSize;
	Vec2						m_AtlasResolution;

	std::vector<TTileInfo>		m_vecTileInfo;
	CStructuredBuffer*			m_TileBuffer;

public:
	void SetRowCol(UINT row, UINT col);
	void SetAtlasTexture(const Ptr<CTexture>& tex);
	void SetTileEachSize(Vec2 size);
	void SetAtlasTileEachSize(Vec2 size);
};

