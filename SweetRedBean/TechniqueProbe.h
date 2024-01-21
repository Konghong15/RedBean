#pragma once

namespace Dcb
{
	class Buffer;
}

class Technique;
class Step;

/*
테크닉 프로브는 테크닉을 순회하며 이벤트 함수를 콜백해주는 용도로,
내부에 있는 정보의 관심사만을 정의하고, 순회 처리는 테크닉 컨테이너에게 맡긴다.
*/

class TechniqueProbe
{
public:
	void SetTechnique(Technique* pTech_in);
	void SetStep(Step* pStep_in);
	bool VisitBuffer(Dcb::Buffer& buf);

protected:
	virtual void OnSetTechnique() {}
	virtual void OnSetStep() {}
	virtual bool OnVisitBuffer(Dcb::Buffer&) { return false; }
protected:
	enum { INVAILD = -1 };

	Technique* mpTechnique = nullptr;
	Step* mpStep = nullptr;
	size_t mTechniqueIndex = INVAILD;
	size_t mStepIndex = INVAILD;
	size_t mBufIndex = INVAILD;
};