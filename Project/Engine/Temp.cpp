#include "pch.h"
#include "Temp.h"

#include "CDevice.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CPathMgr.h"
#include "CAssetMgr.h"

#include "CMesh.h"
#include "CConstBuffer.h"
#include "CGraphicsShader.h"

#include "CGameObject.h"
#include "CTransform.h"
#include "CMeshRender.h"
#include "CPlayerScript.h"

CGameObject* pObject1 = nullptr;
CGameObject* pObject2 = nullptr;


int TempInit()
{
	pObject1 = new CGameObject;
	pObject1->AddComponent(new CTransform);
	pObject1->AddComponent(new CMeshRender);
	pObject1->AddComponent(new CPlayerScript);
		   
	pObject1->GetTransformComp()->SetRelativeScale(.2f, .2f, .2f);
		   
	pObject1->GetMeshRenderComp()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pObject1->GetMeshRenderComp()->SetShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"Std2DShader"));


	pObject2 = new CGameObject;
	pObject2->AddComponent(new CTransform);
	pObject2->AddComponent(new CMeshRender);

	pObject2->GetTransformComp()->SetRelativeScale(.4f, .4f, .4f);

	pObject2->GetMeshRenderComp()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pObject2->GetMeshRenderComp()->SetShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"Std2DShader"));

	return S_OK;
}

void TempRelease()
{
	delete pObject1;
	delete pObject2;
}

void TempTick()
{
	pObject1->Tick();
	pObject2->Tick();

	pObject1->FinalTick();
	pObject2->FinalTick();
}

void TempRender()
{
	pObject1->Render();
	pObject2->Render();
}