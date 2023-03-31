#include "StateMachine.h"

State::State(std::vector<Transition*> transitions) : _transitions(transitions) {}

void State::onEnable()
{
    for(auto& transition : _transitions)
    {
        transition->onEnable();
    }
    start();
}

void State::updateAll(float deltaTime)
{
    for(auto& transition : _transitions)
    {
        transition->update(deltaTime);
    }
    update(deltaTime);
}

bool State::needTransit(Transition* completedTransition)
{
    for(auto& transition : _transitions)
    {
        if(transition->needTransit())
        {
            completedTransition = transition;
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

    if(_currentState->needTransit(completedTransition))
        transit(completedTransition->_nextState);

}

void StateMachine::transit(State* nextState)
{
    _currentState = nextState;
    _currentState->onEnable();
}