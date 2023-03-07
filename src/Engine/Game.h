//
// Created by ivan on 10.04.2022.
//

#ifndef SWTOR_GAME_H
#define SWTOR_GAME_H

#include <GL/glew.h>
#include "../Components/Camera.h"
#include <vector>
#include "Scene.h"
#include "Input.h"

class Game
{
private:
//Variables
    //Window
    GLFWwindow* window;
    const int WINDOW_WIDTH;
    const int WINDOW_HEIGHT;
    int framebufferWidth;
    int framebufferHeight;

    //OpenGL Context
    const int GL_VERSION_MAJOR;
    const int GL_VERSION_MINOR;

    //Delta time
    float dt;
    float curTime;
    float lastTime;

    std::vector<Scene*> scenes;
    unsigned currentSceneIndex;

//Private functions
    void initGLFW();
    void initWindow(
            const char* title,
            bool resizable
    );
    void initGLEW(); //AFTER CONTEXT CREATION!!!
    void initOpenGLOptions();
    void initScenes();

    void updateUniforms();

//Static variables

public:
//Constructors / Destructors
    Game(
            const char* title,
            const int WINDOW_WIDTH, const int WINDOW_HEIGHT,
            const int GL_VERSION_MAJOR, const int GL_VERSION_MINOR,
            bool resizable
    );
    virtual ~Game();

//Accessors
    int getWindowShouldClose();

//Modifiers
    void setWindowShouldClose();

//Functions
    void updateDt();
    void update();
    void render();
//Static functions
    static void framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH);
};

#endif //SWTOR_GAME_H
