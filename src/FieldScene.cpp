#include "FieldScene.h"
#include "Engine/OBJLoader.h"
#include "Engine/AssimpLoader.h"
#include "Physics/rigidbody.h"
#include"GameComponents/CameraController.h"
#include"GameComponents/CameraFollower.h"

FieldScene::FieldScene(int GL_VERSION_MAJOR, int GL_VERSION_MINOR, int framebufferWidth, int framebufferHeight)
{
    this->initialize(GL_VERSION_MAJOR, GL_VERSION_MINOR, framebufferWidth, framebufferHeight);
}

void FieldScene::initShaders(int GL_VERSION_MAJOR, int GL_VERSION_MINOR)
{
    this->shaders.push_back(new Shader(GL_VERSION_MAJOR, GL_VERSION_MINOR,
                                       "Shaders/vertex_core.glsl", "Shaders/fragment_directional.glsl"));
    this->shaders.push_back(new Shader(GL_VERSION_MAJOR, GL_VERSION_MINOR,
                                       "Shaders/vertex_unlit.glsl", "Shaders/fragment_unlit.glsl"));
}

void FieldScene::initTextures()
{
    this->textures.push_back(new Texture("Images/skybox.png", GL_TEXTURE_2D));
    this->textures.push_back(new Texture("Images/red.png", GL_TEXTURE_2D));
    this->textures.push_back(new Texture("Images/red.png", GL_TEXTURE_2D));
}

void FieldScene::initMaterials()
{
    this->materials.push_back(new Material(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f), 0, 1));
}

void FieldScene::initObjects()
{
    GameObject* skybox = new GameObject(glm::vec3(2.f, 0.f, 0.f), glm::vec3(180.f, 0.f, 0.f));
    skybox->addComponent<Model>();
    std::vector<Vertex> mesh = loadOBJ("OBJFiles/skybox.obj");
    skybox->getComponent<Model>()->addMesh(mesh, this->materials[0], this->shaders[1], this->textures[0], this->textures[0]);
    this->gameObjects.push_back(skybox);

    //importing 3d model
    auto holocroneData = AssimpLoader::load("OBJFiles/Cube.fbx");
    //creating pure gameobject with set location, rotation and scale  
    GameObject* holocrone1 = new GameObject(glm::vec3(0.f, -0.5f, 0.f), glm::vec3(0.f, 0.f,0.f), glm::vec3(7.f, 0.5f, 7.f));
    //initializing rendering classes
    holocrone1->addComponent<Model>();
    Mesh* holoMesh = new Mesh(holocroneData[0].vertices.data(), holocroneData[0].vertices.size(), holocroneData[0].indices.data(), holocroneData[0].indices.size(), holocrone1);
    holocrone1->getComponent<Model>()->addMesh(holoMesh, this->materials[0], this->shaders[0], this->textures[1], this->textures[2]);
    //setting collision system
    holocrone1->addComponent<MeshCollider>();
    holocrone1->getComponent<MeshCollider>()->initialize(holocroneData[0].vertices, holocroneData[0].indices);
    holocroneData[0].br.collider = holocrone1->getComponent<MeshCollider>();
    collisionProcessor->addToPending(holocrone1, &holocroneData[0].br);
    //moving gameobject to pool
    this->gameObjects.push_back(holocrone1);
    
    GameObject* holocrone2 = new GameObject(glm::vec3(0.f, 10.f, 0.f), glm::vec3(0.f));
    holocrone2->addComponent<Model>();
    holoMesh = new Mesh(holocroneData[0].vertices.data(), holocroneData[0].vertices.size(), holocroneData[0].indices.data(), holocroneData[0].indices.size(), holocrone2);
    holocrone2->getComponent<Model>()->addMesh(holoMesh, this->materials[0], this->shaders[0], this->textures[1], this->textures[2]);
    
    this->gameObjects.push_back(holocrone2);

    GameObject* camera = new GameObject(glm::vec3(-15.f, 0.f, 0.f), glm::vec3(0.f, -90.f, 0.f));
    camera->addComponent<CameraController>();
    camera->addComponent<CameraFollower>();
    camera->getComponent<CameraFollower>()->setTarget(holocrone2);
    camera->addComponent<Camera>();
    this->renderCamera = camera->getComponent<Camera>();
    this->gameObjects.push_back(camera);

    GameObject* direcionalLight = new GameObject(glm::vec3(-50.f, 50.f, 50.f), glm::vec3(0.f, 0.f, 0.f));
    direcionalLight->addComponent<PointLight>();
    this->pointLights.push_back(direcionalLight->getComponent<PointLight>());
    this->gameObjects.push_back(direcionalLight);
}