#include "pch.h"
#include "Animation.h"

AnimationClip::AnimationClip(aiAnimation* animation)
{
	auto& currentAnimation = animation;

	double totalFrame = currentAnimation->mDuration;
	double framePerSeconds = 1 / (currentAnimation->mTicksPerSecond);
	Duration = totalFrame * framePerSeconds;
	Name = currentAnimation->mName.C_Str();

	for (unsigned int j = 0; j < currentAnimation->mNumChannels; ++j)
	{
		auto& currentChennel = currentAnimation->mChannels[j];

		BoneAnimation animationNode;
		animationNode.Name = currentChennel->mNodeName.C_Str();

		for (unsigned int k = 0; k < currentChennel->mNumPositionKeys; ++k)
		{
			Keyframe keyAnimation;

			keyAnimation.TimePos = static_cast<float>(currentChennel->mPositionKeys[k].mTime * framePerSeconds);

			keyAnimation.Translation.x = currentChennel->mPositionKeys[k].mValue.x;
			keyAnimation.Translation.y = currentChennel->mPositionKeys[k].mValue.y;
			keyAnimation.Translation.z = currentChennel->mPositionKeys[k].mValue.z;

			keyAnimation.Rotation.x = currentChennel->mRotationKeys[k].mValue.x;
			keyAnimation.Rotation.y = currentChennel->mRotationKeys[k].mValue.y;
			keyAnimation.Rotation.z = currentChennel->mRotationKeys[k].mValue.z;
			keyAnimation.Rotation.w = currentChennel->mRotationKeys[k].mValue.w;

			keyAnimation.Scale.x = currentChennel->mScalingKeys[k].mValue.x;
			keyAnimation.Scale.y = currentChennel->mScalingKeys[k].mValue.y;
			keyAnimation.Scale.z = currentChennel->mScalingKeys[k].mValue.z;

			animationNode.Keyframes.push_back(keyAnimation);
		}

		BoneAnimations.push_back(animationNode);
	}
}

AnimationClips::AnimationClips(aiScene* scene)
{
	assert(scene->HasAnimations());

	for (unsigned int i = 0; scene->mNumAnimations; ++i)
	{
		aiAnimation* curAnim = scene->mAnimations[i];

		assert(Animations.find(curAnim->mName.C_Str()) == Animations.end());
		Animations.emplace(curAnim->mName.C_Str(), curAnim);
	}
}