#include "pch.h"
#include "CDebugRenderMgr.h"

void DrawDebugRect(Vec3 worldPos, Vec3 worldRot, Vec3 worldScale, Vec4 color, float duration)
{
	TDebugShapeInfo info = {};
	info.Shape = DEBUG_SHAPE::RECT;

	info.Position = worldPos;
	info.Rotation = worldRot;
	info.Scale = worldScale;
	info.matWorld = XMMatrixIdentity();
	info.Color = color;
	info.Duration = duration;
	info.Age = 0.f;

	CDebugRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void DrawDebugRect(Matrix matWorld, Vec4 color, float duration)
{
	TDebugShapeInfo info = {};
	info.Shape = DEBUG_SHAPE::RECT;

	info.matWorld = matWorld;
	info.Color = color;
	info.Duration = duration;
	info.Age = 0.f;

	CDebugRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void DrawDebugCircle(Vec3 worldPos, float radius, Vec4 color, float duration)
{
	TDebugShapeInfo info = {};
	info.Shape = DEBUG_SHAPE::CIRCLE;

	info.Position = worldPos;
	info.Scale = Vec3(radius * 2.f, radius * 2.f, 1.f);
	info.matWorld = XMMatrixIdentity();
	info.Color = color;
	info.Duration = duration;
	info.Age = 0.f;

	CDebugRenderMgr::GetInst()->AddDebugShapeInfo(info);
}
