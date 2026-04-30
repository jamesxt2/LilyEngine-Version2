#include "pch.h"
#include "CAssetMgr.h"

#include "CPathMgr.h"

void CAssetMgr::Init()
{
	CreateDefaultMesh();
	CreateDefaultTexture();
	CreateDefaultMaterial();
	CreateDefaultGraphicsShader();
	CreateDefaultComputeShader();
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
	vecVtx.push_back(v);

	v.vPos = Vec3(0.5f, 0.5f, 0.f);
	v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(0.5f, -0.5f, 0.f);
	v.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(-0.5f, -0.5f, 0.f);
	v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	vecVtx.push_back(v);

	vecIdx.push_back(0);
	vecIdx.push_back(2);
	vecIdx.push_back(3);
	vecIdx.push_back(0);
	vecIdx.push_back(1);
	vecIdx.push_back(2);

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	AddAsset(L"RectMesh", pMesh);

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

	vecVtx.clear();
	vecIdx.clear();

}

void CAssetMgr::CreateDefaultTexture()
{
}

void CAssetMgr::CreateDefaultMaterial()
{
}

void CAssetMgr::CreateDefaultGraphicsShader()
{
	Ptr<CGraphicsShader> pShader = nullptr;

	std::wstring strPath = CPathMgr::GetInst()->GetContentPath();
	strPath += L"shader\\std2d.fx";

	pShader = new CGraphicsShader;;
	pShader->CreateVertexShader(strPath, "VS_Std2D");
	pShader->CreatePixelShader(strPath, "PS_Std2D");

	AddAsset<CGraphicsShader>(L"Std2DShader", pShader);
}

void CAssetMgr::CreateDefaultComputeShader()
{
}
