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
//	// 애니메이션 된 toParent를 준비한다.
//	clip->second.Interpolate(timePos, &toParentTransforms);
//
//	std::vector<Matrix> toRootTransforms(BONE_COUNT);
//
//	toRootTransforms[0] = toParentTransforms[0];
//
//	// 애니메이션 된 toParent로 모든 본의 toRoot를 구한다.
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
//	// BoneOffset : 메쉬 공간에서 본 공간으로의 변환
//	// toRootTransforms : 루트 공간으로의 변환
//	for (UINT i = 0; i < BONE_COUNT; ++i)
//	{
//		(*outFinalTransforms)[i] = BoneOffsets[i] * toRootTransforms[i];
//	}
//}