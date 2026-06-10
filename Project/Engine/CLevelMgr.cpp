#include "pch.h"
#include "CLevelMgr.h"

#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"

#include "components.h"

#include "CPlayerScript.h"
#include "CCameraMoveScript.h"
#include "CBackgroundScript.h"

#include "CCollisionMgr.h"

#include "CAnim2D.h"

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

	// Light2D
	CGameObject* pLight2D = new CGameObject;

	pLight2D->AddComponent(new CTransform);
	pLight2D->AddComponent(new CLight2D);

	//pLight2D->GetTransformComp()->SetRelativePosition(Vec3(-300.f, 0.f, 0.f));

	pLight2D->GetLight2DComp()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pLight2D->GetLight2DComp()->SetDiffuse(Vec3(1.f, 1.f, 1.f));
	pLight2D->GetLight2DComp()->SetAmbient(Vec3(0.f, 0.f, 0.f));
	pLight2D->GetLight2DComp()->SetRange(500.f);

	m_CurLevel->AddObject(0, pLight2D);

	// Background
	CGameObject* pBackground = new CGameObject;
	pBackground->AddComponent(new CTransform);
	pBackground->AddComponent(new CMeshRender);
	//pBackground->AddComponent(new CBackgroundScript);

	pBackground->GetTransformComp()->SetRelativePosition(Vec3(0.f, 0.f, 200.f));
	pBackground->GetTransformComp()->SetRelativeScale(Vec3(1600.f, 900.f, 1.f));

	pBackground->GetMeshRenderComp()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pBackground->GetMeshRenderComp()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"BackgroundMaterial"));
	pBackground->GetMeshRenderComp()->GetMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"texture/mountain.png", L"texture/mountain.png"));
	pBackground->GetMeshRenderComp()->GetMaterial()->SetTexParam(TEX_1, CAssetMgr::GetInst()->Load<CTexture>(L"texture/noise/noise_03.jpg", L"texture/noise/noise_03.jpg"));

	m_CurLevel->AddObject(0, pBackground);
	
	// Player object
	
	CGameObject* pPlayer = new CGameObject;
	pPlayer->SetName(L"Player");
	pPlayer->AddComponent(new CTransform);
	pPlayer->AddComponent(new CMeshRender);
	//pPlayer->AddComponent(new CCollision2D);
	pPlayer->AddComponent(new CAnimator2D);
	pPlayer->AddComponent(new CPlayerScript);

	pPlayer->GetTransformComp()->SetRelativePosition(0.f, 200.f, 100.f);
	pPlayer->GetTransformComp()->SetRelativeScale(100.f, 100.f, 1.f);

	pPlayer->GetMeshRenderComp()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pPlayer->GetMeshRenderComp()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMaterial"));
	pPlayer->GetMeshRenderComp()->GetMaterial()->SetScalarParam<int>(INT_0, 0);
	pPlayer->GetMeshRenderComp()->GetMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"texture/mountain.png"));

	//pPlayer->GetCollision2DComp()->SetOffset(Vec3(0.f, 0.f, 0.f));
	//pPlayer->GetCollision2DComp()->SetScale(Vec3(1.f, 1.f, 1.f));

	pPlayer->GetAnimator2DComp()->LoadAnimation(L"animation/MOVE_DOWN.anim");

	pPlayer->GetAnimator2DComp()->Play(L"MOVE_DOWN", true);

	m_CurLevel->AddObject(1, pPlayer);
	
	// Monster object
	/*
	CGameObject* pMonster = new CGameObject;
	pMonster->SetName(L"Monster");
	pMonster->AddComponent(new CTransform);
	pMonster->AddComponent(new CMeshRender);
	pMonster->AddComponent(new CCollision2D);

	pMonster->GetTransformComp()->SetRelativePosition(10.f, 0.f, 100.f);
	pMonster->GetTransformComp()->SetRelativeScale(200.f, 200.f, 1.f);
	pMonster->GetTransformComp()->SetAbsolute(true);

	pMonster->GetMeshRenderComp()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pMonster->GetMeshRenderComp()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMaterial"));
	pMonster->GetMeshRenderComp()->GetMaterial()->SetScalarParam<int>(INT_0, 0);
	pMonster->GetMeshRenderComp()->GetMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"texture/mountain.png"));
	pMonster->GetCollision2DComp()->SetOffset(Vec3(0.f, 0.f, 0.f));
	pMonster->GetCollision2DComp()->SetScale(Vec3(1.f, 1.f, 1.f));

	m_CurLevel->AddObject(1, pMonster);
	
	//pPlayer->GetScript<CPlayerScript>()->SetTarget(pMonster);
	
	*/
	// Particle System
	CGameObject* pParticle = new CGameObject;
	pParticle->SetName(L"Particle");
	pParticle->AddComponent(new CTransform);
	pParticle->AddComponent(new CParticleSystem);

	pParticle->GetTransformComp()->SetRelativePosition(Vec3(0.f, 0.f, 50.f));
	
	pParticle->GetParticleSystemComp()->SetParticleTexture(CAssetMgr::GetInst()->Load<CTexture>(L"texture/particle/AlphaCircle.png", L"texture/particle/AlphaCircle.png"));

	m_CurLevel->AddObject(0, pParticle);

	// PostProcess Filter
	CGameObject* pFilterObj = new CGameObject;
	pFilterObj->SetName(L"Filter");
	pFilterObj->AddComponent(new CTransform);
	pFilterObj->AddComponent(new CMeshRender);

	pFilterObj->GetTransformComp()->SetRelativePosition(500.f, 0.f, 50.f);
	pFilterObj->GetTransformComp()->SetRelativeScale(400.f, 400.f, 1.f);

	pFilterObj->GetMeshRenderComp()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pFilterObj->GetMeshRenderComp()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DistortionMaterial"));
	pFilterObj->GetMeshRenderComp()->GetMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"RenderTargetCopyTex"));
	pFilterObj->GetMeshRenderComp()->GetMaterial()->SetTexParam(TEX_1, CAssetMgr::GetInst()->Load<CTexture>(L"texture/noise/noise_03.jpg", L"texture/noise/noise_03.jpg"));

	m_CurLevel->AddObject(0, pFilterObj);

	CCollisionMgr::GetInst()->LayerCheck(1, 1);
	CCollisionMgr::GetInst()->LayerCheck(1, 2);
	
	m_CurLevel->Begin();
}

void CLevelMgr::Tick()
{
	if (m_CurLevel != nullptr)
	{
		m_CurLevel->Tick();
		m_CurLevel->FinalTick();
	}
}
