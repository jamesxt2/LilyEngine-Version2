#ifndef _FUNC
#define _FUNC

#include "value.fx"

float3 CalculateLight2D(int lightIdx, float3 vWorldPos)
{
    TLightInfo info = g_Light2D[lightIdx];
    
    float3 vLightPow = (float3) 0.f;
    
    if (info.LightType == 0) // Directional Light
    {
        vLightPow = info.Light.vDiffuse.rgb + info.Light.vAmbient.rgb;
    }
    else if (info.LightType == 1) // Point Light
    {
        float fDist = distance(info.WorldPos.xy, vWorldPos.xy);
        float fRatio = cos((fDist / info.Range) * (PI / 2.f));
        
        if (fDist < info.Range)
            vLightPow = info.Light.vDiffuse.rgb * fRatio;
    }
    else // Spot Light
    {
        
    }
    
    return vLightPow;
}

#endif