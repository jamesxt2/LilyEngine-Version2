#ifndef _COMPUTE
#define _COMPUTE

#include "value.fx"

#define Width g_int_0
#define Height g_int_1
#define Color g_vec4_0

[numthreads(32, 32, 1)]
void CS_Test(int3 _ThreadID : SV_DispatchThreadID)
{
    if (_ThreadID.x >= Width || _ThreadID.y >= Height)
        return;
    TargetTex[_ThreadID.xy] = Color;
}

#endif