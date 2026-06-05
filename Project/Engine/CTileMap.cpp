#include "pch.h"
#include "CTileMap.h"

#include "CAssetMgr.h"
#include "CTransform.h"
#include "CStructuredBuffer.h"

CTileMap::CTileMap()
	: CRenderComponent(COMPONENT_TYPE::TILEMAP),
	m_Row(1), m_Col(1), m_TileEachSize(Vec2(32.f, 32.f)), m_AtlasMaxRow(1), m_AtlasMaxCol(1),
	m_vecTileInfo{}
{
	SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"TileMapMaterial"));

	//SetRowCol(m_Row, m_Col);

	m_TileBuffer = new CStructuredBuffer;
}

CTileMap::~CTileMap()
{
	delete m_TileBuffer;
}

void CTileMap::Begin()
{
	//SetRowCol(m_Row, m_Col);
}

void CTileMap::FinalTick()
{
}

void CTileMap::Render()
{
	GetOwner()->GetTransformComp()->Bind();

	GetMaterial()->SetTexParam(TEX_0, m_Atlas);
	GetMaterial()->SetScalarParam(VEC2_0, m_AtlasResolution);
	GetMaterial()->SetScalarParam(VEC2_1, m_AtlasTileEachSize);
	GetMaterial()->SetScalarParam(VEC2_2, Vec2(m_Col, m_Row));
	GetMaterial()->SetScalarParam(INT_0, m_AtlasMaxRow);
	GetMaterial()->SetScalarParam(INT_1, m_AtlasMaxCol);
	GetMaterial()->Bind();

	if (m_TileBuffer->GetElementCount() < m_vecTileInfo.size())
		m_TileBuffer->Create(sizeof(TTileInfo), (UINT)m_vecTileInfo.size(), SB_TYPE::SRV_ONLY, true, m_vecTileInfo.data());
	else
		m_TileBuffer->SetData(m_vecTileInfo.data(), (UINT)m_vecTileInfo.size());
	m_TileBuffer->Bind(20);

	GetMesh()->Render();
}

void CTileMap::SetRowCol(UINT row, UINT col)
{
	m_Row = row;
	m_Col = col;
	GetOwner()->GetTransformComp()->SetRelativeScale(Vec3(m_TileEachSize.x * m_Col, m_TileEachSize.y * m_Row, 1.f));

	m_vecTileInfo.clear();
	m_vecTileInfo.resize((size_t)(row * col));

	for (size_t i = 0; i < m_vecTileInfo.size(); ++i)
		m_vecTileInfo[i].ImgIdx = 7;
}

void CTileMap::SetAtlasTexture(const Ptr<CTexture>& tex)
{
	m_Atlas = tex;
	m_AtlasResolution = Vec2(m_Atlas->GetWidth(), m_Atlas->GetHeight());
}

void CTileMap::SetTileEachSize(Vec2 size)
{
	m_TileEachSize = size;
	GetOwner()->GetTransformComp()->SetRelativeScale(Vec3(m_TileEachSize.x * m_Col, m_TileEachSize.y * m_Row, 1.f));
}

void CTileMap::SetAtlasTileEachSize(Vec2 size)
{
	assert(m_Atlas.Get());  

	m_AtlasTileEachSize = size;
	m_AtlasMaxCol = (UINT)(m_Atlas->GetWidth() / m_AtlasTileEachSize.x);
	m_AtlasMaxRow = (UINT)(m_Atlas->GetHeight() / m_AtlasTileEachSize.y);
}
