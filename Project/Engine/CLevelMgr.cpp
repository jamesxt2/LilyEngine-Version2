#include "pch.h"
#include "CLevelMgr.h"

#include "CLevel.h"
#include "CGameObject.h"

#include "components.h"

#include "CPlayerScript.h"
#include "CCameraMoveScript.h"

CLevelMgr::CLevelMgr()
	: m_CurLevel(nullptr)
{

}

CLevelMgr::~CLevelMgr()
{
	if (m_CurLevel != nullptr)
		delete m_CurLevel;
}

void CLevelMgr::Init()
{
	m_CurLevel = new CLevel;

	// Camera object
	CGameObject* pCamera = new CGameObject;
	pCamera->SetName(L"MainCamera");
	pCamera->AddComponent(new CTransform);
	pCamera->AddComponent(new CCamera);
	pCamera->AddComponent(new CCameraMoveScript);
	pCamera->GetCameraComp()->SetCameraPriority(0);

	m_CurLevel->AddObject(0, pCamera);

	// Player object
	CGameObject* pObject = new CGameObject;
	pObject->SetName(L"Player");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new CPlayerScript);

	pObject->GetTransformComp()->SetRelativePosition(0.f, 0.f, 100.f);
	pObject->GetTransformComp()->SetRelativeScale(100.f, 100.f, 0.5f);

	pObject->GetMeshRenderComp()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pObject->GetMeshRenderComp()->SetShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"Std2DShader"));
	pObject->GetMeshRenderComp()->SetTexture(CAssetMgr::GetInst()->FindAsset<CTexture>(L"texture/pistol.png"));

	m_CurLevel->AddObject(0, pObject);

}

void CLevelMgr::Tick()
{
	if (m_CurLevel != nullptr)
	{
		m_CurLevel->Tick();
		m_CurLevel->FinalTick();
	}
}

void CLevelMgr::Render()
{
	if (m_CurLevel != nullptr)
		m_CurLevel->Render();
}