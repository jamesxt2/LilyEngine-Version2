#pragma once

// Vertex info in 3D Coords
struct Vtx
{
	Vec3 vPos;
	Vec4 vColor;
	Vec2 vUV;
};

struct TTransform
{
	Matrix matModel;
	Matrix matView;
	Matrix matProj;
};

extern TTransform g_Trans;

struct TMaterialConst
{
	int		iArr[4];
	float	fArr[4];
	Vec2	v2Arr[4];
	Vec4	v4Arr[4];
	Matrix	matArr[4];
};

struct TDebugShapeInfo
{
	DEBUG_SHAPE Shape;
	Vec3 Position;
	Vec3 Rotation;
	Vec3 Scale;
	Matrix matWorld;
	Vec4 Color;
	float Duration;
	float Age;
};