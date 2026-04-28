#pragma once
#include "singleton.h"

class CAssetMgr : public CSingleton<CAssetMgr>
{
	SINGLE(CAssetMgr)

public:
	void Init();
	void CreateDefaultMesh();
	void CreateDefaultTexture();
	void CreateDefaultMaterial();
	void CreateDefaultGraphicsShader();
	void CreateDefaultComputeShader();
};

