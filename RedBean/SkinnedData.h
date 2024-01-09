#pragma once

struct Keyframe
{
	float TimePos = 0.f;
	Vector3 Translation = { 0.f, 0.f, 0.f };
	Vector3 Scale = { 1.f, 1.f, 1.f };
	Quaternion Rotation;
};

struct BoneAnimation
{
	float GetStartTime() const { return Keyframes.front().TimePos; }
	float GetEndTime() const { return Keyframes.back().TimePos; }
	void Interpolate(float t, Matrix* outMatrix) const;

	std::vector<Keyframe> Keyframes;
};

struct AnimationClip
{
	float GetClipStartTime() const;
	float GetClipEndTime() const;
	void Interpolate(float t, std::vector<Matrix>* outBoneTransforms) const;

	std::vector<BoneAnimation> BoneAnimations;
};

class SkinnedData
{
public:
	size_t GetBoneCount() const { return BoneHierarchy.size(); }
	float GetClipStartTime(const std::string& clipName) const;
	float GetClipEndTime(const std::string& clipName) const;
	void GetFinalTransforms(const string& clipName, float timePos, std::vector<Matrix>* outFinalTransforms) const;

	vector<int> BoneHierarchy;
	vector<Matrix> BoneOffsets;
	map<string, AnimationClip> Animations;
};