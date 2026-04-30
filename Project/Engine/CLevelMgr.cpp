#include "pch.h"
#include "CLevelMgr.h"

#include "CLevel.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CMeshRender.h"
#include "CPlayerScript.h"

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

	CGameObject* pObject = new CGameObject;
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new CPlayerScript);

	pObject->GetTransformComp()->SetRelativeScale(.2f, .2f, .2f);

	pObject->GetMeshRenderComp()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pObject->GetMeshRenderComp()->SetShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"Std2DShader"));

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