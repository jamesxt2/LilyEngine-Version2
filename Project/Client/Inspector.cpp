#include "pch.h"
#include "Inspector.h"

#include <Engine/CLevel.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CGameObject.h>
#include <Engine/components.h>

#include "TransformUI.h"
#include "MeshRenderUI.h"
#include "Collision2DUI.h"
#include "Animator2DUI.h"
#include "CameraUI.h"
#include "Light2DUI.h"
#include "TileMapUI.h"
#include "ParticleSystemUI.h"

#include "MeshUI.h"
#include "MeshDataUI.h"
#include "MaterialUI.h"
#include "TextureUI.h"
#include "PrefabUI.h"
#include "SoundUI.h"
#include "GraphicsShaderUI.h"
#include "ComputeShaderUI.h"


Inspector::Inspector()
	: EditorUI("Inspector", "##Inspector"), m_TargetObject(nullptr), m_arrCompUI{}
{
	CreateComponentUI();
	CreateAssetUI();
}

Inspector::~Inspector()
{
}

void Inspector::Render_Tick()
{
	ImGui::Text("Inspector");
}

void Inspector::CreateComponentUI()
{
	m_arrCompUI[(UINT)COMPONENT_TYPE::TRANSFORM] = new TransformUI;
	m_arrCompUI[(UINT)COMPONENT_TYPE::COLLISION2D] = new Collision2DUI;
	m_arrCompUI[(UINT)COMPONENT_TYPE::ANIMATOR2D] = new Animator2DUI;
	m_arrCompUI[(UINT)COMPONENT_TYPE::MESHRENDER] = new MeshRenderUI;
	m_arrCompUI[(UINT)COMPONENT_TYPE::CAMERA] = new CameraUI;
	m_arrCompUI[(UINT)COMPONENT_TYPE::LIGHT2D] = new Light2DUI;
	m_arrCompUI[(UINT)COMPONENT_TYPE::TILEMAP] = new TileMapUI;
	m_arrCompUI[(UINT)COMPONENT_TYPE::PARTICLESYSTEM] = new ParticleSystemUI;

	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (m_arrCompUI[i] == nullptr)
			continue;
		m_arrCompUI[i]->SetActive(false);
		m_arrCompUI[i]->SetSeparate(true);
		AddChildUI(m_arrCompUI[i]);
	}
}

void Inspector::CreateAssetUI()
{
	m_arrAssetUI[(UINT)ASSET_TYPE::MESH] = new MeshUI;
	m_arrAssetUI[(UINT)ASSET_TYPE::MESH_DATA] = new MeshDataUI;
	m_arrAssetUI[(UINT)ASSET_TYPE::MATERIAL] = new MaterialUI;
	m_arrAssetUI[(UINT)ASSET_TYPE::TEXTURE] = new TextureUI;
	m_arrAssetUI[(UINT)ASSET_TYPE::PREFAB] = new PrefabUI;
	m_arrAssetUI[(UINT)ASSET_TYPE::SOUND] = new SoundUI;
	m_arrAssetUI[(UINT)ASSET_TYPE::GRAPHICS_SHADER] = new GraphicsShaderUI;
	m_arrAssetUI[(UINT)ASSET_TYPE::COMPUTE_SHADER] = new ComputeShaderUI;

	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		if (m_arrAssetUI[i] == nullptr)
			continue;
		m_arrAssetUI[i]->SetActive(false);
		m_arrAssetUI[i]->SetSeparate(true);
		AddChildUI(m_arrAssetUI[i]);
	}
}

void Inspector::SetTargetObject(CGameObject* obj)
{
	if (m_TargetAsset != nullptr)
	{
		m_arrAssetUI[(UINT)m_TargetAsset->GetAssetType()]->SetActive(false);
		m_TargetAsset = nullptr;
	}

	m_TargetObject = obj;

	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (m_arrCompUI[i] == nullptr)
			continue;
		m_arrCompUI[i]->SetTargetObject(obj);
	}
}

void Inspector::SetTargetAsset(Ptr<CAsset> asset)
{
	SetTargetObject(nullptr);
	m_TargetAsset = asset;

	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		if (m_arrAssetUI[i] == nullptr)
			continue;
		m_arrAssetUI[i]->SetActive(false);
	}

	if (m_TargetAsset == nullptr)
		return;

	ASSET_TYPE assetType = m_TargetAsset->GetAssetType();
	m_arrAssetUI[(UINT)assetType]->SetActive(true);
	m_arrAssetUI[(UINT)assetType]->SetTarget(m_TargetAsset);
}
