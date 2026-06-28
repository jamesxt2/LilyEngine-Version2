#pragma once
#include "singleton.h"

#include "CTexture.h"

class CCamera;
class CLight2D;
class CStructuredBuffer;

enum class RENDER_MODE
{
	PLAY,
	EDITOR
};

class CRenderMgr : public CSingleton<CRenderMgr>
{
	SINGLE(CRenderMgr)

public:
	void Init();
	void Tick();
	void Render();

	void RegisterCamera(CCamera* camera, int priority);
	inline void RegisterEditorCamera(CCamera* editorCam) { m_EditorCam = editorCam; }
	int RegisterLight2D(CLight2D* light)
	{
		m_vecLight2D.push_back(light);
		return (int)m_vecLight2D.size() - 1;
	}

	void CopyRenderTarget();

	inline void ChangeRenderMode(RENDER_MODE mode)
	{
		Render_Func = mode == RENDER_MODE::PLAY ? &CRenderMgr::Render_Play : &CRenderMgr::Render_Editor;
	}

private:
	void DataBind();
	void DataClear();

	void Render_Play();
	void Render_Editor();

private:
	std::vector<CCamera*> m_vecCam;
	CCamera* m_EditorCam;

	void(CRenderMgr::* Render_Func)(void);

	std::vector<CLight2D*> m_vecLight2D;
	CStructuredBuffer* m_Light2DBuffer;

	Ptr<CTexture> m_RenderTargetCopyTex;
};

