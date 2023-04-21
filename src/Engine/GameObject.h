//
// Created by ivan on 10.04.2022.
//

#ifndef SWTOR_GAMEOBJECT_H
#define SWTOR_GAMEOBJECT_H

#include<../../glm/glm/glm.hpp>
#include<../../glm/glm/vec3.hpp>
#include<../../glm/glm/gtc/quaternion.hpp>
#include <../../glm/glm/gtc/matrix_transform.hpp>
#include <set>
#include <stdexcept>

class Component;

class OrientedPoint
{
private:
    glm::mat4 model;
    glm::mat3 normalModel;
protected:
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;
public:
    OrientedPoint(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale = glm::vec3(1.0f));
    glm::vec3 getPosition();
    glm::quat getRotation();
    glm::vec3 getScale();
    glm::mat4 getModelMatrix();
    glm::mat3 getNormalModelMatrix();
    virtual void update(float deltaTime);
};

class GameObject : public OrientedPoint
{
private:
    bool isActive = true;
    bool moved;
    std::set<Component*> components;
public:
    GameObject(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale = glm::vec3(1.0f));
    ~GameObject();
    void start();
    void update(float deltaTime) override;
    void render();
    void move(glm::vec3 delta);
    void rotate(glm::quat delta);
    void rotate(glm::vec3 delta);
    void moveAt(glm::vec3 delta);
    void rotateAt(glm::quat delta);
    void rotateAt(glm::vec3 delta);
    void setActive(bool value);
    bool activeSelf();
    bool isMoved();

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
    bool enabled;
public:
    Component(GameObject* object, bool enabledOnStart = true)
    {
        enabled = enabledOnStart;
        gameObject = object;
    }
    ~Component()
    {
        delete gameObject;
    }
    virtual void start() {}
    virtual void update(float deltaTime) {}
    virtual void render() {}
    GameObject* getGameObject() {return gameObject;}

    bool isEnabled()
    {
        return enabled && gameObject->activeSelf();
    }

    template<typename T>
    T* getComponent()
    {
        return gameObject->getComponent<T>();
    }
};

#endif //SWTOR_GAMEOBJECT_H
