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

	string Name;
	vector<Keyframe> Keyframes;
};

struct AnimationClip
{
	AnimationClip(aiAnimation* animation);
	~AnimationClip() = default;

	float GetClipStartTime() const;
	float GetClipEndTime() const;
	void Interpolate(float t, std::vector<Matrix>* outBoneTransforms) const;

	string Name;
	float Duration;
	vector<BoneAnimation> BoneAnimations;
};

struct AnimationClips
{
	AnimationClips(aiScene* scene);
	~AnimationClips() = default;
	
	string Name;
	map<string, AnimationClip> Animations;
};

