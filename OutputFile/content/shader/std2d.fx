#ifndef _STD2D
#define _STD2D

#include "value.fx"

struct VS_IN
{
    float3 vPos : POSITION;
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
    
    float3 vWorldPos : POSITION;
};

VS_OUT VS_Std2D(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vWorldPos = mul(float4(_in.vPos, 1.f), g_matModel).xyz;
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matMVP);
    output.vColor = _in.vColor;
    output.vUV = _in.vUV;
    
    return output;
}

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

float4 PS_Std2D(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;

    if (UseAnim2D)
    {
        float2 vBackgroundLT = vLeftTop - (vBackground - vSliceSize) * 0.5f;
        float2 vUV = vBackgroundLT + _in.vUV * vBackground - vOffset;
        
        if (vUV.x < vLeftTop.x || vUV.x > vLeftTop.x + vSliceSize.x 
            || vUV.y < vLeftTop.y || vUV.y > vLeftTop.y + vSliceSize.y)
            discard;
        else
            vColor = g_Atlas.Sample(g_sam_0, vUV);
    }
    else
    {
        vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    }

    // Light
    float3 vLightPow = (float3) 0.f;
    for (int i = 0; i < Light2DCount; ++i)
    {
        vLightPow += CalculateLight2D(i, _in.vWorldPos);
    }
    vColor.rgb *= vLightPow;
    
    if (vColor.a == 0.f)
        discard;
    
    return vColor;
}

float4 PS_Std2D_AB(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;

    if (UseAnim2D)
    {
        float2 vBackgroundLT = vLeftTop - (vBackground - vSliceSize) * 0.5f;
        float2 vUV = vBackgroundLT + _in.vUV * vBackground - vOffset;
        
        if (vUV.x < vLeftTop.x || vUV.x > vLeftTop.x + vSliceSize.x 
            || vUV.y < vLeftTop.y || vUV.y > vLeftTop.y + vSliceSize.y)
            discard;
        else
            vColor = g_Atlas.Sample(g_sam_0, vUV);
    }
    else
    {
        vColor = g_tex_0.Sample(g_sam_0, _in.vUV);    
    }

    // Light
    float3 vLightPow = (float3) 0.f;
    for (int i = 0; i < Light2DCount; ++i)
    {
        vLightPow += CalculateLight2D(i, _in.vWorldPos);
    }
    vColor.rgb *= vLightPow;
    /*
    if (g_btex_1)
    {
        float4 vNoise = g_tex_1.Sample(g_sam_0, _in.vUV);
        float intense = g_float_0 + vNoise.r;
        
        vColor.r += pow(intense * 10.f, 5.f) / pow(10, 5);
        
        if (intense > 1.f)
            discard;
    }
    */
    if (g_int_0)
        vColor.r *= 1.5f;
    
    return vColor;
}



#endif