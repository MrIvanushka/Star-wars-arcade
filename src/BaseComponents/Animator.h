#ifndef SWTOR_ANIMATOR_H
#define SWTOR_ANIMATOR_H

#include<string>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include"../Engine/GameObject.h"
#include"../Engine/SkinnedMeshRenderer.h"
#include"../Utilities/StateMachine.h"
#include<iostream>

class IMeshContainer
{
public:
    virtual SkinnedMesh& operator[](unsigned index) = 0;

    virtual uint attachedMeshCount() = 0;
};

class Animator : public Component, public IMeshContainer
{
protected:
    StateMachine* _stateMachine;
private:
    std::vector<SkinnedMesh*> _attachedMeshes;
public:
    Animator(GameObject* obj) : Component(obj) { }

    void attachMesh(SkinnedMesh* mesh)
    {
        _attachedMeshes.push_back(mesh);
    }

    void update(float deltaTime)
    {
        _stateMachine->update(deltaTime);
    }

    SkinnedMesh& operator[](unsigned index) override{
        return *_attachedMeshes[index];
    }

    uint attachedMeshCount() override{
        return _attachedMeshes.size();
    }
};

class AnimationState : public State{
protected:
    IMeshContainer* _container;
private:
    float _transitDuration;
    float _currentTime;
public:
    AnimationState(IMeshContainer* container, float transitDuration) :
      State(), _container(container), _transitDuration(transitDuration), _currentTime(0) {}
    AnimationState(IMeshContainer* container, float transitDuration, std::vector<Transition*> transitions) :
      State(transitions), _container(container), _transitDuration(transitDuration), _currentTime(0) {}
    
    void start() override;
    void update(float deltaTime) override;
private:
    virtual void getClipTransforms(float currentTime, std::map<std::string, glm::mat4>& transforms) = 0;
};


class AnimationClip
{
private:
    aiAnimation* _animation;
    aiNode* _rootNode;
    std::map<std::string, glm::mat4> _boneOffsets;
    glm::mat4 _globalInverse;
public:
    AnimationClip(aiAnimation* animation, aiNode* root, std::map<std::string, glm::mat4> boneOffsets, glm::mat4 globalInverse) : 
        _animation(animation), _rootNode(root), _boneOffsets(boneOffsets), _globalInverse(globalInverse) {}
    void GetBoneTransforms(float timeTicks, std::map<std::string, glm::mat4>& Transforms);
    float scoreTimeInTicks(float currentTime);
private:
    void CalcInterpolatedScaling(aiVector3D& out, float animationTime, const aiNodeAnim* pNodeAnim);
    void CalcInterpolatedRotation(aiQuaternion& out, float animationTime, const aiNodeAnim* pNodeAnim);
    void CalcInterpolatedPosition(aiVector3D& out, float AnimationTime, const aiNodeAnim* pNodeAnim);
    uint FindScaling(float animationTime, const aiNodeAnim* pNodeAnim);
    uint FindRotation(float animationTime, const aiNodeAnim* pNodeAnim);
    uint FindPosition(float animationTime, const aiNodeAnim* pNodeAnim);
    const aiNodeAnim* FindNodeAnim(const std::string& nodeName);
    void ReadNodeHierarchy(float AnimationTimeTicks, const aiNode* pNode, const glm::mat4& ParentTransform, std::map<std::string, glm::mat4>& Transforms);
};


class SingleAnimationState : public AnimationState{
private:
    AnimationClip* _clip;
public:
    SingleAnimationState(AnimationClip* clip, IMeshContainer* container, float transitDuration) :
        AnimationState(container, transitDuration), _clip(clip) {}
    SingleAnimationState(AnimationClip* clip, IMeshContainer* container, float transitDuration, std::vector<Transition*> transitions) :
        AnimationState(container, transitDuration, transitions), _clip(clip) {}

private:
    void getClipTransforms(float currentTime, std::map<std::string, glm::mat4>& transforms) override;
};


struct BlendingElement
{
    AnimationClip* Clip;
    float Value;
};

class BlendTree : public AnimationState{
    
private:
    std::vector<BlendingElement> _clips;
    float _blendFactor;
public:
    BlendTree(std::vector<BlendingElement> clips, IMeshContainer* container, float transitDuration) :
        AnimationState(container, transitDuration), _clips(clips) {}

    BlendTree(std::vector<BlendingElement> clips, IMeshContainer* container, float transitDuration, std::vector<Transition*> transitions) :
        AnimationState(container, transitDuration, transitions), _clips(clips) {}

    void update(float deltaTime) override{
        _blendFactor = updateBlendFactor();
        AnimationState::update(deltaTime);
    }
private:
    void getClipTransforms(float currentTime, std::map<std::string, glm::mat4>& transforms) override;
    virtual float updateBlendFactor() = 0;
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