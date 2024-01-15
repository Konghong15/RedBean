#pragma once

namespace resource
{
	struct Keyframe
	{
		float TimePos = 0.f;
		Vector3 Translation = { 0.f, 0.f, 0.f };
		Vector3 Scale = { 1.f, 1.f, 1.f };
		Quaternion Rotation;
	};

	class Keyframes
	{
	public:
		Keyframes(const string& name, const vector<Keyframe>& keyframes);
		Keyframes() = default;

		Matrix Interpolate(float timePos) const;

		const string& GetName() { return mName; }
		float GetStartTime() const { return mKeyframes.front().TimePos; }
		float GetEndTime() const { return mKeyframes.back().TimePos; }

	private:
		string mName;
		vector<Keyframe> mKeyframes;
	};

	struct AnimationClip
	{
		AnimationClip(aiAnimation* animation);
		~AnimationClip() = default;

		const string& GetName() { return mName; }
		float GetDuration() { return mDuration; }
		const map<string, Keyframes>& GetAnimationNodes() { return mAnimationNodes; }

		string mName;
		float mDuration;
		map<string, Keyframes> mAnimationNodes;
	};

	class AnimationResource
	{
	public:
		AnimationResource() = default;
		~AnimationResource() = default;

		bool Init(const string& filename);

		const string GetFileName() const { return mFilename; }
		const map<string, AnimationClip> GetAnimationClips() const { return mAnimationClips; }
		const AnimationClip& GetAnimationClip(string name) const { return mAnimationClips.find(name)->second; }

	private:
		string mFilename;
		map<string, AnimationClip> mAnimationClips;
	};
}