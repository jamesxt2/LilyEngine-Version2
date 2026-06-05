#ifndef _PARTICLE
#define _PARTICLE

#include "value.fx"

struct VS_PARTICLE_IN
{
    float3 vPosition : POSITION;
    float2 vUV : TEXCOORD;
    uint InstID : SV_InstanceID;
};

struct VS_PARTICLE_OUT
{
    float3 vPosition : POSITION;
    float2 vUV : TEXCOORD;
    uint InstID : FOG;
};

VS_PARTICLE_OUT VS_Particle(VS_PARTICLE_IN _in)
{
    VS_PARTICLE_OUT output = (VS_PARTICLE_OUT) 0.f;
    
    output.vPosition = _in.vPosition;
    output.vUV = _in.vUV;
    output.InstID = _in.InstID;
    
    return output;
}

struct GS_PARTICLE_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    uint InstID : FOG;
};

[maxvertexcount(6)]
void GS_Particle(point VS_PARTICLE_OUT _in[1], inout TriangleStream<GS_PARTICLE_OUT> _OutStream)
{
    if (g_Particle[_in[0].InstID].Active == 0)
        return;
    
    GS_PARTICLE_OUT output[4] = { (GS_PARTICLE_OUT) 0.f, (GS_PARTICLE_OUT) 0.f, 
        (GS_PARTICLE_OUT) 0.f, (GS_PARTICLE_OUT) 0.f };
    
    float4 vViewPos = mul(float4(g_Particle[_in[0].InstID].vWorldPos, 1.f), g_matView);
    
    // View Space
    // 0---1
    // | \ |
    // 3---2
    
    output[0].vPosition = vViewPos + float4(-g_Particle[_in[0].InstID].vWorldScale.x * 0.5f,
        g_Particle[_in[0].InstID].vWorldScale.y * 0.5f, 0.f, 0.f);
    output[1].vPosition = vViewPos + float4(g_Particle[_in[0].InstID].vWorldScale.x * 0.5f,
        g_Particle[_in[0].InstID].vWorldScale.y * 0.5f, 0.f, 0.f);
    output[2].vPosition = vViewPos + float4(g_Particle[_in[0].InstID].vWorldScale.x * 0.5f,
        -g_Particle[_in[0].InstID].vWorldScale.y * 0.5f, 0.f, 0.f);
    output[3].vPosition = vViewPos + float4(-g_Particle[_in[0].InstID].vWorldScale.x * 0.5f,
        -g_Particle[_in[0].InstID].vWorldScale.y * 0.5f, 0.f, 0.f);
    
    for (int i = 0; i < 4; ++i)
    {
        output[i].vPosition = mul(output[i].vPosition, g_matProj);
    }
    
        output[0].vUV = float2(0.f, 0.f);
    output[1].vUV = float2(1.f, 0.f);
    output[2].vUV = float2(1.f, 1.f);
    output[3].vUV = float2(0.f, 1.f);
    
    output[0].InstID = output[1].InstID = output[2].InstID = output[3].InstID = _in[0].InstID;

    _OutStream.Append(output[0]);
    _OutStream.Append(output[1]);
    _OutStream.Append(output[2]);
    _OutStream.RestartStrip();

    _OutStream.Append(output[0]);
    _OutStream.Append(output[2]);
    _OutStream.Append(output[3]);
    _OutStream.RestartStrip();
    
    return;
}


float4 PS_Particle(GS_PARTICLE_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
    
    if (g_btex_1)
        vColor = g_tex_1.Sample(g_sam_0, _in.vUV);
    else
        vColor = float4(1.f, 0.f, 1.f, 1.f);
    
    //vColor.rgb *= g_Particle[_in.InstID].vColor.rgb;
    
    return vColor;
}

#endif