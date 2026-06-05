#ifndef _PARTICLE_TICK
#define _PARTICLE_TICK

#include "value.fx"

struct TSpawnCount
{
    int SpawnCount;
    uint3 padding;
};

RWStructuredBuffer<TParticle> ParticleBuffer : register(u0);
RWStructuredBuffer<TSpawnCount> SpawnCountBuffer : register(u1);

#define ParticleMaxCount g_int_0

static int g_SpawnCount = 1;

[numthreads(32, 1, 1)]
void CS_ParticleTick(int3 _ID : SV_DispatchThreadID)
{
    if (_ID.x >= ParticleMaxCount)
        return;
    
    if (ParticleBuffer[_ID.x].Active == 0)
    {
        int CurSpawnCount = SpawnCountBuffer[0].SpawnCount;
        while (CurSpawnCount > 0)
        {
            int originValue = 0;
            
            InterlockedCompareExchange(SpawnCountBuffer[0].SpawnCount,
                                CurSpawnCount, SpawnCountBuffer[0].SpawnCount - 1, originValue);
            
            if (CurSpawnCount == originValue)
            {
                ParticleBuffer[_ID.x].Active = 1;
                break;
            }
            
            CurSpawnCount = SpawnCountBuffer[0].SpawnCount;
        }
    }
    else
        ParticleBuffer[_ID.x].vWorldPos.y += 100.f * DeltaTime;

}

#endif