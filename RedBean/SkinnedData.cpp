//#include "pch.h"
//
//#include "SkinnedData.h"
//#include "Animation.h"
//
//float SkinnedData::GetClipStartTime(const std::string& clipName) const
//{
//	auto find = Animations.find(clipName);
//	assert(find != Animations.end());
//
//	return find->second.GetClipStartTime();
//}
//
//float SkinnedData::GetClipEndTime(const std::string& clipName) const
//{
//	auto find = Animations.find(clipName);
//	assert(find != Animations.end());
//
//	return find->second.GetClipEndTime();
//}
//
//void SkinnedData::GetFinalTransforms(const string& clipName, float timePos, std::vector<Matrix>* outFinalTransforms) const
//{
//	const size_t BONE_COUNT = BoneOffsets.size();
//
//	std::vector<Matrix> toParentTransforms(BONE_COUNT);
//
//	auto clip = Animations.find(clipName);
//	assert(clip != Animations.end());
//	// �ִϸ��̼� �� toParent�� �غ��Ѵ�.
//	clip->second.Interpolate(timePos, &toParentTransforms);
//
//	std::vector<Matrix> toRootTransforms(BONE_COUNT);
//
//	toRootTransforms[0] = toParentTransforms[0];
//
//	// �ִϸ��̼� �� toParent�� ��� ���� toRoot�� ���Ѵ�.
//	// curRoot = curParent * parentRoot;
//	for (UINT i = 1; i < BONE_COUNT; ++i)
//	{
//		const Matrix& toParent = toParentTransforms[i];
//		int parentIndex = BoneHierarchy[i];
//		const Matrix& parentToRoot = toRootTransforms[parentIndex];
//
//		toRootTransforms[i] = toParent * parentToRoot;
//	}
//
//	// BoneOffset : �޽� �������� �� ���������� ��ȯ
//	// toRootTransforms : ��Ʈ ���������� ��ȯ
//	for (UINT i = 0; i < BONE_COUNT; ++i)
//	{
//		(*outFinalTransforms)[i] = BoneOffsets[i] * toRootTransforms[i];
//	}
//}