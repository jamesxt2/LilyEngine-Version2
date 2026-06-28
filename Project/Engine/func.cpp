#include "pch.h"
#include "CDebugRenderMgr.h"
#include "CTaskMgr.h"
#include "CGameObject.h"

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

void SaveWString(const std::wstring& str, FILE* file)
{
	size_t len = str.length();
	fwrite(&len, sizeof(size_t), 1, file);
	fwrite(str.c_str(), sizeof(wchar_t), len, file);
}

void LoadWString(std::wstring& str, FILE* file)
{
	size_t len = 0;
	fread(&len, sizeof(size_t), 1, file);
	str.resize(len);
	fread((wchar_t*)str.c_str(), sizeof(wchar_t), len, file);
}

void SpawnObject(int layerIdx, CGameObject* object)
{
	TTask task = {};
	task.type = TASK_TYPE::SPAWN_OBJECT;
	task.dwParam_0 = 0;
	task.dwParam_1 = (DWORD_PTR)object;

	CTaskMgr::GetInst()->AddTask(task);
}

bool IsValid(CGameObject*& object)
{
	if (object == nullptr)
		return false;
	else if (object->IsDead())
	{
		object = nullptr;
		return false;
	}
	return true;
}

void ChangeLevel(CLevel* nextLevel, LEVEL_STATE nextLevelState)
{
	TTask task = {};
	task.type = TASK_TYPE::CHANGE_LEVEL;
	task.dwParam_0 = (DWORD_PTR)nextLevel;
	task.dwParam_1 = (DWORD_PTR)nextLevelState;

	CTaskMgr::GetInst()->AddTask(task);
}
