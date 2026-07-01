#include "pch.h"
#include "CTestLevel.h"

#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/components.h>
#include <Scripts/CPlayerScript.h>
#include <Scripts/CCameraMoveScript.h>
#include <Scripts/CBackgroundScript.h>
#include <Engine/CCollisionMgr.h>
#include <Engine/CAnim2D.h>
#include <Engine/CPrefab.h>
#include <Scripts/CMissileScript.h>

void CTestLevel::CreateTestLevel()
{
	CLevel* pLevel = new CLevel;

	pLevel->GetLayer(0)->SetName(L"Default");
	pLevel->GetLayer(1)->SetName(L"Player");
	pLevel->GetLayer(2)->SetName(L"Monster");

	// Camera object
	CGameObject* pCamera = new CGameObject;
	pCamera->SetName(L"MainCamera");
	pCamera->AddComponent(new CTransform);
	pCamera->AddComponent(new CCamera);
	pCamera->AddComponent(new CCameraMoveScript);
	pCamera->GetCameraComp()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);

	pCamera->GetCameraComp()->LayerCheckAll();
	pCamera->GetCameraComp()->SetCameraPriority(0);

	pLevel->AddObject(0, pCamera);

	// Light2D
	CGameObject* pLight2D = new CGameObject;

	pLight2D->AddComponent(new CTransform);
	pLight2D->AddComponent(new CLight2D);

	pLight2D->GetTransformComp()->SetRelativePosition(Vec3(-500.f, 0.f, 0.f));

	pLight2D->GetLight2DComp()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pLight2D->GetLight2DComp()->SetDiffuse(Vec3(1.f, 1.f, 1.f));
	pLight2D->GetLight2DComp()->SetAmbient(Vec3(0.f, 0.f, 0.f));

	pLevel->AddObject(0, pLight2D);

	// Background
	CGameObject* pBackground = new CGameObject;
	pBackground->AddComponent(new CTransform);
	pBackground->AddComponent(new CMeshRender);
	//pBackground->AddComponent(new CBackgroundScript);

	pBackground->GetTransformComp()->SetRelativePosition(Vec3(0.f, 0.f, 500.f));
	pBackground->GetTransformComp()->SetRelativeScale(Vec3(1600.f, 900.f, 1.f));

	pBackground->GetMeshRenderComp()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pBackground->GetMeshRenderComp()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"BackgroundMaterial"));
	pBackground->GetMeshRenderComp()->GetMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"texture/mountain.png", L"texture/mountain.png"));
	pBackground->GetMeshRenderComp()->GetMaterial()->SetTexParam(TEX_1, CAssetMgr::GetInst()->Load<CTexture>(L"texture/noise/noise_03.jpg", L"texture/noise/noise_03.jpg"));

	CGameObject* pBGClone = pBackground->Clone();
	pBGClone->GetTransformComp()->SetRelativePosition(0.f, 0.f, 1000.f);

	pLevel->AddObject(0, pBackground);
	pLevel->AddObject(0, pBGClone);

	// Player object

	CGameObject* pPlayer = new CGameObject;
	pPlayer->SetName(L"Player");
	pPlayer->AddComponent(new CTransform);
	pPlayer->AddComponent(new CMeshRender);
	pPlayer->AddComponent(new CCollision2D);
	pPlayer->AddComponent(new CAnimator2D);
	pPlayer->AddComponent(new CPlayerScript);

	pPlayer->GetTransformComp()->SetRelativePosition(0.f, 200.f, 100.f);
	pPlayer->GetTransformComp()->SetRelativeScale(100.f, 100.f, 1.f);

	pPlayer->GetMeshRenderComp()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pPlayer->GetMeshRenderComp()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMaterial"));
	//pPlayer->GetMeshRenderComp()->GetMaterial()->SetScalarParam<int>(INT_0, 0);
	//pPlayer->GetMeshRenderComp()->GetMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"texture/mountain.png"));

	pPlayer->GetCollision2DComp()->SetAbsolute(true);
	pPlayer->GetCollision2DComp()->SetOffset(Vec3(0.f, 0.f, 0.f));
	pPlayer->GetCollision2DComp()->SetScale(Vec3(150.f, 150.f, 1.f));

	pPlayer->GetAnimator2DComp()->LoadAnimation(L"animation/MOVE_DOWN.anim");

	pPlayer->GetAnimator2DComp()->Play(L"MOVE_DOWN", true);

	pLevel->AddObject(0, pPlayer);

	// Particle System
	CreatePrefab();

	// TileMap
	CGameObject* pTileMap = new CGameObject;
	pTileMap->SetName(L"TileMap");
	pTileMap->AddComponent(new CTransform);
	pTileMap->AddComponent(new CTileMap);

	pTileMap->GetTileMapComp()->SetAtlasTexture(CAssetMgr::GetInst()->Load<CTexture>(L"texture/TILE.bmp", L"texture/TILE.bmp"));
	pTileMap->GetTileMapComp()->SetRowCol(8, 8);
	pTileMap->GetTileMapComp()->SetTileEachSize(Vec2(64.f, 64.f));
	pTileMap->GetTileMapComp()->SetAtlasTileEachSize(Vec2(64.f, 64.f));
	pTileMap->GetTransformComp()->SetRelativePosition(-500.f, 0.f, 100.f);

	CGameObject* pTileMapClone = pTileMap->Clone();
	pTileMapClone->GetTransformComp()->SetRelativePosition(500.f, 0.f, 100.f);

	pLevel->AddObject(0, pTileMap);
	pLevel->AddObject(0, pTileMapClone);

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

	pLevel->AddObject(0, pFilterObj);

	CCollisionMgr::GetInst()->LayerCheck(1, 1);
	CCollisionMgr::GetInst()->LayerCheck(1, 2);

	ChangeLevel(pLevel, LEVEL_STATE::PLAY);
}

void CTestLevel::CreatePrefab()
{
	CGameObject* pParticle = new CGameObject;
	pParticle->SetName(L"Particle");
	pParticle->AddComponent(new CTransform);
	pParticle->AddComponent(new CParticleSystem);

	pParticle->GetTransformComp()->SetRelativePosition(Vec3(0.f, 0.f, 50.f));
	pParticle->GetParticleSystemComp()->SetParticleTexture(CAssetMgr::GetInst()->Load<CTexture>(L"texture/particle/AlphaCircle.png", L"texture/particle/AlphaCircle.png"));

	CAssetMgr::GetInst()->AddAsset<CPrefab>(L"ParticlePrefab", new CPrefab(pParticle));

	CGameObject* pMissile = new CGameObject;
	pMissile->AddComponent(new CTransform);
	pMissile->AddComponent(new CMeshRender);
	pMissile->AddComponent(new CMissileScript);
	pMissile->GetTransformComp()->SetRelativeScale(40.f, 40.f, 40.f);
	pMissile->GetMeshRenderComp()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pMissile->GetMeshRenderComp()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMaterial"));

	CAssetMgr::GetInst()->AddAsset<CPrefab>(L"MissilePrefab", new CPrefab(pMissile));
}