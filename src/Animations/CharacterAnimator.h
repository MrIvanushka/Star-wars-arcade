#ifndef SWTOR_CHARACTERANIMATOR_H
#define SWTOR_CHARACTERANIMATOR_H

#include"../BaseComponents/Animator.h"
#include"CharacterAnimations.h"

#define IDLE_ANIM 0
#define WALK_ANIM 1
#define JUMP_ANIM 2
#define FALL_ANIM 3

class CharacterAnimator : public Animator{
public:
    CharacterAnimator(GameObject* obj) : Animator(obj) {}

    void setupStateMachine(std::vector<AnimationClip> &clips, CharacterController* controller){

        std::vector<BlendingElement> blendedClips(2);

        blendedClips[0].Clip = &(clips[IDLE_ANIM]);
        blendedClips[0].Value = 0;
        blendedClips[1].Clip = &(clips[WALK_ANIM]);
        blendedClips[1].Value = 1;
        
        clips[JUMP_ANIM].changeSpeed(2.5f);

        MovementAnimation* movementTree = new MovementAnimation(controller, blendedClips, this, 0.1f);
        SingleAnimationState* jumpAnim = new SingleAnimationState(&clips[JUMP_ANIM], this, 0.1f);
        SingleAnimationState* fallAnim = new SingleAnimationState(&clips[FALL_ANIM], this, 0.3f);
        Transition* jumpTransition = new CharacterJumpTransition(jumpAnim, controller);
        Transition* fallTransition = new CharacterFallTransition(fallAnim, controller);
        Transition* exitTransition = new ExitTimeTransition(fallAnim, clips[JUMP_ANIM].getDuration());
        Transition* landTransition = new CharacterLandTransition(movementTree, controller);

        movementTree->addTransition(jumpTransition);
        movementTree->addTransition(fallTransition);
        jumpAnim->addTransition(exitTransition);
        jumpAnim->addTransition(landTransition);
        fallAnim->addTransition(landTransition);

        _stateMachine = new StateMachine(movementTree);
    }
};

#endif