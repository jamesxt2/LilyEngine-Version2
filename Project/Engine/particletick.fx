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
                    float3 vRandom = GetRandom(NoiseTex, 2.f * ((float) _ID.x / (float) (ParticleMaxCount - 1)));
                    float3 vRandom1 = GetRandom(NoiseTex, ((float) (_ID.x + 1) / (float) (ParticleMaxCount - 1)));
                    float3 vRandom2 = GetRandom(NoiseTex, ((float) (_ID.x + 2) / (float) (ParticleMaxCount - 1)));
                
                    float BoxScale = 300.f;
                    float3 vRandomPos = (float3) 0.f;
                
                    vRandomPos.x = vRandom.x * BoxScale - BoxScale / 2.f;
                    vRandomPos.y = vRandom.y * BoxScale - BoxScale / 2.f;
                    vRandomPos.z = vRandom.z * BoxScale - BoxScale / 2.f;
                
                    ParticleBuffer[_ID.x].vLocalPosition = vRandomPos;
                    ParticleBuffer[_ID.x].vWorldPosition = vRandomPos + ParticleObjectPos;
                    ParticleBuffer[_ID.x].vWorldScale = (Module[0].vSpawnMaxScale - Module[0].vSpawnMinScale) * vRandom.x + Module[0].vSpawnMinScale;
                
                    ParticleBuffer[_ID.x].vColor = Module[0].vSpawnColor;
                
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
        //ParticleBuffer[_ID.x].vWorldPos.y += 100.f * g_DeltaTime;
        ParticleBuffer[_ID.x].Age += g_DeltaTime;
        ParticleBuffer[_ID.x].NormalizedAge = ParticleBuffer[_ID.x].Age / ParticleBuffer[_ID.x].Life;
        if (ParticleBuffer[_ID.x].Age >= ParticleBuffer[_ID.x].Life)
            ParticleBuffer[_ID.x].Active = 0;

    }
}


#endif