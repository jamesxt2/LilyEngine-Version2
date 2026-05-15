#include "pch.h"
#include "CDebugRenderMgr.h"

#include "CTimeMgr.h"
#include "CAssetMgr.h"
#include "CMesh.h"
#include "CMaterial.h"

#include "CTransform.h"
#include "CMeshRender.h"

#include "CGameObject.h"

CDebugRenderMgr::CDebugRenderMgr()
	: m_DebugRenderObj(nullptr)
{
	m_DebugRenderObj = new CGameObject;
	m_DebugRenderObj->AddComponent(new CTransform);
	m_DebugRenderObj->AddComponent(new CMeshRender);

	m_DebugRenderObj->GetMeshRenderComp()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DebugShapeMaterial"));
}

CDebugRenderMgr::~CDebugRenderMgr()
{
	delete m_DebugRenderObj;
	m_DebugRenderObj = nullptr;
}

void CDebugRenderMgr::Render()
{
	std::list<TDebugShapeInfo>::iterator iter = m_ShapeInfo.begin();

	for (; iter != m_ShapeInfo.end(); )
	{
		if (iter->matWorld == XMMatrixIdentity())
		{
			m_DebugRenderObj->GetTransformComp()->SetRelativePosition(iter->Position);
			m_DebugRenderObj->GetTransformComp()->SetRelativeRotation(iter->Rotation);
			m_DebugRenderObj->GetTransformComp()->SetRelativeScale(iter->Scale);
			m_DebugRenderObj->FinalTick();
		}
		else
		{
			m_DebugRenderObj->GetTransformComp()->SetWorldMat(iter->matWorld);
		}

		switch (iter->Shape)
		{
		case DEBUG_SHAPE::RECT:
			m_DebugRenderObj->GetMeshRenderComp()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh_Debug"));
			break;
		case DEBUG_SHAPE::CIRCLE:
			m_DebugRenderObj->GetMeshRenderComp()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CircleMesh_Debug"));
			break;
		case DEBUG_SHAPE::LINE:
			m_DebugRenderObj->GetMeshRenderComp()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"LineMesh"));
			break;
		case DEBUG_SHAPE::CUBE:
			m_DebugRenderObj->GetMeshRenderComp()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CubeMesh"));
			break;
		case DEBUG_SHAPE::SPHERE:
			m_DebugRenderObj->GetMeshRenderComp()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"SphereMesh"));
			break;
		}

		m_DebugRenderObj->GetMeshRenderComp()->GetMaterial()->SetScalarParam(VEC4_0, iter->Color);
		m_DebugRenderObj->Render();

		iter->Age += DT;
		if (iter->Duration <= iter->Age)
		{
			iter = m_ShapeInfo.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}
