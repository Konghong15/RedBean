//#pragma once
//
//class SkinnedData
//{
//public:
//	SkinnedData(aiMesh* mesh);
//	~SkinnedData() = default;
//
//	size_t GetBoneCount() const { return BoneHierarchy.size(); }
//	float GetClipStartTime(const std::string& clipName) const;
//	float GetClipEndTime(const std::string& clipName) const;
//	void GetFinalTransforms(const string& clipName, float timePos, std::vector<Matrix>* outFinalTransforms) const;
//
//	vector<int> BoneHierarchy;
//	vector<Matrix> BoneOffsets;
//};