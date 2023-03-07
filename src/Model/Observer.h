//
// Created by ivan on 15.04.22.
//

#ifndef SWTOR_OBSERVER_H
#define SWTOR_OBSERVER_H

#include <set>

class Observable;

class Observer
{
public:
    virtual ~Observer() = default;
    virtual void handle(Observable&) = 0;
};

class Observable
{
public:
    virtual ~Observable() = default;
    void Attach(Observer& o) { observers.insert(&o); }
    void Detach(Observer& o)
    {
        observers.erase(&o);
    }
    void invoke()
    {
        for (auto* o : observers) {
            o->handle(*this);
        }
    }
private:
    std::set<Observer*> observers;
};

template<typename T>
class ArgObservable;

template<typename T>
class ArgObserver
{
public:
    virtual ~ArgObserver() = default;
    virtual void handle(ArgObservable<T>&, T arg) = 0;
};

template<typename T>
class ArgObservable
{
public:
    virtual ~ArgObservable() = default;
    void Attach(ArgObserver<T>* o) { observers.insert(o); }
    void Detach(ArgObserver<T>* o)
    {
        observers.erase(o);
    }
    void invoke(T arg)
    {
        for (auto* o : observers) {
            o->handle(*this, arg);
        }
    }
private:
    std::set<ArgObserver<T>*> observers;
};

#endif //SWTOR_OBSERVER_H
