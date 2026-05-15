#include "pch.h"
#include "CLevelMgr.h"

#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"

#include "components.h"

#include "CPlayerScript.h"
#include "CCameraMoveScript.h"

#include "CCollisionMgr.h"

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

	m_CurLevel->GetLayer(0)->SetName(L"Default");
	m_CurLevel->GetLayer(1)->SetName(L"Player");
	m_CurLevel->GetLayer(2)->SetName(L"Monster");

	// Camera object
	CGameObject* pCamera = new CGameObject;
	pCamera->SetName(L"MainCamera");
	pCamera->AddComponent(new CTransform);
	pCamera->AddComponent(new CCamera);
	pCamera->AddComponent(new CCameraMoveScript);
	pCamera->GetCameraComp()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);

	pCamera->GetCameraComp()->LayerCheckAll();
	pCamera->GetCameraComp()->SetCameraPriority(0);

	m_CurLevel->AddObject(0, pCamera);

	// Player object
	CGameObject* pPlayer = new CGameObject;
	pPlayer->SetName(L"Player");
	pPlayer->AddComponent(new CTransform);
	pPlayer->AddComponent(new CMeshRender);
	pPlayer->AddComponent(new CCollision2D);
	pPlayer->AddComponent(new CPlayerScript);

	pPlayer->GetTransformComp()->SetRelativePosition(0.f, 0.f, 100.f);
	pPlayer->GetTransformComp()->SetRelativeScale(100.f, 100.f, 1.f);

	pPlayer->GetMeshRenderComp()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pPlayer->GetMeshRenderComp()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMaterial"));
	pPlayer->GetMeshRenderComp()->GetMaterial()->SetScalarParam<int>(INT_0, 0);
	pPlayer->GetMeshRenderComp()->GetMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"texture/mountain.png"));

	//pObject->GetCollision2DComp()->SetAbsolute(true);
	pPlayer->GetCollision2DComp()->SetOffset(Vec3(0.f, 0.f, 0.f));
	pPlayer->GetCollision2DComp()->SetScale(Vec3(0.8f, 0.8f, 1.f));

	

	// Monster object
	CGameObject* pMonster = new CGameObject;
	pMonster->SetName(L"Monster");
	pMonster->AddComponent(new CTransform);
	pMonster->AddComponent(new CMeshRender);
	pMonster->AddComponent(new CCollision2D);

	pMonster->GetTransformComp()->SetRelativePosition(500.f, 0.f, 300.f);
	pMonster->GetTransformComp()->SetRelativeScale(200.f, 200.f, 1.f);
	pMonster->GetTransformComp()->SetAbsolute(true);

	pMonster->GetMeshRenderComp()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pMonster->GetMeshRenderComp()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMaterial"));
	pMonster->GetMeshRenderComp()->GetMaterial()->SetScalarParam<int>(INT_0, 0);
	pMonster->GetMeshRenderComp()->GetMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"texture/mountain.png"));

	//pObject->GetCollision2DComp()->SetAbsolute(true);
	pMonster->GetCollision2DComp()->SetOffset(Vec3(0.f, 0.f, 0.f));
	pMonster->GetCollision2DComp()->SetScale(Vec3(0.8f, 0.8f, 1.f));

	pPlayer->AddChild(pMonster);
	m_CurLevel->AddObject(0, pPlayer);

	CCollisionMgr::GetInst()->LayerCheck(1, 2);
}

void CLevelMgr::Tick()
{
	if (m_CurLevel != nullptr)
	{
		m_CurLevel->Tick();
		m_CurLevel->FinalTick();
	}
}
