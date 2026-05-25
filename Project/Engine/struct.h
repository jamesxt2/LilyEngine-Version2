#pragma once

// Vertex info in 3D Coords
struct Vtx
{
	Vec3 vPos;
	Vec4 vColor;
	Vec2 vUV;
};

struct TTransform
{
	Matrix matModel;
	Matrix matView;
	Matrix matProj;
};

extern TTransform g_Trans;

struct TMaterialConst
{
	int		iArr[4];
	float	fArr[4];
	Vec2	v2Arr[4];
	Vec4	v4Arr[4];
	Matrix	matArr[4];
};

struct TDebugShapeInfo
{
	DEBUG_SHAPE Shape;
	Vec3 Position;
	Vec3 Rotation;
	Vec3 Scale;
	Matrix matWorld;
	Vec4 Color;
	float Duration;
	float Age;
};

struct TAnim2DInfo
{
	Vec2 vLeftTop;
	Vec2 vSliceSize;

	Vec2 vOffset;
	Vec2 vBackground;

	int UseAnim2D;
	int padding[3];
};

struct TTask
{
	TASK_TYPE type;
	DWORD_PTR dwParam_0;
	DWORD_PTR dwParam_1;
	DWORD_PTR dwParam_2;
};

/***************************LIGHT************************************/
struct TLight
{
	Vec4 vDiffuse;	// color of the light
	Vec4 vAmbient;  // lowest brightness
	Vec4 vMaxSpecular; // highest brightness
};

struct TLightInfo
{
	TLight		Light;
	
	UINT		LightType; // 0: DirectionalLight 1: PointLight 2: SpotLight
	Vec3		WorldDir;
	Vec3		WorldPos;
	float		Range; // SpotLight or PointLight
	float		Angle; // SpotLight

	Vec3		Padding;
};

/***************************LIGHT************************************/

struct TGlobalData
{
	Vec2 vResolution;
	float DeltaTime;
	float Time;

	int Light2DCount;
	int Light3DCount;

	Vec2 Padding;
};

extern TGlobalData g_GlobalData;