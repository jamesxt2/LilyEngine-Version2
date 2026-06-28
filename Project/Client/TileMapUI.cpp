#include "pch.h"
#include "TileMapUI.h"

TileMapUI::TileMapUI()
	: ComponentUI("TileMap", "##TileMapUI", COMPONENT_TYPE::TILEMAP)
{
	SetChildSize(ImVec2(0.f, 300.f));
}

TileMapUI::~TileMapUI()
{
}

void TileMapUI::Render_Tick()
{
	Render_Title();
}
