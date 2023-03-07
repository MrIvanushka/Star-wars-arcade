//
// Created by ivan on 10.04.2022.
//

#include "Scene.h"
#include "Game.h"

void Scene::initialize(int GL_VERSION_MAJOR, int GL_VERSION_MINOR, int frameBufferWidth, int frameBufferHeight)
{
    this->initShaders(GL_VERSION_MAJOR, GL_VERSION_MINOR);
    this->initTextures();
    this->initMaterials();
    this->initObjects();
    this->initUniforms(frameBufferWidth, frameBufferHeight);
    this->Collision_Detector.initialize(this->gameObjects);
}

Scene::~Scene()
{
    for (size_t i = 0; i < this->shaders.size(); i++)
        delete this->shaders[i];

    for (size_t i = 0; i < this->textures.size(); i++)
        delete this->textures[i];

    for (size_t i = 0; i < this->materials.size(); i++)
        delete this->materials[i];

    for (GameObject* i : this->gameObjects)
        delete i;
}

void Scene::initUniforms(int frameBufferWidth, int frameBufferHeight)
{
    auto ViewMatrix = renderCamera->getViewMatrix();
    auto ProjectionMatrix = renderCamera->getProjectionMatrix(frameBufferWidth, frameBufferHeight);

    for (Shader * shader : this->shaders)
    {
        shader->setMat4fv(ViewMatrix, "ViewMatrix");
        shader->setMat4fv(ProjectionMatrix, "ProjectionMatrix");

        for(PointLight * pl : this->pointLights)
        {
            pl->sendToShader(*shader);
        }
    }
}

void Scene::updateUniforms(int framebufferWidth, int framebufferHeight)
{
    auto ViewMatrix = renderCamera->getViewMatrix();
    auto ProjectionMatrix = renderCamera->getProjectionMatrix(framebufferWidth, framebufferHeight);
    auto cameraPos = renderCamera->getPosition();

    for (Shader* shader : this->shaders)
    {
        shader->setMat4fv(ViewMatrix, "ViewMatrix");
        shader->setVec3f(cameraPos, "cameraPos");

        pointLights[0]->sendToShader(*shader);
        for (int j = 0; j < pointLights.size(); j++)
        {
            pointLights[j]->sendToShader(*shader);
        }
        shader->setMat4fv(ProjectionMatrix, "ProjectionMatrix");
    }
}

void Scene::update(float deltaTime)
{
    for (GameObject* object : this->gameObjects)
    {
        object->update(deltaTime);
    }
    Collision_Detector.update();
}

void Scene::render()
{
    for (GameObject* object : this->gameObjects)
    {
        object->render();
    }
}

Camera* Scene::getCamera() {
    return this->renderCamera;
}
