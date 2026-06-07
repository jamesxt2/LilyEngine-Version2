#ifndef _STRUCT
#define _STRUCT

struct TLight
{
    float4      vDiffuse; // color of the light
    float4      vAmbient; // lowest brightness
    float4      vMaxSpecular; // highest brightness
};

struct TLightInfo
{
    TLight      Light;
	
    uint        LightType; // 0: DirectionalLight 1: PointLight 2: SpotLight
    float3      WorldDir;
    float3      WorldPos;
    float       Range; // SpotLight or PointLight
    float       Angle; // SpotLight

    float3      Padding;
};

struct TTileInfo
{
    uint ImgIdx;
    float3 vPadding;
};

struct TParticle
{
    float4 vColor;

    float3 vLocalPosition;
    float3 vWorldPosition;
    float3 vWorldRotation;
    float3 vWorldScale;

    float3 vForce;
    float3 vVelocity;

    float Life;
    float Age;
    float NormalizedAge;
    int Active;
    
    float2 padding;
};

struct TSpawnCount
{
    int SpawnCount;
    uint3 padding;
};

struct TParticleModule
{
    uint SpawnRate;
    float4 vSpawnColor;
    float3 vSpawnMinScale;
    float3 vSpawnMaxScale;

    float MinLife;
    float MaxLife;

    // Spawn Burst
    uint SpawnBurstCount;
    uint SpawnBurstRepeat;
    float SpawnBurstRepeatTime;
    
	// Module on/off
    int Module[3];
    
    float padding;
};

#endif