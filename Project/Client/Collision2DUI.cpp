#include "pch.h"
#include "Collision2DUI.h"

#include <Engine/CCollision2D.h>

Collision2DUI::Collision2DUI()
	: ComponentUI("Collision2D", "##Collision2DUI", COMPONENT_TYPE::COLLISION2D)
{
	SetChildSize(ImVec2(0.f, 300.f));
}

Collision2DUI::~Collision2DUI()
{
}

void Collision2DUI::Render_Tick()
{
	Render_Title();

	CCollision2D* pColl = GetTargetObject()->GetCollision2DComp();
	assert(pColl != nullptr);

	Vec3 vOffset = pColl->GetOffset();
	Vec3 vScale = pColl->GetScale();
	bool bAbsolute = pColl->IsAbsolute();

	ImGui::Text("Offset Position  ");
	ImGui::SameLine();
	if (ImGui::DragFloat3("##Collision2DOffset", vOffset))
		pColl->SetOffset(vOffset);

	ImGui::Text("Scale           ");
	ImGui::SameLine();
	if (ImGui::DragFloat3("##Collision2DScale", vScale, bAbsolute ? 1.f : 0.01f))
		pColl->SetScale(vScale);

	ImGui::Text("Absolute         ");
	ImGui::SameLine();
	if (ImGui::Checkbox("##Collision2DAbsolute", &bAbsolute))
		pColl->SetAbsolute(bAbsolute);
}
