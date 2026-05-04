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