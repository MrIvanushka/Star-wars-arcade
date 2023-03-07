//
// Created by ivan on 10.04.2022.
//

#ifndef SWTOR_LIGHT_H
#define SWTOR_LIGHT_H

#include<iostream>

#include<GL/glew.h>
#include"GLFW/glfw3.h"

#include "../../glm/glm/glm.hpp"
#include "../../glm/glm/vec3.hpp"
#include"../../glm/glm/mat4x4.hpp"
#include"../../glm/glm/gtc/matrix_transform.hpp"

#include"../Engine/Shader.h"
#include"../Engine/GameObject.h"

class Light : public Component
{
protected:
    float intensity;
    glm::vec3 color;

public:
    Light(GameObject* object) : Component(object)
    {}

    void initialize(float intensity, glm::vec3 color)
    {
        this->intensity = intensity;
        this->color = color;
    }

    ~Light()
    {

    }

    //Functions
    virtual void sendToShader(Shader& program) = 0;
};

class PointLight : public Light
{
protected:
    float constant;
    float linear;
    float quadratic;

public:
    PointLight(GameObject* object) : Light(object)
    {}

    void initialize(float intensity = 1.f, glm::vec3 color = glm::vec3(1.f),
               float constant = 1.f, float linear = 0.045f, float quadratic = 0.0075f)
    {
        Light::initialize(intensity, color);
        this->constant = constant;
        this->linear = linear;
        this->quadratic = quadratic;
    }

    ~PointLight()
    {

    }

    void sendToShader(Shader& program)
    {
        program.setVec3f(this->gameObject->getPosition(), "pointLight.position");
        program.set1f(this->intensity, "pointLight.intensity");
        program.setVec3f(this->color, "pointLight.color");
        program.set1f(this->constant, "pointLight.constant");
        program.set1f(this->linear, "pointLight.linear");
        program.set1f(this->quadratic, "pointLight.quadratic");
    }
};

#endif //SWTOR_LIGHT_H
