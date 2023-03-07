//
// Created by ivan on 10.04.2022.
//

#ifndef SWTOR_GAMEOBJECT_H
#define SWTOR_GAMEOBJECT_H

#include<../../glm/glm/glm.hpp>
#include<../../glm/glm/vec3.hpp>
#include<../../glm/glm/gtc/quaternion.hpp>
#include <set>
#include <stdexcept>

class Component;

class OrientedPoint
{
protected:
    glm::vec3 position;
    glm::quat rotation;
public:
    OrientedPoint(glm::vec3 position, glm::vec3 rotation);
    glm::vec3 getPosition();
    glm::quat getRotation();
};

class GameObject : public OrientedPoint
{
private:
    bool isActive = true;
    std::set<Component*> components;
public:
    GameObject(glm::vec3 position, glm::vec3 rotation);
    ~GameObject();
    void update(float deltaTime);
    void render();
    void move(glm::vec3 delta);
    void rotate(glm::quat delta);
    void rotate(glm::vec3 delta);
    void moveAt(glm::vec3 delta);
    void rotateAt(glm::quat delta);
    void rotateAt(glm::vec3 delta);
    void setActive(bool value);
    bool activeSelf();

    template<typename T>
    void addComponent()
    {
        if (this->getComponent<T>() != nullptr)
            throw std::logic_error("GameObject already contains component that you are trying to add.");

        this->components.insert(new T(this));
    }
    template<typename T>
    T* getComponent()
    {
        for (Component* component : components)
        {
            T* typedComponent = dynamic_cast<T*>(component);
            if (typedComponent != nullptr)
                return typedComponent;
        }
        return nullptr;
    }
};


class Component
{
protected:
    GameObject* gameObject;
public:
    bool enabled;

    Component(GameObject* object, bool enabledOnStart = true)
    {
        enabled = enabledOnStart;
        gameObject = object;
    }
    ~Component()
    {
        delete gameObject;
    }
    virtual void update(float deltaTime) {}
    virtual void render() {}
};

#endif //SWTOR_GAMEOBJECT_H
