#include "pch.h"
#include "ContentUI.h"

#include "TreeUI.h"
#include <Engine/CAssetMgr.h>
#include "OutlinerUI.h"
#include "Inspector.h"

ContentUI::ContentUI()
	: EditorUI("Content", "##Content"), m_Tree(nullptr)
{
	m_Tree = new TreeUI("ContentTree");
	AddChildUI(m_Tree);

	m_Tree->ShowRoot(false);
	m_Tree->ShowFileName(true);

	m_Tree->RegisterSelectedDelegate(this, (UI_DELEGATE_PARAM_1)&ContentUI::SelectAsset);

	RenewContent();
}

ContentUI::~ContentUI()
{
	if (m_Tree != nullptr)
		delete m_Tree;
}

void ContentUI::RenewContent()
{
	m_Tree->Clear();

	TreeNode* pRootNode = m_Tree->AddTreeNode(nullptr, "DummyRoot");

	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		TreeNode* pCategoryNode = m_Tree->AddTreeNode(pRootNode, ASSET_TYPE_STRING[i]);
		pCategoryNode->SetFrame(true);

		const std::map<std::wstring, Ptr<CAsset>>& mapAsset = CAssetMgr::GetInst()->GetAssets((ASSET_TYPE)i);
		for (const auto& pair : mapAsset)
		{
			m_Tree->AddTreeNode(pCategoryNode, WStringToString(pair.first), (DWORD_PTR)pair.second.Get());
		}
	}
}

void ContentUI::Render_Tick()
{

}

UINT ContentUI::SelectAsset(DWORD_PTR data)
{
	TreeNode* pSelectedNode = (TreeNode*)data;

	Ptr<CAsset> pAsset = (CAsset*)pSelectedNode->GetData();

	Inspector* pInspector = CImGuiMgr::GetInst()->FindEditorUI<Inspector>("Inspector");

	pInspector->SetTargetAsset(pAsset);

	return 0;
}
