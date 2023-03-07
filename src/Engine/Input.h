//
// Created by ivan on 11.04.2022.
//

#ifndef SWTOR_INPUT_H
#define SWTOR_INPUT_H

#include "GLFW/glfw3.h"
#include "../../glm/glm/vec2.hpp"
#include <string>
#include <map>

class Input
{
private:
    static Input* instance;
    Input() {}

    std::map<std::string, bool> keyStatus;
    glm::vec2 mousePosition;
public:
    static Input* getInstance();
    void update(GLFWwindow* window);
    bool keyIsPressed(std::string key);
    glm::vec2 getMousePosition();
};

#endif //SWTOR_INPUT_H
