#ifndef JEDISTATEMACHINE_H
#define JEDISTATEMACHINE_H

#include "../Utilities/StateMachine.h"

#include "CharacterStates.hpp"

class Ai;  // interface

class Jedi : public Ai;

#endif // JEDISTATEMACHINE_H

//Ai - component, stores statemachine and updates it
//Jedi, soldier <- Ai 
//Ai has void setupSateMachine()

//Vision - component, stores collider
// merge main
