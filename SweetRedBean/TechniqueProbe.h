#pragma once

namespace Dcb
{
	class Buffer;
}

class Technique;
class Step;

/*
��ũ�� ���κ�� ��ũ���� ��ȸ�ϸ� �̺�Ʈ �Լ��� �ݹ����ִ� �뵵��,
���ο� �ִ� ������ ���ɻ縸�� �����ϰ�, ��ȸ ó���� ��ũ�� �����̳ʿ��� �ñ��.
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