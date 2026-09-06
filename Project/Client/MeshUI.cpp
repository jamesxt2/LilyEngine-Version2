#include "pch.h"
#include "MeshUI.h"

MeshUI::MeshUI()
	: AssetUI("MeshUI", "##MeshUI", ASSET_TYPE::MESH)
{
}

MeshUI::~MeshUI()
{
}

void MeshUI::Render_Tick()
{
	Render_Title();

	std::string name = WStringToString(GetTarget()->GetKey());

	Ptr<CMesh> pMesh = dynamic_cast<CMesh*>(GetTarget().Get());
	assert(pMesh.Get());

	ImGui::Text("Mesh Name");
	ImGui::SameLine(120);
	ImGui::InputText("##MeshNameMeshUI", (char*)name.c_str(), name.capacity(), ImGuiInputTextFlags_ReadOnly);

	int vtxCount = pMesh->GetVertexCount();
	ImGui::Text("Vertex Count");
	ImGui::SameLine(120);
	ImGui::InputInt("##VtxCountMeshUI", &vtxCount, 0, 100, ImGuiInputTextFlags_ReadOnly);

	int idxCount = pMesh->GetIndexCount() / 3;
	ImGui::Text("Triangle Count");
	ImGui::SameLine(120);
	ImGui::InputInt("##IdxCountMeshUI", &idxCount, 0, 100, ImGuiInputTextFlags_ReadOnly);
}
