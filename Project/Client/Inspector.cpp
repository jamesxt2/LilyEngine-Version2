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

Inspector::Inspector()
	: EditorUI("Inspector", "##Inspector"), m_TargetObject(nullptr), m_arrCompUI{}
{
	CreateComponentUI();
}

Inspector::~Inspector()
{
}

void Inspector::Render_Tick()
{
	if (m_TargetObject == nullptr)
	{
		CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
		if (pLevel != nullptr)
			if (CGameObject* pTarget = pLevel->FindObjectByName(L"Player"); pTarget)
				SetTargetObject(pTarget);
		return;
	}

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
		m_arrCompUI[i]->SetActive(true);
		m_arrCompUI[i]->SetSeparate(true);
		AddChildUI(m_arrCompUI[i]);
	}
}

void Inspector::SetTargetObject(CGameObject* obj)
{
	m_TargetObject = obj;

	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (m_arrCompUI[i] == nullptr)
			continue;
		m_arrCompUI[i]->SetTargetObject(obj);
	}
}