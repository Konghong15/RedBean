#pragma once

class Model;

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

	inline void Interpolate(float t, Matrix* outMatrix) const
	{
		if (t <= Keyframes.front().TimePos)
		{
			XMVECTOR S = XMLoadFloat3(&Keyframes.front().Scale);
			XMVECTOR P = XMLoadFloat3(&Keyframes.front().Translation);
			XMVECTOR Q = XMLoadFloat4(&Keyframes.front().Rotation);

			XMVECTOR zero = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
			XMStoreFloat4x4(outMatrix, XMMatrixAffineTransformation(S, zero, Q, P));

			return;
		}
		if (t >= Keyframes.back().TimePos)
		{
			XMVECTOR S = XMLoadFloat3(&Keyframes.back().Scale);
			XMVECTOR P = XMLoadFloat3(&Keyframes.back().Translation);
			XMVECTOR Q = XMLoadFloat4(&Keyframes.back().Rotation);

			XMVECTOR zero = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
			XMStoreFloat4x4(outMatrix, XMMatrixAffineTransformation(S, zero, Q, P));

			return;
		}

		for (UINT i = 0; i < Keyframes.size() - 1; ++i)
		{
			auto& prev = Keyframes[i];
			auto& next = Keyframes[i + 1];

			if (t >= prev.TimePos && t <= next.TimePos)
			{
				float lerpPercent = (t - prev.TimePos) / (next.TimePos - prev.TimePos);

				XMVECTOR s0 = XMLoadFloat3(&prev.Scale);
				XMVECTOR s1 = XMLoadFloat3(&next.Scale);

				XMVECTOR p0 = XMLoadFloat3(&prev.Translation);
				XMVECTOR p1 = XMLoadFloat3(&next.Translation);

				XMVECTOR q0 = XMLoadFloat4(&prev.Rotation);
				XMVECTOR q1 = XMLoadFloat4(&next.Rotation);

				XMVECTOR S = XMVectorLerp(s0, s1, lerpPercent);
				XMVECTOR P = XMVectorLerp(p0, p1, lerpPercent);
				XMVECTOR Q = XMQuaternionSlerp(q0, q1, lerpPercent);

				XMVECTOR zero = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
				XMStoreFloat4x4(outMatrix, XMMatrixAffineTransformation(S, zero, Q, P));

				break;
			}
		}
	}

	std::vector<Keyframe> Keyframes;
};

struct AnimationClip
{
	float GetClipStartTime()const
	{
		float t = common::MathHelper::Infinity;

		for (const auto& animation : BoneAnimations)
		{
			t = common::MathHelper::Min(t, animation.GetStartTime());
		}

		return t;
	}

	float GetClipEndTime()const
	{
		float t = 0.0f;

		for (const auto& animation : BoneAnimations)
		{
			t = common::MathHelper::Max(t, animation.GetEndTime());
		}

		return t;
	}

	void Interpolate(float t, std::vector<Matrix>* outBoneTransforms) const
	{
		for (UINT i = 0; i < BoneAnimations.size(); ++i)
		{
			BoneAnimations[i].Interpolate(t, &(*outBoneTransforms)[i]);
		}
	}

	std::vector<BoneAnimation> BoneAnimations;
};


class SkinnedData
{
public:
	size_t GetBoneCount() const { return BoneHierarchy.size(); }
	float GetClipStartTime(const std::string& clipName) const
	{
		auto find = Animations.find(clipName);
		assert(find != Animations.end());

		return find->second.GetClipStartTime();
	}
	float GetClipEndTime(const std::string& clipName) const
	{
		auto find = Animations.find(clipName);
		assert(find != Animations.end());

		return find->second.GetClipEndTime();
	}
	void GetFinalTransforms(const string& clipName, float timePos, std::vector<XMFLOAT4X4>& finalTransforms) const
	{
		const size_t BONE_COUNT = BoneOffsets.size();

		std::vector<Matrix> toParentTransforms(BONE_COUNT);

		auto clip = Animations.find(clipName);
		assert(clip != Animations.end());
		// 애니메이션 된 toParent를 준비한다.
		clip->second.Interpolate(timePos, &toParentTransforms);

		std::vector<Matrix> toRootTransforms(BONE_COUNT);

		toRootTransforms[0] = toParentTransforms[0];

		// 애니메이션 된 toParent로 모든 본의 toRoot를 구한다.
		// curRoot = curParent * parentRoot;
		for (UINT i = 1; i < BONE_COUNT; ++i)
		{
			const Matrix& toParent = toParentTransforms[i];
			int parentIndex = BoneHierarchy[i];
			const Matrix& parentToRoot = toRootTransforms[parentIndex];

			toRootTransforms[i] = toParent * parentToRoot;
		}

		// BoneOffset : 메쉬 공간에서 본 공간으로의 변환
		// toRootTransforms : 루트 공간으로의 변환
		for (UINT i = 0; i < BONE_COUNT; ++i)
		{
			finalTransforms[i] = BoneOffsets[i] * toRootTransforms[i];
		}
	}

	vector<int> BoneHierarchy;
	vector<Matrix> BoneOffsets;
	map<string, AnimationClip> Animations;
};

class SkinnedModel
{
public:
	shared_ptr<Model> Model;

};

class  SkinnedModelInstance
{
public:
	inline void UpdateToRootTransforms(float deltaTime);

	shared_ptr<SkinnedModel> SkinnedModel;
	float TimePos;
	string ClipName;
	Matrix World;
	vector<Matrix> ToRootTransforms;
};

void SkinnedModelInstance::UpdateToRootTransforms(float deltaTime)
{

}