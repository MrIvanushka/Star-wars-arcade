//
// Created by ivan on 11.04.2022.
//

#include "Input.h"

Input* Input::instance = nullptr;

Input* Input::getInstance() {
    if (instance == nullptr)
    {
        instance = new Input();
    }

    return instance;
}

void Input::update(GLFWwindow* window)
{
    keyStatus["Esc"] = glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
    keyStatus["W"] = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
    keyStatus["A"] = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
    keyStatus["S"] = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
    keyStatus["D"] = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
    keyStatus["LM"] = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

    double mouseX, mouseY;
    int windowWidth, windowHeight;
    glfwGetCursorPos(window, &mouseX, &mouseY);
    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    this->mousePosition = glm::vec2(mouseX / windowWidth, mouseY / windowHeight);
}

bool Input::keyIsPressed(std::string key) {
    return keyStatus[key];
}

glm::vec2 Input::getMousePosition() {
    return this->mousePosition;
}