#ifndef SWTOR_STATEMACHINE_H
#define SWTOR_STATEMACHINE_H

#include <vector>

class Transition;

class State
{
private:
    std::vector<Transition*> _transitions;
protected:
    virtual void start() {}

    virtual void update(float deltaTime) {}
public:
    State(){}
    
    State(std::vector<Transition*> transitions);

    void onEnable();
    
    void updateAll(float deltaTime);
    
    void addTransition(Transition* transition);

    bool needTransit(Transition** completedTransition);
};

class Transition
{
private:
    State* _nextState;
public:
    friend class StateMachine;

    Transition(State* nextState) : _nextState(nextState) {}

    virtual void onEnable() {}

    virtual void update(float deltaTime) {} 

    virtual bool needTransit() = 0;
};

class StateMachine
{
private:
    State* _currentState;

public:
    StateMachine(State* startState);

    void update(float deltaTime);

private:
    void transit(State* nextState); 
};

#endif