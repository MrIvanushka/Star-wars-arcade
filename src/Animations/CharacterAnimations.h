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

#endif