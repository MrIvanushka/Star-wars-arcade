#include"Animator.h"
#include <../../glm/glm/gtx/matrix_decompose.hpp>

void AnimationState::start()
{
    _currentTime = 0;
}

void AnimationState::update(float deltaTime)
{
    auto meshTransforms = _mesh->getBoneTransforms();
    
    getClipTransforms(_currentTime, meshTransforms);
    _currentTime += deltaTime;
    
    //do something with blendings

    _mesh->setBoneTransforms(meshTransforms);
}

void SingleAnimationState::getClipTransforms(float currentTime, std::vector<glm::mat4>& transforms)
{
    return _clip->GetBoneTransforms(*_mesh, _clip->scoreTimeInTicks(currentTime), transforms);
}

uint AnimationClip::FindPosition(float AnimationTimeTicks, const aiNodeAnim* pNodeAnim)
{
    for (uint i = 0 ; i < pNodeAnim->mNumPositionKeys - 1 ; i++) {
        float t = (float)pNodeAnim->mPositionKeys[i + 1].mTime;
        if (AnimationTimeTicks < t) {
            return i;
        }
    }

    return 0;
}

void AnimationClip::CalcInterpolatedPosition(aiVector3D& Out, float AnimationTimeTicks, const aiNodeAnim* pNodeAnim)
{
    // we need at least two values to interpolate...
    if (pNodeAnim->mNumPositionKeys == 1) {
        Out = pNodeAnim->mPositionKeys[0].mValue;
        return;
    }

    uint PositionIndex = FindPosition(AnimationTimeTicks, pNodeAnim);
    uint NextPositionIndex = PositionIndex + 1;
    assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
    float t1 = (float)pNodeAnim->mPositionKeys[PositionIndex].mTime;
    float t2 = (float)pNodeAnim->mPositionKeys[NextPositionIndex].mTime;
    float DeltaTime = t2 - t1;
    float Factor = (AnimationTimeTicks - t1) / DeltaTime;
    assert(Factor >= 0.0f && Factor <= 1.0f);
    const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
    const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
    aiVector3D Delta = End - Start;
    Out = Start + Factor * Delta;
}

uint AnimationClip::FindRotation(float AnimationTimeTicks, const aiNodeAnim* pNodeAnim)
{
    assert(pNodeAnim->mNumRotationKeys > 0);

    for (uint i = 0 ; i < pNodeAnim->mNumRotationKeys - 1 ; i++) {
        float t = (float)pNodeAnim->mRotationKeys[i + 1].mTime;
        if (AnimationTimeTicks < t) {
            return i;
        }
    }

    return 0;
}

void AnimationClip::CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTimeTicks, const aiNodeAnim* pNodeAnim)
{
    // we need at least two values to interpolate...
    if (pNodeAnim->mNumRotationKeys == 1) {
        Out = pNodeAnim->mRotationKeys[0].mValue;
        return;
    }

    uint RotationIndex = FindRotation(AnimationTimeTicks, pNodeAnim);
    uint NextRotationIndex = RotationIndex + 1;
    assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
    float t1 = (float)pNodeAnim->mRotationKeys[RotationIndex].mTime;
    float t2 = (float)pNodeAnim->mRotationKeys[NextRotationIndex].mTime;
    float DeltaTime = t2 - t1;
    float Factor = (AnimationTimeTicks - t1) / DeltaTime;
    assert(Factor >= 0.0f && Factor <= 1.0f);
    const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
    const aiQuaternion& EndRotationQ   = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
    aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
    Out.Normalize();
}

uint AnimationClip::FindScaling(float AnimationTimeTicks, const aiNodeAnim* pNodeAnim)
{
    assert(pNodeAnim->mNumScalingKeys > 0);

    for (uint i = 0 ; i < pNodeAnim->mNumScalingKeys - 1 ; i++) {
        float t = (float)pNodeAnim->mScalingKeys[i + 1].mTime;
        if (AnimationTimeTicks < t) {
            return i;
        }
    }

    return 0;
}

void AnimationClip::CalcInterpolatedScaling(aiVector3D& Out, float AnimationTimeTicks, const aiNodeAnim* pNodeAnim)
{
    // we need at least two values to interpolate...
    if (pNodeAnim->mNumScalingKeys == 1) {
        Out = pNodeAnim->mScalingKeys[0].mValue;
        return;
    }

    uint ScalingIndex = FindScaling(AnimationTimeTicks, pNodeAnim);
    uint NextScalingIndex = ScalingIndex + 1;
    assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
    float t1 = (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime;
    float t2 = (float)pNodeAnim->mScalingKeys[NextScalingIndex].mTime;
    float DeltaTime = t2 - t1;
    float Factor = (AnimationTimeTicks - (float)t1) / DeltaTime;
    assert(Factor >= 0.0f && Factor <= 1.0f);
    const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
    const aiVector3D& End   = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
    aiVector3D Delta = End - Start;
    Out = Start + Factor * Delta;
}

void convert_aimatrix_to_glm(glm::mat4& glmMat4, const aiMatrix4x4& aiMatrix) {
	for (auto i = 0; i < 4; i++) {
		for (auto j = 0; j < 4; j++) {
			glmMat4[i][j] = aiMatrix[i][j];
		}
	}
}

void convert_aimatrix_to_glm(glm::mat4& glmMat4, const aiMatrix3x3& aiMatrix) {

	for (auto i = 0; i < 3; i++) {
		for (auto j = 0; j < 3; j++) {
			glmMat4[i][j] = aiMatrix[i][j];
		}
	}

	// The rest would be zero, other than the 4,4.
	glmMat4[0][3] = 0.0f;
	glmMat4[1][3] = 0.0f;
	glmMat4[2][3] = 0.0f;

	glmMat4[3][0] = 0.0f;
	glmMat4[3][1] = 0.0f;
	glmMat4[3][2] = 0.0f;

	glmMat4[3][3] = 1.0f;

}

void AnimationClip::ReadNodeHierarchy(SkinnedMesh& mesh, float AnimationTimeTicks, const aiNode* pNode, const glm::mat4& ParentTransform, std::vector<glm::mat4>& Transforms)
{
    std::string NodeName(pNode->mName.data);

    glm::mat4 NodeTransformation;
    convert_aimatrix_to_glm(NodeTransformation, pNode->mTransformation);

    const aiNodeAnim* pNodeAnim = FindNodeAnim(NodeName);

    if (pNodeAnim) {
        // Interpolate scaling and generate scaling transformation matrix
        aiVector3D Scaling;
        CalcInterpolatedScaling(Scaling, AnimationTimeTicks, pNodeAnim);
        glm::mat4 ScalingM(1.0f);
        ScalingM = glm::scale(ScalingM, glm::vec3(Scaling.x, Scaling.y, Scaling.z));

        // Interpolate rotation and generate rotation transformation matrix
        aiQuaternion RotationQ;
        CalcInterpolatedRotation(RotationQ, AnimationTimeTicks, pNodeAnim);
        glm::mat4 RotationM(1.0f);
        convert_aimatrix_to_glm(RotationM, RotationQ.GetMatrix());

        // Interpolate translation and generate translation transformation matrix
        aiVector3D Translation;
        CalcInterpolatedPosition(Translation, AnimationTimeTicks, pNodeAnim);
        glm::mat4 TranslationM(1.0f);
        TranslationM = glm::translate(TranslationM, glm::vec3(Translation.x, Translation.y, Translation.z));

        // Combine the above transformations
        NodeTransformation = TranslationM * RotationM * ScalingM;
    }

    glm::mat4 GlobalTransformation = ParentTransform * NodeTransformation;

    uint BoneIndex;

    if (mesh.tryFindBoneID(NodeName, BoneIndex)) {
        Transforms[BoneIndex] = /*m_GlobalInverseTransform **/ GlobalTransformation/* * m_BoneInfo[BoneIndex].OffsetMatrix*/;
    }

    for (uint i = 0 ; i < pNode->mNumChildren ; i++) {
        ReadNodeHierarchy(mesh, AnimationTimeTicks, pNode->mChildren[i], GlobalTransformation, Transforms);
    }
}

void AnimationClip::GetBoneTransforms(SkinnedMesh& mesh, float timeTicks, std::vector<glm::mat4>& Transforms)
{
    glm::mat4 Identity(1.0f);

    ReadNodeHierarchy(mesh, timeTicks, mesh.getRoot(), Identity, Transforms);
}

const aiNodeAnim* AnimationClip::FindNodeAnim(const std::string& NodeName)
{
    for (uint i = 0 ; i < _animation->mNumChannels ; i++) {
        const aiNodeAnim* pNodeAnim = _animation->mChannels[i];

        if (std::string(pNodeAnim->mNodeName.data) == NodeName) {
            return pNodeAnim;
        }
    }

    return NULL;
}

float AnimationClip::scoreTimeInTicks(float currentTime)
{
    float TicksPerSecond = (float)(_animation->mTicksPerSecond != 0 ? _animation->mTicksPerSecond : 25.0f);
    float TimeInTicks = currentTime * TicksPerSecond;
    return fmod(TimeInTicks, (float)_animation->mDuration);
}