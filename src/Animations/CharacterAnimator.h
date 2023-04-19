#ifndef SWTOR_CHARACTERANIMATOR_H
#define SWTOR_CHARACTERANIMATOR_H

#include"../BaseComponents/Animator.h"
#include"../Utilities/BasicTransitions.h"
#include"CharacterAnimations.h"

#define IDLE_ANIM 0
#define WALK_ANIM 1
#define RUN_ANIM 2
#define JUMP_ANIM 3
#define FALL_ANIM 4
#define ATTACK_ANIM 5

class CharacterAnimator : public Animator{
public:
    CharacterAnimator(GameObject* obj) : Animator(obj) {}

    void setupStateMachine(std::vector<AnimationClip> &clips, CharacterController* controller, Observable* attackController, Observer* attackListener){

        std::vector<BlendingElement> blendedClips(3);

        blendedClips[0].Clip = &(clips[IDLE_ANIM]);
        blendedClips[0].Value = 0;
        blendedClips[1].Clip = &(clips[WALK_ANIM]);
        blendedClips[1].Value = 0.3;
        blendedClips[2].Clip = &(clips[RUN_ANIM]);
        blendedClips[2].Value = 1;
        
        clips[JUMP_ANIM].changeSpeed(2.5f);
        clips[ATTACK_ANIM].changeSpeed(2.5f);

        MovementAnimation* movementTree = new MovementAnimation(controller, blendedClips, this, 0.1f);
        SingleAnimationState* jumpAnim = new SingleAnimationState(&clips[JUMP_ANIM], this, 0.1f);
        SingleAnimationState* fallAnim = new SingleAnimationState(&clips[FALL_ANIM], this, 0.3f);
        SingleAnimationState* attackAnim = new SingleAnimationState(&clips[ATTACK_ANIM], this, 0.3f);
        attackAnim->addListener("DealDamage", attackListener);

        Transition* jumpTransition = new CharacterJumpTransition(jumpAnim, controller);
        Transition* fallTransition = new CharacterFallTransition(fallAnim, controller);
        Transition* exitJumpTransition = new ExitTimeTransition(fallAnim, clips[JUMP_ANIM].getDuration());
        Transition* exitAttackTransition = new ExitTimeTransition(movementTree, clips[ATTACK_ANIM].getDuration());
        Transition* landTransition = new CharacterLandTransition(movementTree, controller);
        ParameterTransition* attackTransition = new ParameterTransition(attackAnim);
        attackController->Attach(*attackTransition);

        movementTree->addTransition(jumpTransition);
        movementTree->addTransition(fallTransition);
        movementTree->addTransition(attackTransition);
        jumpAnim->addTransition(exitJumpTransition);
        jumpAnim->addTransition(landTransition);
        fallAnim->addTransition(landTransition);
        attackAnim->addTransition(exitAttackTransition);

        _stateMachine = new StateMachine(movementTree);
    }
};

#endif