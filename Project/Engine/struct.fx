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
    float3 vLocalPos;
    float3 vWorldPos;
    float3 vWorldScale;
    float4 vColor;
    float3 vVelocity;

    float Life;
    float Age;
    float NormalizedAge;
    int Active;
};

#endif