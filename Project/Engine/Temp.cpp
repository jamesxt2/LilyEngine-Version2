#include "pch.h"
#include "Temp.h"

#include "CDevice.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CPathMgr.h"

#include "CMesh.h"

#include "CConstBuffer.h"

#include "CGraphicsShader.h"


Ptr<CMesh> g_RectMesh = nullptr;
Ptr<CMesh> g_CircleMesh = nullptr;

// system memory
Vtx g_arrVtx[4] = {};

TTransform g_Transform = {};

// Shader
Ptr<CGraphicsShader> g_Shader = nullptr;


int TempInit()
{
	/***************/
	// Rect Mesh
	/***************/
	// Vertex Buffer
	g_arrVtx[0].vPos = Vec3(-0.5f, 0.5f, 0.f);
	g_arrVtx[0].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	g_arrVtx[1].vPos = Vec3(0.5f, 0.5f, 0.f);
	g_arrVtx[1].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	g_arrVtx[2].vPos = Vec3(0.5f, -0.5f, 0.f);
	g_arrVtx[2].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	g_arrVtx[3].vPos = Vec3(-0.5f, -0.5f, 0.f);
	g_arrVtx[3].vColor = Vec4(0.f, 1.f, 0.f, 1.f);

	UINT arrIdx[6] = { 0, 2, 3, 0, 1, 2 };
	
	g_RectMesh = new CMesh;
	g_RectMesh->Create(g_arrVtx, 4, arrIdx, 6);



	/***************/
	// Circle Mesh
	/***************/
	std::vector<Vtx> vecVtx;
	std::vector<UINT> vecIdx;

	Vtx v;
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	vecVtx.push_back(v);

	float Radius = 0.5f;
	UINT Slice = 60;
	float AngleStep = 2 * XM_PI / Slice;

	float Angle = 0.f;
	for (int i = 0; i <= Slice; ++i, Angle += AngleStep)
	{
		v.vPos = Vec3(cosf(Angle) * Radius, sinf(Angle) * Radius, 0.f);
		v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
		vecVtx.push_back(v);
	}

	for (int i = 0; i < Slice; ++i)
	{
		vecIdx.push_back(0);
		vecIdx.push_back(i + 2);
		vecIdx.push_back(i + 1);
	}

	g_CircleMesh = new CMesh;
	g_CircleMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());

	// Create shader
	g_Shader = new CGraphicsShader;
	
	// Vertex Shader
	std::wstring strPath = CPathMgr::GetInst()->GetContentPath();
	strPath += L"shader\\std2d.fx";

	g_Shader->CreateVertexShader(strPath, "VS_Std2D");
	g_Shader->CreatePixelShader(strPath, "PS_Std2D");
	
	return S_OK;
}

void TempRelease()
{

}

void TempTick()
{
	float DT = CTimeMgr::GetInst()->GetDeltaTime();

	if (KEY_PRESSED(KEY::W))
	{
		g_Transform.Position.y += DT;
	}

	if (KEY_PRESSED(KEY::S))
	{
		g_Transform.Position.y -= DT;
	}

	if (KEY_PRESSED(KEY::A))
	{
		g_Transform.Position.x -= DT;
	}

	if (KEY_PRESSED(KEY::D))
	{
		g_Transform.Position.x += DT;
	}

	// System memory -> GPU
	CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);
	pCB->SetData(&g_Transform);
	pCB->Bind();
}

void TempRender()
{
	g_Shader->Bind();

	//g_RectMesh->Render();
	g_CircleMesh->Render();
}