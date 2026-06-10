#ifndef _PARTICLE
#define _PARTICLE

#include "value.fx"

// Particle
StructuredBuffer<TParticle> g_Particle : register(t17);
StructuredBuffer<TParticleModule> ModuleBuffer : register(t18);

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

[maxvertexcount(12)]
void GS_Particle(point VS_PARTICLE_OUT _in[1], inout TriangleStream<GS_PARTICLE_OUT> _OutStream)
{
    if (g_Particle[_in[0].InstID].Active == 0)
        return;
    
    GS_PARTICLE_OUT output[4] =
    {
        (GS_PARTICLE_OUT) 0.f, (GS_PARTICLE_OUT) 0.f,
        (GS_PARTICLE_OUT) 0.f, (GS_PARTICLE_OUT) 0.f
    };
    GS_PARTICLE_OUT output_cross[4] =
    {
        (GS_PARTICLE_OUT) 0.f, (GS_PARTICLE_OUT) 0.f,
        (GS_PARTICLE_OUT) 0.f, (GS_PARTICLE_OUT) 0.f
    };
    
    float4 vViewPos = mul(float4(g_Particle[_in[0].InstID].vWorldPosition, 1.f), g_matView);
    
    // View Space
    // 0---1
    // | \ |
    // 3---2
    
    output[0].vPosition = float4(-g_Particle[_in[0].InstID].vWorldCurrentScale.x * 0.5f,
        g_Particle[_in[0].InstID].vWorldCurrentScale.y * 0.5f, 0.f, 0.f);
    output[1].vPosition = float4(g_Particle[_in[0].InstID].vWorldCurrentScale.x * 0.5f,
        g_Particle[_in[0].InstID].vWorldCurrentScale.y * 0.5f, 0.f, 0.f);
    output[2].vPosition = float4(g_Particle[_in[0].InstID].vWorldCurrentScale.x * 0.5f,
        -g_Particle[_in[0].InstID].vWorldCurrentScale.y * 0.5f, 0.f, 0.f);
    output[3].vPosition = float4(-g_Particle[_in[0].InstID].vWorldCurrentScale.x * 0.5f,
        -g_Particle[_in[0].InstID].vWorldCurrentScale.y * 0.5f, 0.f, 0.f);
    
    if (ModuleBuffer[0].VelocityAlignment)
    {
        output_cross[0].vPosition = float4(-g_Particle[_in[0].InstID].vWorldCurrentScale.x * 0.5f,
            0.f, g_Particle[_in[0].InstID].vWorldCurrentScale.y * 0.5f, 0.f);
        output_cross[1].vPosition = float4(g_Particle[_in[0].InstID].vWorldCurrentScale.x * 0.5f,
            0.f, g_Particle[_in[0].InstID].vWorldCurrentScale.y * 0.5f, 0.f);
        output_cross[2].vPosition = float4(g_Particle[_in[0].InstID].vWorldCurrentScale.x * 0.5f,
            0.f, -g_Particle[_in[0].InstID].vWorldCurrentScale.y * 0.5f, 0.f);
        output_cross[3].vPosition = float4(-g_Particle[_in[0].InstID].vWorldCurrentScale.x * 0.5f,
            0.f, -g_Particle[_in[0].InstID].vWorldCurrentScale.y * 0.5f, 0.f);
        
        float3 vR = normalize(mul(float4(g_Particle[_in[0].InstID].vVelocity, 0.f), g_matView).xyz);
        float3 vF = normalize(cross(vR, float3(0.f, 1.f, 0.f)));
        float3 vU = normalize(cross(vF, vR));
        
        float3x3 vRot = { vR, vU, vF };

        for (int i = 0; i < 4; ++i)
        {
            output[i].vPosition.xyz = mul(output[i].vPosition.xyz, vRot);
            output_cross[i].vPosition.xyz = mul(output_cross[i].vPosition.xyz, vRot);
        }
    }
    
    for (int i = 0; i < 4; ++i)
    {
        output[i].vPosition += vViewPos;
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
    
    if (ModuleBuffer[0].VelocityAlignment)
    {
        for (int i = 0; i < 4; ++i)
        {
            output_cross[i].vPosition += vViewPos;
            output_cross[i].vPosition = mul(output_cross[i].vPosition, g_matProj);
        }
    
        output_cross[0].vUV = float2(0.f, 0.f);
        output_cross[1].vUV = float2(1.f, 0.f);
        output_cross[2].vUV = float2(1.f, 1.f);
        output_cross[3].vUV = float2(0.f, 1.f);
    
        output_cross[0].InstID = output_cross[1].InstID = output_cross[2].InstID = output_cross[3].InstID = _in[0].InstID;

        _OutStream.Append(output_cross[0]);
        _OutStream.Append(output_cross[1]);
        _OutStream.Append(output_cross[2]);
        _OutStream.RestartStrip();

        _OutStream.Append(output_cross[0]);
        _OutStream.Append(output_cross[2]);
        _OutStream.Append(output_cross[3]);
        _OutStream.RestartStrip();
    }
    
    return;
}


float4 PS_Particle(GS_PARTICLE_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
    
    if (g_btex_1)
    {
        vColor = g_tex_1.Sample(g_sam_0, _in.vUV);
        vColor *= g_Particle[_in.InstID].vColor;
    }
    else
        vColor = g_Particle[_in.InstID].vColor;
    return vColor;
}

#endif