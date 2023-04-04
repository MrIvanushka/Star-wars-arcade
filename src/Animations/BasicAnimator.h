#ifndef SWTOR_BASICANIMATOR_H
#define SWTOR_BASICANIMATOR_H

#include"../BaseComponents/Animator.h"

class BasicAnimator : public Animator{
public:
    BasicAnimator(GameObject* obj) : Animator(obj) {}

    void setupStateMachine(AnimationClip* clip){
        _stateMachine = new StateMachine(new SingleAnimationState(clip, this, 1));
    }
};

#endif