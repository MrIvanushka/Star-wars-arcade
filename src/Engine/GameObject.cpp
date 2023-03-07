//
// Created by ivan on 10.04.2022.
//

#include "GameObject.h"
#include "../Components/Collider.h"

OrientedPoint::OrientedPoint(glm::vec3 position, glm::vec3 rotation)
{
    this->position = position;
    this->rotation = glm::quat(rotation * (3.1415f / 180.f));
}

GameObject::GameObject(glm::vec3 position, glm::vec3 rotation) : OrientedPoint(position, rotation)
{}

glm::vec3 OrientedPoint::getPosition()
{
    return position;
}
glm::quat OrientedPoint::getRotation()
{
    return rotation;
}

GameObject::~GameObject()
{
    for (Component* component : components)
    {
        delete component;
    }
}

void GameObject::update(float deltaTime)
{
    if(isActive) {
        for (Component *component: this->components) {
            if (component->enabled)
                component->update(deltaTime);
        }
    }
}

void GameObject::render()
{
    if(isActive) {
        for (Component *component: this->components) {
            if (component->enabled)
                component->render();
        }
    }
}

void GameObject::move(glm::vec3 delta) {
    this->position += delta;
}

void GameObject::rotate(glm::quat delta) {
    rotation = rotation * delta;
}

void GameObject::rotate(glm::vec3 delta) {
    rotate(glm::quat(delta));
}

void GameObject::moveAt(glm::vec3 newPos)
{
    this->position = newPos;
}

void GameObject::rotateAt(glm::quat newRot) {
    rotation = newRot;
}

void GameObject::rotateAt(glm::vec3 newRot) {
    rotate(glm::quat(newRot));
}

void GameObject::setActive(bool value) {
    this->isActive = value;
}

bool GameObject::activeSelf() {
    return this->isActive;
}
