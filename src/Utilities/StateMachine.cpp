#include "StateMachine.h"
#include<iostream>
State::State(std::vector<Transition*> transitions) : _transitions(transitions) {
    for(auto transition : transitions)
            processNewTransition(transition);
}

void State::onEnable()
{
    if(_transitions.size() > 0)
        for(auto transition : _transitions)
            transition->onEnable();
    
    start();
}

void State::updateAll(float deltaTime)
{
    for(auto transition : _transitions)
    {
        transition->update(deltaTime);
    }
    update(deltaTime);
}

void State::addTransition(Transition* transition)
{
    _transitions.push_back(transition);
    processNewTransition(transition);
}

bool State::needTransit(Transition** completedTransition)
{
    if(_transitions.size() == 0)
        return false;

    for(auto transition : _transitions)
    {
        if(transition->needTransit())
        {
            *completedTransition = transition;
            return true;
        }
    }
    return false;
}

StateMachine::StateMachine(State* startState) : _currentState(startState)
{
    _currentState->onEnable();
}

void StateMachine::update(float deltaTime)
{
    _currentState->updateAll(deltaTime);

    Transition* completedTransition;

    if(_currentState->needTransit(&completedTransition))
        transit(completedTransition->_nextState);

}

void StateMachine::transit(State* nextState)
{
    nextState->onEnable();
    _currentState = nextState;
}