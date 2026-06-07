#ifndef _TILEMAP
#define _TILEMAP

#include "value.fx"
#include "struct.fx"



struct VS_IN
{
    float3 vLocalPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

#define AtlasMaxRow g_int_0
#define AtlasMaxCol g_int_1
#define AtlasResolution g_vec2_0
#define AtlasTileSize g_vec2_1
#define TileColRow g_vec2_2

VS_OUT VS_TileMap(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    _in.vLocalPos.xy += float2(0.5f, -0.5f);
    output.vPosition = mul(float4(_in.vLocalPos, 1.f), g_matMVP);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_TileMap(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
    
    float2 vUV = _in.vUV * TileColRow;
    int2 PixColRowIdx = (int2) vUV;
    vUV -= floor(vUV);
    
    float2 TileSize = AtlasTileSize / AtlasResolution;
    vUV *= TileSize;
    
    int imgIdx = g_TileInfo[PixColRowIdx.y * TileColRow.x + PixColRowIdx.x].ImgIdx;
    
    int row = imgIdx / AtlasMaxCol;
    int col = imgIdx % AtlasMaxCol;
    
    vUV += float2(TileSize.x * col, TileSize.y * row);
    
    vColor = g_tex_0.Sample(g_sam_0, vUV);
    
    return vColor;
}

#endif