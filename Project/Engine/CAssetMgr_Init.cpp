#include "pch.h"
#include "CAssetMgr.h"

#include "CPathMgr.h"

void CAssetMgr::Init()
{
	CreateDefaultMesh();
	CreateDefaultTexture();
	CreateDefaultGraphicsShader();
	CreateDefaultComputeShader();
	CreateDefaultMaterial();
}

void CAssetMgr::CreateDefaultMesh()
{
	Ptr<CMesh> pMesh = nullptr;

	std::vector<Vtx> vecVtx;
	std::vector<UINT> vecIdx;
	Vtx v;

	/***************/
	// Rect Mesh
	/***************/
	// Vertex Buffer
	v.vPos = Vec3(-0.5f, 0.5f, 0.f);
	v.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(0.f, 0.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(0.5f, 0.5f, 0.f);
	v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	v.vUV = Vec2(1.f, 0.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(0.5f, -0.5f, 0.f);
	v.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	v.vUV = Vec2(1.f, 1.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(-0.5f, -0.5f, 0.f);
	v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	v.vUV = Vec2(0.f, 1.f);
	vecVtx.push_back(v);

	vecIdx.push_back(0);
	vecIdx.push_back(3);
	vecIdx.push_back(2);

	vecIdx.push_back(0);
	vecIdx.push_back(1);
	vecIdx.push_back(2);

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	AddAsset(L"RectMesh", pMesh);

	vecIdx.clear();

	vecIdx.push_back(0);
	vecIdx.push_back(1);
	vecIdx.push_back(2);
	vecIdx.push_back(3);
	vecIdx.push_back(0);

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	AddAsset(L"RectMesh_Debug", pMesh);

	vecVtx.clear();
	vecIdx.clear();

	/***************/
	// Circle Mesh
	/***************/
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	vecVtx.push_back(v);

	float Radius = 0.5f;
	UINT Slice = 60;
	float AngleStep = 2 * XM_PI / Slice;

	float Angle = 0.f;
	for (UINT i = 0; i <= Slice; ++i, Angle += AngleStep)
	{
		v.vPos = Vec3(cosf(Angle) * Radius, sinf(Angle) * Radius, 0.f);
		v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
		vecVtx.push_back(v);
	}

	for (UINT i = 0; i < Slice; ++i)
	{
		vecIdx.push_back(0);
		vecIdx.push_back(i + 2);
		vecIdx.push_back(i + 1);
	}

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	AddAsset(L"CircleMesh", pMesh);

	vecIdx.clear();

	for (UINT i = 0; i < Slice; ++i)
	{
		vecIdx.push_back(i + 1);
	}
	vecIdx.push_back(1);

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	AddAsset(L"CircleMesh_Debug", pMesh);

	vecVtx.clear();
	vecIdx.clear();

}

void CAssetMgr::CreateDefaultTexture()
{
	Load<CTexture>(L"texture/mountain.png", L"texture/mountain.png");
	Load<CTexture>(L"texture/pistol.png", L"texture/pistol.png");
}

void CAssetMgr::CreateDefaultGraphicsShader()
{
	std::wstring strPath = CPathMgr::GetInst()->GetContentPath();

	Ptr<CGraphicsShader> pShader = nullptr;

	// Std2DShader
	pShader = new CGraphicsShader;;
	pShader->CreateVertexShader(strPath + L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(strPath + L"shader\\std2d.fx", "PS_Std2D");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

	AddAsset<CGraphicsShader>(L"Std2DShader", pShader);

	// DebugShapeShader
	pShader = new CGraphicsShader;;
	pShader->CreateVertexShader(strPath + L"shader\\debug_shape.fx", "VS_DebugShape");
	pShader->CreatePixelShader(strPath + L"shader\\debug_shape.fx", "PS_DebugShape");
	pShader->SetRSType(RS_TYPE::WIRE_FRAME);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	AddAsset<CGraphicsShader>(L"DebugShapeShader", pShader);
}

void CAssetMgr::CreateDefaultComputeShader()
{
}

void CAssetMgr::CreateDefaultMaterial()
{
	Ptr<CMaterial> pMaterial = nullptr;

	// Std2DMaterial
	pMaterial = new CMaterial;
	pMaterial->SetName(L"Std2DMaterial");
	pMaterial->SetShader(FindAsset<CGraphicsShader>(L"Std2DShader"));

	AddAsset<CMaterial>(pMaterial->GetName(), pMaterial);

	// DebugShapeMaterial
	pMaterial = new CMaterial;
	pMaterial->SetName(L"DebugShapeMaterial");
	pMaterial->SetShader(FindAsset<CGraphicsShader>(L"DebugShapeShader"));

	AddAsset<CMaterial>(pMaterial->GetName(), pMaterial);
}