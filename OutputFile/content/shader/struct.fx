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
    float3 vWorldInitScale;
    float3 vWorldCurrentScale;

    float3 vForce;
    float3 vVelocity;

    float NoiseForceAccTime;
    float3 NoiseForceDir;
    
    float Mass;
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
    
    float Mass;
    
    uint SpawnShape;
    float3 SpawnShapeScale;
    
    uint BlockSpawnShape;
    float3 BlockSpawnShapeScale;

    uint SpaceType;
    
    // Spawn Burst
    uint SpawnBurstCount;
    uint SpawnBurstRepeat;
    float SpawnBurstRepeatTime;
    
    // Add Velocity
    uint AddVelocityType;
    float3 AddVelocityFixedDir;
    float AddMinSpeed;
    float AddMaxSpeed;
    
    // Scale
    float StartScale;
    float EndScale;
    
    // Drag
    float DstNormalizedAge;
    float LimitSpeed;
    
    // Noise Force
    float NoiseForceTerm;
    float NoiseForceScale;
    
    // Render
    float3 EndColor;
    uint FadeOut;
    float StartRatio;
    uint VelocityAlignment;
    
	// Module on/off
    int Module[7];
    
    float padding;
};

#endif