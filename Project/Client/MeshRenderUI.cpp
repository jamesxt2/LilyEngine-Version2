#include "pch.h"
#include "MeshRenderUI.h"

#include <Engine/CAssetMgr.h>
#include <Engine/CMeshRender.h>

#include "CImGuiMgr.h"
#include "ListUI.h"
#include "Inspector.h"

void ChangeBySelectedMesh()
{
	ListUI* pListUI = CImGuiMgr::GetInst()->FindEditorUI<ListUI>("List");
	Inspector* pInspector = CImGuiMgr::GetInst()->FindEditorUI<Inspector>("Inspector");
	std::wstring meshKey;
	StringToWString(pListUI->GetSelected(), meshKey);
	Ptr<CMesh> pMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(meshKey);

	CGameObject* pTargetObj = pInspector->GetTargetObject();
	assert(pTargetObj != nullptr && pMesh != nullptr);
	pTargetObj->GetRenderComp()->SetMesh(pMesh);
}

MeshRenderUI::MeshRenderUI()
	: ComponentUI("MeshRenderUI", "##MeshRenderUI", COMPONENT_TYPE::MESHRENDER)
{
	SetChildSize(ImVec2(0.f, 300.f));
}

MeshRenderUI::~MeshRenderUI()
{
}

void MeshRenderUI::Render_Tick()
{
	Render_Title();

	CMeshRender* pMeshRender = GetTargetObject()->GetMeshRenderComp();

	Ptr<CMesh> pMesh = pMeshRender->GetMesh();
	Ptr<CMaterial> pMaterial = pMeshRender->GetMaterial();

	std::string strMeshKey;
	WStringToString(pMesh->GetKey(), strMeshKey);
	ImGui::Text("Mesh      ");
	ImGui::SameLine();
	ImGui::InputText("##MeshName", (char*)strMeshKey.c_str(), strMeshKey.capacity(), ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine();
	if (ImGui::Button("##MeshButton", ImVec2(18.f, 18.f)))
	{
		ListUI* pListUI = CImGuiMgr::GetInst()->FindEditorUI<ListUI>("List");
		if (pListUI != nullptr)
		{
			//pListUI->SetModal(true);
			pListUI->Deactivate();
			pListUI->SetActive(true);
			pListUI->SetFocus();
			std::vector<std::string> vecNames;
			CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::MESH, vecNames);
			pListUI->AddItem(vecNames);
			pListUI->RegisterDBClickDelegate(this, (UI_DELEGATE_PARAM_1)&MeshRenderUI::SelectMesh);
		}
	}

	std::string strMaterialKey;
	WStringToString(pMaterial->GetKey(), strMaterialKey);
	ImGui::Text("Material  ");
	ImGui::SameLine();
	ImGui::InputText("##MaterialName", (char*)strMaterialKey.c_str(), strMaterialKey.capacity(), ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine();
	if (ImGui::Button("##MaterialButton", ImVec2(18.f, 18.f)))
	{
		ListUI* pListUI = CImGuiMgr::GetInst()->FindEditorUI<ListUI>("List");
		if (pListUI != nullptr)
		{
			pListUI->Deactivate();
			pListUI->SetActive(true);
			pListUI->SetFocus();
			std::vector<std::string> vecNames;
			CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::MATERIAL, vecNames);
			pListUI->AddItem(vecNames);
			pListUI->RegisterDBClickDelegate(this, (UI_DELEGATE_PARAM_1)& MeshRenderUI::SelectMaterial);
		}
	}
}

UINT MeshRenderUI::SelectMesh(DWORD_PTR selected)
{
	std::string* pStr = (std::string*)selected;
	std::wstring key;
	StringToWString(*pStr, key);
	Ptr<CMesh> pMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(key);

	assert(GetTargetObject() != nullptr && pMesh != nullptr);
	GetTargetObject()->GetMeshRenderComp()->SetMesh(pMesh);

	return 1;
}

UINT MeshRenderUI::SelectMaterial(DWORD_PTR selected)
{
	std::string* pStr = (std::string*)selected;
	std::wstring key;
	StringToWString(*pStr, key);
	Ptr<CMaterial> pMaterial = CAssetMgr::GetInst()->FindAsset<CMaterial>(key);

	assert(GetTargetObject() != nullptr && pMaterial != nullptr);
	GetTargetObject()->GetMeshRenderComp()->SetMaterial(pMaterial);

	return 1;
}
