#include "FieldScene.h"
#include "Engine/OBJLoader.h"

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
    std::cout << "Initializing textures...\n";
    this->textures.push_back(new Texture("Images/skybox.png", GL_TEXTURE_2D));
    std::cout << "Successfully initialized textures\n";
}

void FieldScene::initMaterials()
{
    std::cout << "Initializing materials...\n";
    this->materials.push_back(new Material(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f), 0, 1));
    std::cout << "Successfully initialized materials\n";
}

void FieldScene::initObjects()
{
    std::cout << "Initializing objects...\n";
    GameObject* skybox = new GameObject(glm::vec3(2.f, 0.f, 2.f), glm::vec3(180.f, 0.f, 0.f));
    skybox->addComponent<Model>();
    std::vector<Vertex> mesh = loadOBJ("OBJFiles/skybox.obj");
    skybox->getComponent<Model>()->addMesh(mesh, 5.f,  this->materials[0], this->shaders[1], this->textures[0], this->textures[0]);
    this->gameObjects.push_back(skybox);
    std::cout << "Created skybox\n";
    GameObject* camera = new GameObject(glm::vec3(-10.f, 0.f, 0.f), glm::vec3(0.f, -90.f, 0.f));
    camera->addComponent<Camera>();
    this->renderCamera = camera->getComponent<Camera>();
    this->gameObjects.push_back(camera);
    std::cout << "Created camera\n";
    GameObject* direcionalLight = new GameObject(glm::vec3(-50.f, 50.f, 50.f), glm::vec3(0.f, 0.f, 0.f));
    direcionalLight->addComponent<PointLight>();
    this->pointLights.push_back(direcionalLight->getComponent<PointLight>());
    this->gameObjects.push_back(direcionalLight);
    std::cout << "Successfully initialized objects\n";
}