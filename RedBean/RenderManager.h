#pragma once

#include <queue>

struct RenderInfo
{
	// ���ҽ� �Ŵ������� �����ؼ� �ٷ� ���ε� ������
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


		// �ǻ� �ڵ�
		while (mRenderQueue.empty())
		{
			auto info = mRenderQueue.front();
			mRenderQueue.pop();

			// excute info
		}
	}

	void SetPostProcessingFlag(PostProcessingFlag postProcessingFlag) { mPostProcessingFlag = postProcessingFlag; };

private:
	// �̱���
	PostProcessingFlag mPostProcessingFlag;

	queue<RenderInfo> mRenderQueue;

};

