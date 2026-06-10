#ifndef _PARTICLE_TICK
#define PARTICLE_TICK

#include "value.fx"

RWStructuredBuffer<TParticle> ParticleBuffer : register(u0);
RWStructuredBuffer<TSpawnCount> SpawnCountBuffer : register(u1);
Texture2D NoiseTex : register(t18);
StructuredBuffer<TParticleModule> Module : register(t19);

#define ParticleMaxCount g_int_0
#define ParticleObjectPos g_vec4_0.xyz

// Module Check
#define SpawnModule Module[0].Module[0]
#define SpawnBurstModule Module[0].Module[1]
#define AddVelocityModule Module[0].Module[2]
#define ScaleModule Module[0].Module[3]
#define DragModule Module[0].Module[4]
#define NoiseForceModule Module[0].Module[5]
#define RenderModule Module[0].Module[6]

float3 GetRandom(in Texture2D noise, float normalizedThreadID)
{
    float2 vUV = (float2) 0.f;
    vUV.x = normalizedThreadID + g_Time * 0.1f;
    vUV.y = sin((vUV.x - g_Time) * 20 * PI) * 0.5f + g_Time * 0.2f;

    float3 vNoise = NoiseTex.SampleLevel(g_sam_0, vUV, 0).xyz;
    return vNoise;
}

[numthreads(32, 1, 1)]
void CS_ParticleTick(int3 _ID : SV_DispatchThreadID)
{
    if (_ID.x >= ParticleMaxCount)
        return;
    
    if (ParticleBuffer[_ID.x].Active == 0)
    {
        if (SpawnModule || SpawnBurstModule)
        {
            int CurSpawnCount = SpawnCountBuffer[0].SpawnCount;
            while (CurSpawnCount > 0)
            {
                int originValue = 0;
            
                InterlockedCompareExchange(SpawnCountBuffer[0].SpawnCount,
                                CurSpawnCount, SpawnCountBuffer[0].SpawnCount - 1, originValue);
            
                if (CurSpawnCount == originValue)
                {
                    float3 vRandom = GetRandom(NoiseTex, ((float) _ID.x / (float) (ParticleMaxCount - 1)));
                    float3 vRandom1 = GetRandom(NoiseTex, ((float) (_ID.x + 1) / (float) (ParticleMaxCount - 1)));
                    float3 vRandom2 = GetRandom(NoiseTex, ((float) (_ID.x + 2) / (float) (ParticleMaxCount - 1)));
                
                    float3 vSpawnPos = (float3) 0.f;

                    // 0: Box, 1: Sphere
                    if (Module[0].SpawnShape == 0)
                    {
                        vSpawnPos.x = vRandom.x * Module[0].SpawnShapeScale.x - Module[0].SpawnShapeScale.x / 2.f;
                        vSpawnPos.y = vRandom.y * Module[0].SpawnShapeScale.y - Module[0].SpawnShapeScale.y / 2.f;
                        vSpawnPos.z = vRandom.z * Module[0].SpawnShapeScale.z - Module[0].SpawnShapeScale.z / 2.f;
                    }
                    else if (Module[0].SpawnShape == 1)
                    {
                        float radius = Module[0].SpawnShapeScale.x;
                        
                        if (Module[0].BlockSpawnShape == 1)
                        {
                            float blockRadius = Module[0].BlockSpawnShapeScale.x;
                            float differRadius = radius - blockRadius;
                            vSpawnPos = normalize(vRandom1 - 0.5f) * differRadius * vRandom2.x
                                            + normalize(vRandom1 - 0.5f) * blockRadius;
                        }
                        else
                            vSpawnPos = normalize(vRandom1 - 0.5f) * radius * vRandom2.x;
                    }
                    
                    // Add Velocity Module
                    ParticleBuffer[_ID.x].vVelocity = (float3) 0.f;
                    if (AddVelocityModule)
                    {
                        float speed = Module[0].AddMinSpeed + (Module[0].AddMaxSpeed - Module[0].AddMinSpeed) * vRandom1.x;
                        // Random
                        if (Module[0].AddVelocityType == 0)
                            ParticleBuffer[_ID.x].vVelocity = normalize(vRandom2 - 0.5f) * speed;
                        // From Center
                        else if (Module[0].AddVelocityType == 1)
                            ParticleBuffer[_ID.x].vVelocity = normalize(vSpawnPos) * speed;
                        // To Center
                        else if (Module[0].AddVelocityType == 2)
                            ParticleBuffer[_ID.x].vVelocity = -normalize(vSpawnPos) * speed;
                        // Fixed
                        else if (Module[0].AddVelocityType == 3)
                            ParticleBuffer[_ID.x].vVelocity = normalize(Module[0].AddVelocityFixedDir) * speed;
                    }
                    
                    ParticleBuffer[_ID.x].vLocalPosition = vSpawnPos;
                    ParticleBuffer[_ID.x].vWorldPosition = vSpawnPos + ParticleObjectPos;
                    ParticleBuffer[_ID.x].vWorldInitScale = (Module[0].vSpawnMaxScale - Module[0].vSpawnMinScale) * vRandom.x + Module[0].vSpawnMinScale;
                
                    ParticleBuffer[_ID.x].vColor = Module[0].vSpawnColor;
                    ParticleBuffer[_ID.x].Mass = Module[0].Mass;
                    ParticleBuffer[_ID.x].NoiseForceAccTime = 0.f;
                
                    ParticleBuffer[_ID.x].Active = 1;
                    ParticleBuffer[_ID.x].Age = 0.f;
                    ParticleBuffer[_ID.x].Life = (Module[0].MaxLife - Module[0].MinLife) * vRandom1.y + Module[0].MinLife;
                    ParticleBuffer[_ID.x].NormalizedAge = 0.f;
                
                    break;
                }
            
                CurSpawnCount = SpawnCountBuffer[0].SpawnCount;
            }
        }
    }
    else
    {
        ParticleBuffer[_ID.x].vForce = float3(0.f, 0.f, 0.f);
        
        if (NoiseForceModule)
        {
            if (ParticleBuffer[_ID.x].NoiseForceAccTime >= Module[0].NoiseForceTerm)
            {
                ParticleBuffer[_ID.x].NoiseForceAccTime = 0.f;
                float3 vRandom = GetRandom(NoiseTex, ((float) _ID.x / (float) (ParticleMaxCount - 1)));
                ParticleBuffer[_ID.x].NoiseForceDir = normalize(vRandom - 0.5f);
            }
            
            ParticleBuffer[_ID.x].vForce += ParticleBuffer[_ID.x].NoiseForceDir * Module[0].NoiseForceScale;
            ParticleBuffer[_ID.x].NoiseForceAccTime += g_DeltaTime;
        }
        
        float3 vAccelerate = ParticleBuffer[_ID.x].vForce / ParticleBuffer[_ID.x].Mass;
        ParticleBuffer[_ID.x].vVelocity += vAccelerate * g_DeltaTime;
        
        if (Module[0].SpaceType == 0)
        {
            ParticleBuffer[_ID.x].vLocalPosition += ParticleBuffer[_ID.x].vVelocity * g_DeltaTime;
            ParticleBuffer[_ID.x].vWorldPosition = ParticleBuffer[_ID.x].vLocalPosition + ParticleObjectPos;
        }
        else
            ParticleBuffer[_ID.x].vWorldPosition += ParticleBuffer[_ID.x].vVelocity * g_DeltaTime;
        
        ParticleBuffer[_ID.x].Age += g_DeltaTime;
        ParticleBuffer[_ID.x].NormalizedAge = ParticleBuffer[_ID.x].Age / ParticleBuffer[_ID.x].Life;
        
        if (ScaleModule)
            ParticleBuffer[_ID.x].vWorldCurrentScale = 
                ((Module[0].EndScale - Module[0].StartScale) * ParticleBuffer[_ID.x].NormalizedAge
                + Module[0].StartScale) * ParticleBuffer[_ID.x].vWorldInitScale;
        else
            ParticleBuffer[_ID.x].vWorldCurrentScale = ParticleBuffer[_ID.x].vWorldInitScale;
        
        if (DragModule)
        {
            if (Module[0].DstNormalizedAge > ParticleBuffer[_ID.x].NormalizedAge)
            {
                float grad = (Module[0].LimitSpeed - length(ParticleBuffer[_ID.x].vVelocity))
                    / (Module[0].DstNormalizedAge - ParticleBuffer[_ID.x].NormalizedAge);
                float newSpeed = length(ParticleBuffer[_ID.x].vVelocity) 
                    + grad * g_DeltaTime / ParticleBuffer[_ID.x].Life;
                ParticleBuffer[_ID.x].vVelocity = normalize(ParticleBuffer[_ID.x].vVelocity) * newSpeed;
            }
        }
        
        if (RenderModule)
        {
            ParticleBuffer[_ID.x].vColor.rgb = (Module[0].EndColor.rgb - Module[0].vSpawnColor.rgb)
                    * ParticleBuffer[_ID.x].NormalizedAge + Module[0].vSpawnColor.rgb;
            if (Module[0].FadeOut)
            {
                ParticleBuffer[_ID.x].vColor.a = 
                        saturate(1.f - (ParticleBuffer[_ID.x].NormalizedAge - Module[0].StartRatio) 
                        / (1.f - Module[0].StartRatio));
            }
        }
        
        if (ParticleBuffer[_ID.x].Age >= ParticleBuffer[_ID.x].Life)
            ParticleBuffer[_ID.x].Active = 0;
    }
}


#endif