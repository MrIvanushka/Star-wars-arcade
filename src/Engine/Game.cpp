//
// Created by ivan on 10.04.2022.
//
#include "Game.h"

void Game::initGLFW()
{
    if (glfwInit() == GLFW_FALSE)
    {
        std::cout << "ERROR::GLFW_INIT_FAILED" << "\n";
        glfwTerminate();
    }
}

void Game::initWindow(
        const char* title,
        bool resizable
)
{
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, this->GL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, this->GL_VERSION_MINOR);
    glfwWindowHint(GLFW_RESIZABLE, resizable);

    this->window = glfwCreateWindow(this->WINDOW_WIDTH, this->WINDOW_HEIGHT, title, NULL, NULL);

    if (this->window == nullptr)
    {
        std::cout << "ERROR::GLFW_WINDOW_INIT_FAILED" << "\n";
        glfwTerminate();
    }

    glfwGetFramebufferSize(this->window, &this->framebufferWidth, &this->framebufferHeight);
    glfwSetFramebufferSizeCallback(window, Game::framebuffer_resize_callback);

    glfwMakeContextCurrent(this->window); //IMPORTANT!!
}

void Game::initGLEW()
{
    //INIT GLEW (NEEDS WINDOW AND OPENGL CONTEXT)
    glewExperimental = GL_TRUE;

    //Error
    if (glewInit() != GLEW_OK)
    {
        std::cout << "ERROR::MAIN.CPP::GLEW_INIT_FAILED" << "\n";
        glfwTerminate();
    }
}

void Game::initOpenGLOptions()
{
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //Input
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Game::initScenes()
{
    //this->scenes.push_back(new SpaceScene(GL_VERSION_MAJOR, GL_VERSION_MINOR, framebufferWidth, framebufferHeight));
    this->currentSceneIndex = 0;
}

void Game::updateUniforms()
{
    this->scenes[currentSceneIndex]->updateUniforms(framebufferWidth, framebufferHeight);
}

//Constructors / Destructors
Game::Game(
        const char* title,
        const int WINDOW_WIDTH, const int WINDOW_HEIGHT,
        const int GL_VERSION_MAJOR, const int GL_VERSION_MINOR,
        bool resizable
)
        :
        WINDOW_WIDTH(WINDOW_WIDTH),
        WINDOW_HEIGHT(WINDOW_HEIGHT),
        GL_VERSION_MAJOR(GL_VERSION_MAJOR),
        GL_VERSION_MINOR(GL_VERSION_MINOR)
{
    //Init variables
    this->window = nullptr;
    this->framebufferWidth = this->WINDOW_WIDTH;
    this->framebufferHeight = this->WINDOW_HEIGHT;

    this->dt = 0.f;
    this->curTime = 0.f;
    this->lastTime = 0.f;

    this->initGLFW();
    this->initWindow(title, resizable);
    this->initGLEW();
    this->initOpenGLOptions();
    this->initScenes();
}

Game::~Game()
{
    glfwDestroyWindow(this->window);
    delete scenes[currentSceneIndex];
    glfwTerminate();
}

//Accessor
int Game::getWindowShouldClose()
{
    return glfwWindowShouldClose(this->window);
}

//Modifier
void Game::setWindowShouldClose()
{
    glfwSetWindowShouldClose(this->window, GLFW_TRUE);
}

//Functions
void Game::updateDt()
{
    this->curTime = static_cast<float>(glfwGetTime());
    this->dt = this->curTime - this->lastTime;
    this->lastTime = this->curTime;
}

void Game::update()
{
    this->updateDt();
    glfwPollEvents();
    Input::getInstance()->update(window);
    this->scenes[currentSceneIndex]->update(dt);
}

void Game::render()
{
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    //Update the uniforms
    this->updateUniforms();

    this->scenes[currentSceneIndex]->render();

    //End Draw
    glfwSwapBuffers(window);
    glFlush();

    glBindVertexArray(0);
    glUseProgram(0);
    glActiveTexture(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

//Static functions
void Game::framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH)
{
    glViewport(0, 0, fbW, fbH);
};


