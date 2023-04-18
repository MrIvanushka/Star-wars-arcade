#include "CharacterController.h"
#include <iostream>

void CharacterController::addStepTrigger(Collider* collider) {
    _stepTrigger = collider;
}

void CharacterController::start() {
    _offsetRotation = glm::angleAxis(glm::radians(180.f), glm::vec3(0,1,0));
    _velocity = glm::vec3(0.f);
    _gravityVelocity = glm::vec3(0.f);
    _selfCollider = gameObject->getComponent<MeshCollider>();
    _selfCollider->Attach(this);
}

void CharacterController::update(float deltaTime) {
    if(deltaTime > 0.5)
        deltaTime = 0.5;

    processCollisions();

    if(_groundNormals.size() > 0 && _gravityVelocity.y < 0.001f)
    {
        _isGrounded = true;
        _gravityVelocity = glm::vec3(0.f);
    }
    else
    {
        _isGrounded = false;
    }

    move(deltaTime);

    if(_isGrounded == false)
        fall(deltaTime);
}

void CharacterController::accelerate(glm::vec3 direction, float deltaTime)
{
    if(_isGrounded)
    {
        _velocity += direction * _acceleration * deltaTime;
        float currentSpeed = glm::length(_velocity);

        if(currentSpeed > _maxSpeed)
            _velocity *= _maxSpeed / currentSpeed;
    }
}

void CharacterController::slowDown(float deltaTime)
{
    if(_isGrounded)
    {
        float currentSpeed = glm::length(_velocity);
        float delta = _acceleration * deltaTime;

        if(currentSpeed <= delta)
            _velocity = glm::vec3(0.f);
        else
            _velocity *= (currentSpeed - delta) / currentSpeed;
    }
}

void CharacterController::jump(float deltaTime){
    if(_isGrounded)
    {
        _gravityVelocity += _jumpForce * glm::vec3(0,1,0) * deltaTime;
    }
}

float CharacterController::getCurrentSpeed()
{
    return glm::length(_velocity);
}

float CharacterController::getGravitySpeed()
{
    return _gravityVelocity.y;
}

float CharacterController::getMaxSpeed()
{
    return _maxSpeed;
}

bool CharacterController::isGrounded()
{
    return _isGrounded;
}

void CharacterController::handle(Collision collision) {
    _touchedColliders.insert(collision.Br.collider);
}

void CharacterController::move(float deltaTime)
{

    glm::vec3 actualVelocity = _velocity;

    for(int i = 0; i < _wallNormals.size(); i++)
    {
        glm::vec3 normal = glm::vec3(_wallNormals[i].x, 0, _wallNormals[i].z);
        float currentCos = glm::dot(actualVelocity, normal);

        if(currentCos < 0)
            actualVelocity -= normal * currentCos;
    }

    if(_groundNormals.size() > 0 && glm::length(actualVelocity) > 0.001f)
    {
        glm::vec3 bestNorm;
        float minCos = 100000;

        for(int i = 0; i < _groundNormals.size(); i++)
        {
            float currentCos = glm::dot(_velocity, _groundNormals[i]);

            if(currentCos < minCos)
            {
                bestNorm = _groundNormals[i];
                minCos = currentCos;
            }
        }
        actualVelocity -= bestNorm * minCos;
    }
    gameObject->move(actualVelocity * deltaTime);

    if(glm::length(_velocity) > 0.01f)
    {
        glm::vec3 movingDirection = glm::normalize(_velocity);
        movingDirection.y = 0;
        gameObject->rotateAt(_offsetRotation * glm::quatLookAt(movingDirection, glm::vec3(0, 1, 0)));
    }
}

void CharacterController::fall(float deltaTime) 
{
    _gravityVelocity += _gravityAcceleration * deltaTime;

    for(int i = 0; i < _wallNormals.size(); i++)
    {
        float currentCos = glm::dot(_gravityVelocity, _wallNormals[i]);

        if(currentCos < 0)
            _gravityVelocity -= _wallNormals[i] * currentCos;
    }
    
    gameObject->move(_gravityVelocity * deltaTime);
}

void CharacterController::processNorm(Collider* touchedCollider, glm::vec3 norm) {
    norm = glm::normalize(norm);
    float groundAngle = glm::degrees(acos(glm::dot(norm, glm::vec3(0,1,0))));

    if(groundAngle > _maxGroundAngle)
    {
        if(_stepTrigger != nullptr)
            if(_stepTrigger->touches(touchedCollider))
                _wallNormals.push_back(norm);
    }
    else if(groundAngle <= _maxGroundAngle)
    {
        _groundNormals.push_back(norm);
    }
}

void CharacterController::processCollisions(){
    _groundNormals = std::vector<glm::vec3>();
    _wallNormals = std::vector<glm::vec3>();
    std::vector<Collider*> notTouchedColliders;

    for(auto collider : _touchedColliders){
        bool foundCollision = false;
        std::vector<Face> touchedFaces;

        MeshCollider* touchedMeshCollider = dynamic_cast<MeshCollider*>(collider);
        if(touchedMeshCollider != nullptr)
        {
            touchedFaces = touchedMeshCollider->getFaces();
        }
        else continue;/*
        {
            TerrainCollider* touchedTerrain = dynamic_cast<TerrainCollider*>(collider);
            if(touchedTerrain != nullptr)
            {
                touchedFaces = touchedTerrain->getFacesInRegion(_selfCollider->getRegion());
            }
            else
                continue;
        }*/

        for(auto& touchedFace : touchedFaces){
            auto faces = _selfCollider->getFaces();
            for(auto& face : faces){
                glm::vec3 gotNorm;

                if(touchedFace.collidesWithFace(collider->getCenter(), face, gameObject, gotNorm)){
                    foundCollision = true;
                    processNorm(collider, gotNorm);
                    break;
                }
            }
        }

        if(foundCollision == false)
        {
            notTouchedColliders.push_back(collider);
        }
    }

    for(int i = 0; i < notTouchedColliders.size(); i++)
        _touchedColliders.erase(notTouchedColliders[i]);
}

