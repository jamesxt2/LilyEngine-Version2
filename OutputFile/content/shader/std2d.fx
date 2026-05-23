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
};

VS_OUT VS_Std2D(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    float4 vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld);  
    float4 vViewPos = mul(vWorldPos, g_matView);
    float4 vProjPos = mul(vViewPos, g_matProj);
    
    output.vPosition = vProjPos;
    output.vColor = _in.vColor;
    output.vUV = _in.vUV;
    
    return output;
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
    
    return vColor;
}

#endif