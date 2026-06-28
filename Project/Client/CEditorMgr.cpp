#include "pch.h"
#include "CEditorMgr.h"

#include <Engine/CGameObject.h>
#include <Engine/components.h>
#include <Engine/CRenderMgr.h>
#include "CGameObjectEx.h"
#include "CEditorCameraScript.h"

CEditorMgr::CEditorMgr()
{

}

CEditorMgr::~CEditorMgr()
{
	Safe_Del_Vector(m_vecEditorObj);
}

void CEditorMgr::Init()
{
	CGameObject* pEditorCamera = new CGameObjectEx;
	pEditorCamera->AddComponent(new CTransform);
	pEditorCamera->AddComponent(new CCamera);
	pEditorCamera->AddComponent(new CEditorCameraScript);

	pEditorCamera->GetCameraComp()->LayerCheckAll();
	pEditorCamera->GetCameraComp()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);

	CRenderMgr::GetInst()->RegisterEditorCamera(pEditorCamera->GetCameraComp());

	m_vecEditorObj.push_back(pEditorCamera);
}

void CEditorMgr::Tick()
{
	for (size_t i = 0; i < m_vecEditorObj.size(); ++i)
		m_vecEditorObj[i]->Tick();
	for (size_t i = 0; i < m_vecEditorObj.size(); ++i)
		m_vecEditorObj[i]->FinalTick();
}