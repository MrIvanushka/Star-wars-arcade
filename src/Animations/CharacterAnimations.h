#ifndef SWTOR_CHARACTERANIMATIONS_H
#define SWTOR_CHARACTERANIMATIONS_H

#include"../GameComponents/CharacterController.h"
#include"../BaseComponents/Animator.h"

class MovementAnimation : public BlendTree{
private:
    CharacterController* _controller;
public:
    MovementAnimation(CharacterController* controller, std::vector<BlendingElement> clips, IMeshContainer* container, float transitDuration) :
        BlendTree(clips, container, transitDuration), _controller(controller) {}

    MovementAnimation(CharacterController* controller, std::vector<BlendingElement> clips, IMeshContainer* container, float transitDuration, std::vector<Transition*> transitions) :
        BlendTree(clips, container, transitDuration, transitions), _controller(controller) {}

    float updateBlendFactor() override{
        return _controller->getCurrentSpeed() / _controller->getMaxSpeed();
    }
};


class CharacterJumpTransition : public Transition{
private:
    CharacterController* _controller;
public:
    CharacterJumpTransition(State* nextState, CharacterController* controller) : Transition(nextState), _controller(controller)
    {}

    bool needTransit() override
    {
        return _controller->getGravitySpeed() > 0.01f;
    }   
};

class CharacterFallTransition : public Transition{
private:
    CharacterController* _controller;
public:
    CharacterFallTransition(State* nextState, CharacterController* controller) : Transition(nextState), _controller(controller)
    {}

    bool needTransit() override
    {
        return !_controller->isGrounded();
    }
};

class CharacterLandTransition : public Transition{
private:
    CharacterController* _controller;
public:
    CharacterLandTransition(State* nextState, CharacterController* controller) : Transition(nextState), _controller(controller)
    {}

    bool needTransit() override
    {
        return _controller->isGrounded();
    }
};

#endif