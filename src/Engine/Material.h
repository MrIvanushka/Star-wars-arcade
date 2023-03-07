//
// Created by ivan on 10.04.2022.
//

#ifndef SWTOR_MATERIAL_H
#define SWTOR_MATERIAL_H

#include<GL/glew.h>
#include<GLFW/glfw3.h>

#include<../../glm/glm/glm.hpp>
#include<../../glm/glm/vec2.hpp>
#include<../../glm/glm/vec3.hpp>
#include<../../glm/glm/vec4.hpp>
#include<../../glm/glm/mat4x4.hpp>
#include<../../glm/glm/gtc/type_ptr.hpp>

#include"Shader.h"

class Material
{
private:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    GLint diffuseTex;
    GLint specularTex;

public:
    Material(
            glm::vec3 ambient,
            glm::vec3 diffuse,
            glm::vec3 specular,
            GLint diffuseTex,
            GLint specularTex
    )
    {
        this->ambient = ambient;
        this->diffuse = diffuse;
        this->specular = specular;
        this->diffuseTex = diffuseTex;
        this->specularTex = specularTex;
    }

    ~Material(){}

    //Function
    void sendToShader(Shader& program)
    {
        program.setVec3f(this->ambient, "material.ambient");
        program.setVec3f(this->diffuse, "material.diffuse");
        program.setVec3f(this->specular, "material.specular");
        program.set1i(this->diffuseTex, "material.diffuseTex");
        program.set1i(this->specularTex, "material.specularTex");
    }
};

#endif //SWTOR_MATERIAL_H
