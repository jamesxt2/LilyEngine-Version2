#ifndef _POSTPROCESS
#define _POSTPROCESS

#include "value.fx"

struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

// mesh : RectMesh
VS_OUT VS_Distortion(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matMVP);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_Distortion(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
    
    if (g_btex_0)
    {
        float2 vScreenUV = _in.vPosition.xy / g_vResolution;
        
        float2 vUVOffset = (float2) 0.f;
        if (g_btex_1)
        {
            _in.vUV.x += g_Time * 0.1f;
            vUVOffset = g_tex_1.Sample(g_sam_0, _in.vUV);
            vUVOffset -= 0.5f;
            vUVOffset *= 0.02f;
        }
        vColor = g_tex_0.Sample(g_sam_0, vScreenUV + vUVOffset);
    }
    else
        vColor = float4(1.f, 0.f, 0.f, 1.f);
    
    return vColor;
}

#endif