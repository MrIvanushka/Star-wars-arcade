#ifndef SWTOR_ANIMATOR_H
#define SWTOR_ANIMATOR_H

#include<string>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include"../Engine/GameObject.h"
#include"../Engine/SkinnedMeshRenderer.h"
#include"../Utilities/StateMachine.h"

class Animator : public Component
{
private:
    StateMachine* _stateMachine;
public:
    Animator(GameObject* obj) : Component(obj)
    {}
    void update(float deltaTime)
    {
        _stateMachine->update(deltaTime);
    }
};

class AnimationState : public State{
protected:
    SkinnedMesh* _mesh;
private:
    float _transitDuration;
    float _currentTime;
public:
    AnimationState(SkinnedMesh* mesh, float transitDuration, std::vector<Transition*> transitions) :
      State(transitions), _mesh(mesh), _transitDuration(transitDuration), _currentTime(0) {}
    
    void start() override;
    void update(float deltaTime) override;
private:
    virtual void getClipTransforms(float currentTime, std::vector<glm::mat4>& transforms) = 0;
};


class AnimationClip
{
private:
    aiAnimation* _animation;
public:
    AnimationClip(aiAnimation* animation) : _animation(animation){ }
    void GetBoneTransforms(SkinnedMesh& mesh, float timeTicks, std::vector<glm::mat4>& Transforms);
    float scoreTimeInTicks(float currentTime);
private:
    void CalcInterpolatedScaling(aiVector3D& out, float animationTime, const aiNodeAnim* pNodeAnim);
    void CalcInterpolatedRotation(aiQuaternion& out, float animationTime, const aiNodeAnim* pNodeAnim);
    void CalcInterpolatedPosition(aiVector3D& out, float AnimationTime, const aiNodeAnim* pNodeAnim);
    uint FindScaling(float animationTime, const aiNodeAnim* pNodeAnim);
    uint FindRotation(float animationTime, const aiNodeAnim* pNodeAnim);
    uint FindPosition(float animationTime, const aiNodeAnim* pNodeAnim);
    const aiNodeAnim* FindNodeAnim(const std::string& nodeName);
    void ReadNodeHierarchy(SkinnedMesh& mesh, float AnimationTimeTicks, const aiNode* pNode, const glm::mat4& ParentTransform, std::vector<glm::mat4>& Transforms);
};


class SingleAnimationState : public AnimationState{
private:
    AnimationClip* _clip;
public:
    SingleAnimationState(AnimationClip* clip, SkinnedMesh* mesh, float transitDuration, std::vector<Transition*> transitions) :
        AnimationState(mesh, transitDuration, transitions), _clip(clip) {}

private:
    void getClipTransforms(float currentTime, std::vector<glm::mat4>& transforms) override;
};


class BlendTree : public AnimationState{
    struct BlendingElement
    {
        const AnimationClip* Clip;
        const float Value;

        BlendingElement(AnimationClip* clip, float value) : Clip(clip), Value(value) {}
    };
private:
    std::vector<BlendingElement> _clips;
    float* _blendFactor;
public:
    BlendTree(std::vector<BlendingElement> clips, SkinnedMesh* mesh, float transitDuration, std::vector<Transition*> transitions) :
        AnimationState(mesh, transitDuration, transitions), _clips(clips) {}

private:
    void getClipTransforms(float currentTime, std::vector<glm::mat4>& transforms) override;
};

class ExitTimeTransition : public Transition
{
private:
    float _duration;
    float _currentTime;
public:
    ExitTimeTransition(State* nextState, float duration) : Transition(nextState), _duration(duration)
    {}

    void onEnable() override
    {
        _currentTime = 0;
    }

    void update(float deltaTime) override
    {
        _currentTime += deltaTime;
    }

    bool needTransit() override
    {
        return _currentTime > _duration;
    }  
};


class ParameterTransition : public Transition
{
private:
    bool* _parameter;
public:
    ParameterTransition(State* nextState, bool* parameter) : Transition(nextState), _parameter(parameter)
    {}

    void onEnable() override{
        *_parameter = false;
    }


    bool needTransit() override
    {
        return *_parameter;
    }   
};

#endif //SWTOR_ANIMATOR_H