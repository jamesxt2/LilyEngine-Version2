#include "pch.h"
#include "CMaterial.h"

#include "CDevice.h"
#include "CConstBuffer.h"
#include "CGraphicsShader.h"
#include "CTexture.h"

CMaterial::CMaterial()
	: CAsset(ASSET_TYPE::MATERIAL), m_Dynamic(false)
{
}

CMaterial::~CMaterial()
{
}

void CMaterial::Bind()
{
	for (UINT i = 0; i < TEX_PARAM::END; ++i)
	{
		if (m_arrTex[i] == nullptr)
		{
			CTexture::Clear(i);
			continue;
		}
		m_arrTex[i]->Bind(i);
	}

	if (m_Shader.Get() != nullptr)
		m_Shader->Bind();

	CConstBuffer* pMaterialCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::MATERIAL);
	pMaterialCB->SetData(&m_Const);
	pMaterialCB->Bind();
}

Ptr<CMaterial> CMaterial::GetDynamicMaterial()
{
	Ptr<CMaterial> pMtrl = new CMaterial(*this);
	pMtrl->m_Dynamic = true;
	return pMtrl;
}
