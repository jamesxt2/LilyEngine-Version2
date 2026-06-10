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

	Matrix matMV;
	Matrix matMVP;
};

extern TTransform g_Trans;

struct TMaterialConst
{
	int		iArr[4] = {};
	float	fArr[4] = {};
	Vec2	v2Arr[4];
	Vec4	v4Arr[4];
	Matrix	matArr[4];
	UINT	bTex[16] = {};
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

/***********************************************************************/

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

/***************************PARTICLE************************************/

struct TParticle
{
	Vec4 vColor{ 1.f, 1.f, 1.f, 1.f };

	Vec3 vLocalPosition{ 0.f, 0.f, 0.f };
	Vec3 vWorldPosition{ 0.f, 0.f, 0.f };
	Vec3 vWorldRotation{ 0.f, 0.f, 0.f };
	Vec3 vWorldInitScale{ 1.f, 1.f, 1.f };
	Vec3 vWorldCurrentScale{ 1.f, 1.f, 1.f };

	Vec3 vForce;
	Vec3 vVelocity;

	float NoiseForceAccTime{ 0.f };
	Vec3 NoiseForceDir;

	float Mass{ 1.f };
	float Life{ 0.f };
	float Age{ 0.f };
	float NormalizedAge{ 0.f };
	int Active{ 0 };

	Vec2 padding;
};

struct TParticleModule
{
	UINT SpawnRate{ 0 }; // per second
	Vec4 vSpawnColor;
	Vec3 vSpawnMinScale;
	Vec3 vSpawnMaxScale;

	float Mass{ 1.f };

	float MinLife{ 0.f };
	float MaxLife{ 0.f };

	UINT SpawnShape{ 1 }; // 0: Box, 1: Sphere
	Vec3 SpawnShapeScale; // x == Radius

	UINT BlockSpawnShape{ 1 }; // 0: Box, 1: Sphere
	Vec3 BlockSpawnShapeScale; // x == Radius

	UINT SpaceType{ 0 }; // 0: LocalSpace, 1: WorldSpace

	// Spawn Burst
	UINT SpawnBurstCount{ 0 };
	UINT SpawnBurstRepeat{ 0 };
	float SpawnBurstRepeatTime{ 0.f };

	// Add Velocity
	UINT AddVelocityType{ 0 }; // 0: Random, 1: FromCenter, 2: ToCenter, 3: Fixed
	Vec3 AddVelocityFixedDir;
	float AddMinSpeed{ 0.f };
	float AddMaxSpeed{ 0.f };

	// Scale
	float StartScale{ 1.f }; //Modify spawn scale
	float EndScale{ 1.f }; // Modify spawn scale

	// Drag
	float DstNormalizedAge{ 0.f };
	float LimitSpeed{ 0.f };

	// Noise Force
	float NoiseForceTerm{ 0.f };
	float NoiseForceScale{ 0.f };

	// Render
	Vec3 EndColor;
	UINT FadeOut{ 0 }; // 0: Off, 1: NormalizedAge
	float StartRatio{ 0.f }; // Normalized age when start to fade out
	UINT VelocityAlignment{ 0 };

	// Module on/off
	int Module[(UINT)PARTICLE_MODULE::END] = {};

	float padding;
};

/***********************************************************************/