//
// Created by ivan on 10.04.2022.
//

#ifndef SWTOR_SCENE_H
#define SWTOR_SCENE_H

#include <vector>
#include"Texture.h"
#include"Shader.h"
#include"Material.h"
#include "../Components/Camera.h"
#include "GameObject.h"
#include "../Components/Light.h"
#include "../Components/Model.h"
#include "CollisionDetector.h"

class Scene
{
protected:
    CollisionDetector Collision_Detector;
    std::vector<Shader*> shaders;
    std::vector<Texture*> textures;
    std::vector<Material*> materials;
    std::vector<GameObject*> gameObjects;
    std::vector<PointLight*> pointLights;
    Camera* renderCamera;
    void initialize(int GL_VERSION_MAJOR, int GL_VERSION_MINOR, int frameBufferWidth, int frameBufferHeight);
private:
    virtual void initShaders(int GL_VERSION_MAJOR, int GL_VERSION_MINOR) = 0;
    virtual void initTextures() = 0;
    virtual void initMaterials() = 0;
    virtual void initObjects() = 0;
    void initUniforms(int frameBufferWidth, int frameBufferHeight);
public:
    ~Scene();
    void updateUniforms(int framebufferWidth, int framebufferHeight);
    void update(float deltaTime);
    void render();
    Camera* getCamera();
};

#endif //SWTOR_SCENE_H
