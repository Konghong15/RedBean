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

		const string& GetName() const { return mName; }
		float GetStartTime() const { return mKeyframes.front().TimePos; }
		float GetEndTime() const { return mKeyframes.back().TimePos; }

	private:
		string mName;
		vector<Keyframe> mKeyframes;
	};

	class AnimationClip
	{
	public:
		AnimationClip(aiAnimation* animation);
		~AnimationClip() = default;

		inline const Keyframes& Getkeyframes(string boneName) const;
		inline bool Getkeyframes(string boneName, Keyframes& keyframes) const;

		const string& GetName() const { return mName; }
		float GetDuration() const { return mDuration; }
		const map<string, Keyframes>& GetBoneKeyframes() const { return mBoneKeyframes; }

	private:
		string mName;
		float mDuration;
		map<string, Keyframes> mBoneKeyframes;
	};

	inline const Keyframes& AnimationClip::Getkeyframes(string boneName) const
	{
		auto find = mBoneKeyframes.find(boneName);
		assert(find != mBoneKeyframes.end());

		return find->second;
	};

	inline bool AnimationClip::Getkeyframes(string boneName, Keyframes& keyframes) const
	{
		auto find = mBoneKeyframes.find(boneName);

		if (find == mBoneKeyframes.end())
		{
			return false;
		}

		keyframes = find->second;

		return true;
	}

	class AnimationResource
	{
	public:
		AnimationResource(const aiScene* scene);
		~AnimationResource() = default;

		const AnimationClip& GetAnimationClip(string name) const { return mAnimationClips.find(name)->second; }

		const string GetFileName() const { return mName; }
		const map<string, AnimationClip> GetAnimationClips() const { return mAnimationClips; }

	private:
		string mName;
		map<string, AnimationClip> mAnimationClips;
	};
}