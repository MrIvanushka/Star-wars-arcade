//
// Created by ivan on 10.04.2022.
//

#ifndef SWTOR_CAMERA_H
#define SWTOR_CAMERA_H

#include<iostream>

#include<GL/glew.h>
#include<GLFW/glfw3.h>

#include "../../glm/glm/glm.hpp"
#include "../../glm/glm/vec3.hpp"
#include<../../glm/glm/mat4x4.hpp>
#include<../../glm/glm/gtc/matrix_transform.hpp>
#include<../../glm/glm/gtx/euler_angles.hpp>

#include "../Engine/GameObject.h"

enum direction {FORWARD = 0, BACKWARD, LEFT, RIGHT};

class Camera : public Component {
private:
    glm::mat4 ViewMatrix;
    glm::mat4 ProjectionMatrix;

    glm::vec3 worldUp;
    glm::vec3 front;
    glm::vec3 right;
    glm::vec3 up;

    float fov;
    float nearPlane;
    float farPlane;

    void updateCameraVectors() {
        auto rotation = this->gameObject->getRotation();
        this->front = glm::vec3(0, 0, 1) * rotation;

        this->front = glm::normalize(this->front);
        this->right = glm::normalize(glm::cross(this->front, this->worldUp));
        this->up = glm::normalize(glm::cross(this->right, this->front));
    }

public:
    Camera(GameObject *object) : Component(object) {
        this->ViewMatrix = glm::mat4(1.f);

        this->worldUp = glm::vec3(0.f, 1.f, 0.f);;
        this->right = glm::vec3(0.f);
        this->up = worldUp;

        this->fov = 90.f;
        this->nearPlane = 0.1f;
        this->farPlane = 1000.f;

        this->updateCameraVectors();
    }

    ~Camera() {}

    //Accessors
    const glm::mat4 getViewMatrix() {
        this->updateCameraVectors();

        auto position = this->gameObject->getPosition();
        this->ViewMatrix = glm::lookAt(position, position + this->front, this->up);
        return this->ViewMatrix;
    }

    glm::mat4 getProjectionMatrix(int framebufferWidth, int framebufferHeight)
    {
        this->ProjectionMatrix = glm::mat4(1.f);
        this->ProjectionMatrix = glm::perspective(
                glm::radians(this->fov),
                static_cast<float>(framebufferWidth) / framebufferHeight,
                this->nearPlane,
                this->farPlane
        );
        return ProjectionMatrix;
    }

    glm::vec3 getPosition()
    {
        return this->gameObject->getPosition();
    }
};

#endif //SWTOR_CAMERA_H
