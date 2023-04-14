//
// Created by ivan on 10.04.2022.
//

#include "GameObject.h"

OrientedPoint::OrientedPoint(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
    this->position = position;
    this->rotation = glm::quat(rotation * (3.1415f / 180.f));
    this->scale = scale;
}

GameObject::GameObject(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) : OrientedPoint(position, rotation, scale)
{}

glm::vec3 OrientedPoint::getPosition()
{
    return position;
}
glm::quat OrientedPoint::getRotation()
{
    return rotation;
}

glm::vec3 OrientedPoint::getScale()
{
    return scale;
}

glm::mat4 OrientedPoint::getModelMatrix()
{
    return model;
}

glm::mat3 OrientedPoint::getNormalModelMatrix()
{
    return normalModel;
}

void OrientedPoint::update(float deltaTime)
{
    // calculate rotation matrix
    glm::mat4 rotMat = glm::mat4_cast(rotation);

    // model = trans * rot * scale = T * R * S
    model = glm::translate(glm::mat4(1.0f), position); // M = I * T = T
    model = model * rotMat; // M = M * R = T * R
    model = glm::scale(model, scale); // M = M * S = T * R * S

    normalModel = glm::transpose(glm::inverse(glm::mat3(model)));
}

GameObject::~GameObject()
{
    for (Component* component : components)
    {
        delete component;
    }
}

void GameObject::start()
{
    for (Component *component: this->components) {
        component->start();
    }
}

void GameObject::update(float deltaTime)
{
    OrientedPoint::update(deltaTime);
    moved = false;

    if(isActive) {
        for (Component *component: this->components) {
            if (component->isEnabled())
                component->update(deltaTime);
        }
    }
}

void GameObject::render()
{
    if(isActive) {
        for (Component *component: this->components) {
            if (component->isEnabled())
                component->render();
        }
    }
}

void GameObject::move(glm::vec3 delta) {
    if(glm::length(delta) > 0.0001f)
        moved = true;

    this->position += delta;
}

void GameObject::rotate(glm::quat delta) {
    if(glm::length(delta) > 0.0001f)
        moved=true;

    rotation = rotation * delta;
    moved=true;
}

void GameObject::rotate(glm::vec3 delta) {
    rotate(glm::quat(delta));
}

void GameObject::moveAt(glm::vec3 newPos)
{
    if(glm::length(this->position - newPos) > 0.0001f)
        moved = true;

    this->position = newPos;
}

void GameObject::rotateAt(glm::quat newRot) {
    if(glm::length(newRot - rotation) > 0.0001f)
        moved=true;

    rotation = newRot;
}

void GameObject::rotateAt(glm::vec3 newRot) {
    rotate(glm::quat(newRot));
}

void GameObject::setActive(bool value) {
    this->isActive = value;
    moved=true;
}

bool GameObject::activeSelf() {
    return this->isActive;
}

bool GameObject::isMoved() {
    return this->moved;
}
