#pragma once

#include <queue>

struct RenderInfo
{
	// 리소스 매니저에서 접근해서 다룰 바인딩 정보들
	// shader
	// texture
	// 
};

struct PostProcessingFlag
{
	bool bUseShadow;
	bool bUseSsao;
};

class RenderManager
{
public:

	void RegisterRednerInfo(RenderInfo renderInfo) { mRenderQueue.push(renderInfo); }
	void DrawAll()
	{
		// preprocessing

		// render to offscreen texture

		// postprocessing 

		// render to backBuffer and present


		// 의사 코드
		while (mRenderQueue.empty())
		{
			auto info = mRenderQueue.front();
			mRenderQueue.pop();

			// excute info
		}
	}

	void SetPostProcessingFlag(PostProcessingFlag postProcessingFlag) { mPostProcessingFlag = postProcessingFlag; };

private:
	// 싱글턴
	PostProcessingFlag mPostProcessingFlag;

	queue<RenderInfo> mRenderQueue;

};

