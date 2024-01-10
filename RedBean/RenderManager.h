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

	// 
	// 기능을 뺴자 => 기능이 안 나왔는데 최적화하고 조건부 컴파일하고, 의미가 있나?
	//

	void Render()
	{
		
		// 니가 디퍼드용 텍스처 어떻게 만들지
		// 디퍼드 텍스처를 다 그린다.

		// 랜더 패스

		// 후처리 
	}



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

