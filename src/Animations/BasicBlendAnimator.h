#ifndef SWTOR_BASICBLENDANIMATOR_H
#define SWTOR_BASICBLENDANIMATOR_H

#include"../BaseComponents/Animator.h"
#include"CharacterAnimations.h"

class BasicBlendAnimator : public Animator{
public:
    BasicBlendAnimator(GameObject* obj) : Animator(obj) {}

    void setupStateMachine(std::vector<AnimationClip> &clips, CharacterController* controller){

        std::vector<BlendingElement> blendedClips(clips.size());

        for(int i = 0; i < clips.size(); i++)
        {
            blendedClips[i].Clip = &(clips[i]);
            blendedClips[i].Value = i;
            blendedClips[i].Value /= (clips.size() - 1);
        }

        MovementAnimation* blendTree = new MovementAnimation(controller, blendedClips, this, 1);
        _stateMachine = new StateMachine(blendTree);
    }
};

#endif