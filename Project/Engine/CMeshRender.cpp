#include "pch.h"
#include "CMeshRender.h"

#include "CTransform.h"
#include "CAnimator2D.h"

CMeshRender::CMeshRender()
	: CRenderComponent(COMPONENT_TYPE::MESHRENDER)
{
}

CMeshRender::~CMeshRender()
{
}

void CMeshRender::FinalTick()
{
}

void CMeshRender::Render()
{
	if (GetMesh() == nullptr || GetMaterial() == nullptr) return;

	GetOwner()->GetTransformComp()->Bind();

	if (GetOwner()->GetAnimator2DComp())
	{
		GetOwner()->GetAnimator2DComp()->Bind();
	}
	else
	{
		CAnimator2D::Unbind();
	}

	GetMaterial()->Bind();
	
	GetMesh()->Render();
}
