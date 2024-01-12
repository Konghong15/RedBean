#include "pch.h"
#include "Animation.h"

Keyframes::Keyframes(const string& name, const vector<Keyframe>& keyframes)
	: mName(name)
	, mKeyframes(keyframes)
{
}

Matrix Keyframes::Interpolate(float timePos) const
{
	Vector3 position = mKeyframes[0].Translation;
	Quaternion rotation = mKeyframes[0].Rotation;
	Vector3 scaling = mKeyframes[0].Scale;

	for (auto iter = mKeyframes.begin(); iter != mKeyframes.end(); ++iter)
	{
		if (iter->TimePos > timePos)
		{
			auto begin = iter - 1;
			float delta = timePos - begin->TimePos;
			float deltaRatio = delta / (iter->TimePos - begin->TimePos);

			position = Vector3::Lerp(begin->Translation, iter->Translation, deltaRatio);
			rotation = Quaternion::Slerp(begin->Rotation, iter->Rotation, deltaRatio);
			scaling = Vector3::Lerp(begin->Scale, iter->Scale, deltaRatio);

			break;
		}
	}

	return Matrix::CreateScale(scaling)
		* Matrix::CreateFromQuaternion(rotation)
		* Matrix::CreateTranslation(position);
}

AnimationClip::AnimationClip(aiAnimation* animation)
{
	double totalFrame = animation->mDuration;
	double framePerSeconds = 1 / (animation->mTicksPerSecond);

	mDuration = totalFrame * framePerSeconds;
	mName = animation->mName.C_Str();

	for (unsigned int j = 0; j < animation->mNumChannels; ++j)
	{
		auto& currentChennel = animation->mChannels[j];

		vector<Keyframe> keyframes;
		keyframes.reserve(currentChennel->mNumPositionKeys);

		for (unsigned int k = 0; k < currentChennel->mNumPositionKeys; ++k)
		{
			Keyframe keyframe;
			keyframe.TimePos = static_cast<float>(currentChennel->mPositionKeys[k].mTime * framePerSeconds);

			keyframe.Translation.x = currentChennel->mPositionKeys[k].mValue.x;
			keyframe.Translation.y = currentChennel->mPositionKeys[k].mValue.y;
			keyframe.Translation.z = currentChennel->mPositionKeys[k].mValue.z;

			keyframe.Rotation.x = currentChennel->mRotationKeys[k].mValue.x;
			keyframe.Rotation.y = currentChennel->mRotationKeys[k].mValue.y;
			keyframe.Rotation.z = currentChennel->mRotationKeys[k].mValue.z;
			keyframe.Rotation.w = currentChennel->mRotationKeys[k].mValue.w;

			keyframe.Scale.x = currentChennel->mScalingKeys[k].mValue.x;
			keyframe.Scale.y = currentChennel->mScalingKeys[k].mValue.y;
			keyframe.Scale.z = currentChennel->mScalingKeys[k].mValue.z;

			keyframes.push_back(keyframe);
		}

		mAnimationNodes.emplace(currentChennel->mNodeName.C_Str(), keyframes);
	}
}
