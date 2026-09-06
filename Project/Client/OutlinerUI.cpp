#include "pch.h"
#include "OutlinerUI.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/CTaskMgr.h>
#include "Inspector.h"
#include "TreeUI.h"

OutlinerUI::OutlinerUI()
	: EditorUI("Outliner", "##Outliner")
{
	m_Tree = new TreeUI("OutlinerTree");
	AddChildUI(m_Tree);

	m_Tree->ShowRoot(false);

	m_Tree->RegisterSelectedDelegate(this, (UI_DELEGATE_PARAM_1)&OutlinerUI::SelectGameObject);

	RenewGameObject();
}

OutlinerUI::~OutlinerUI()
{
	if (m_Tree != nullptr)
		delete m_Tree;
}

void OutlinerUI::RenewGameObject()
{
	m_Tree->Clear();
	TreeNode* pRootNode = m_Tree->AddTreeNode(nullptr, "DummyRoot");

	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	if (pCurLevel == nullptr) return;

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = pCurLevel->GetLayer(i);
		const std::vector<CGameObject*>& vecParent = pLayer->GetParentObjects();
		for (size_t j = 0; j < vecParent.size(); ++j)
		{
			AddGameObject(pRootNode, vecParent[j]);
		}
	}
}

void OutlinerUI::Render_Tick()
{
	if (CTaskMgr::GetInst()->IsObjectChanged())
		RenewGameObject();
}

void OutlinerUI::AddGameObject(TreeNode* node, CGameObject* object)
{
	TreeNode* pNewNode = m_Tree->AddTreeNode(node, WStringToString(object->GetName()), (DWORD_PTR)object);
	
	const std::vector<CGameObject*>& vecChild = object->GetChild();
	for (size_t i = 0; i < vecChild.size(); ++i)
	{
		AddGameObject(pNewNode, vecChild[i]);
	}
}

UINT OutlinerUI::SelectGameObject(DWORD_PTR data)
{
	TreeNode* pSelectedNode = (TreeNode*)data;

	CGameObject* pSelectedObj = (CGameObject*)pSelectedNode->GetData();

	Inspector* pInspector = CImGuiMgr::GetInst()->FindEditorUI<Inspector>("Inspector");

	pInspector->SetTargetObject(pSelectedObj);

	return 0;
}
