//
// Created by ivan on 10.04.2022.
//

#ifndef SWTOR_OBJLOADER_H
#define SWTOR_OBJLOADER_H

//STD Libs
#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<sstream>
#include<algorithm>

//GLEW
#include<GL/glew.h>

//GLFW
#include<GLFW/glfw3.h>

//OpenGL Math libs
#include<../../glm/glm/glm.hpp>
#include<../../glm/glm/vec3.hpp>
#include<../../glm/glm/vec4.hpp>
#include<../../glm/glm/mat4x4.hpp>
#include<../../glm/glm/gtc/matrix_transform.hpp>
#include<../../glm/glm/gtc/type_ptr.hpp>

//Own libs
#include"Vertex.h"

static std::vector<Vertex> loadOBJ(const char* file_name)
{
    //Vertex portions
    std::vector<glm::fvec3> vertex_positions;
    std::vector<glm::fvec2> vertex_texcoords;
    std::vector<glm::fvec3> vertex_normals;

    //Face vectors
    std::vector<GLint> vertex_position_indicies;
    std::vector<GLint> vertex_texcoord_indicies;
    std::vector<GLint> vertex_normal_indicies;

    //Vertex array
    std::vector<Vertex> vertices;

    std::stringstream ss;
    std::ifstream in_file(file_name);
    std::string line = "";
    std::string prefix = "";
    glm::vec3 temp_vec3;
    glm::vec2 temp_vec2;
    GLint temp_glint = 0;

    //File open error check
    if (!in_file.is_open())
    {
        throw "ERROR::OBJLOADER::Could not open file.";
    }

    //Read one line at a time
    while (std::getline(in_file, line))
    {
        //Get the prefix of the line
        ss.clear();
        ss.str(line);
        ss >> prefix;

        if (prefix == "#")
        {

        }
        else if (prefix == "o")
        {

        }
        else if (prefix == "s")
        {

        }
        else if (prefix == "use_mtl")
        {

        }
        else if (prefix == "v") //Vertex position
        {
            ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
            vertex_positions.push_back(temp_vec3);
        }
        else if (prefix == "vt")
        {
            ss >> temp_vec2.x >> temp_vec2.y;
            vertex_texcoords.push_back(temp_vec2);
        }
        else if (prefix == "vn")
        {
            ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
            vertex_normals.push_back(temp_vec3);
        }
        else if (prefix == "f")
        {
            int counter = 0;
            while (ss >> temp_glint)
            {
                //Pushing indices into correct arrays
                if (counter == 0)
                    vertex_position_indicies.push_back(temp_glint);
                else if (counter == 1)
                    vertex_texcoord_indicies.push_back(temp_glint);
                else if (counter == 2)
                    vertex_normal_indicies.push_back(temp_glint);

                //Handling characters
                if (ss.peek() == '/')
                {
                    ++counter;
                    ss.ignore(1, '/');
                }
                else if (ss.peek() == ' ')
                {
                    ++counter;
                    ss.ignore(1, ' ');
                }

                //Reset the counter
                if (counter > 2)
                    counter = 0;
            }
        }
        else
        {

        }
    }

    //Build final vertex array (mesh)
    vertices.resize(vertex_position_indicies.size(), Vertex());

    //Load in all indices
    for (size_t i = 0; i < vertices.size(); ++i)
    {
        vertices[i].position = vertex_positions[vertex_position_indicies[i] - 1];
        vertices[i].texcoord = vertex_texcoords[vertex_texcoord_indicies[i] - 1];
        vertices[i].normal = vertex_normals[vertex_normal_indicies[i] - 1];
        vertices[i].color = glm::vec3(1.f, 1.f, 1.f);
    }
    return vertices;
}

static std::vector<std::vector<Vertex>> loadOBJwithManyMeshes(const char* file_name)
{
    //Vertex portions
    std::vector<glm::fvec3> vertex_positions;
    std::vector<glm::fvec2> vertex_texcoords;
    std::vector<glm::fvec3> vertex_normals;

    //Face vectors
    std::vector<std::vector<GLint>> vertex_position_indicies;
    std::vector<std::vector<GLint>> vertex_texcoord_indicies;
    std::vector<std::vector<GLint>> vertex_normal_indicies;
    int current_index = -1;

    //Vertex array
    std::vector<std::vector<Vertex>> vertices;

    std::stringstream ss;
    std::ifstream in_file(file_name);
    std::string line = "";
    std::string prefix = "";
    glm::vec3 temp_vec3;
    glm::vec2 temp_vec2;
    GLint temp_glint = 0;

    //File open error check
    if (!in_file.is_open())
    {
        throw "ERROR::OBJLOADER::Could not open file.";
    }

    //Read one line at a time
    while (std::getline(in_file, line))
    {
        //Get the prefix of the line
        ss.clear();
        ss.str(line);
        ss >> prefix;

        if (prefix == "#")
        {

        }
        else if (prefix == "o")
        {

        }
        else if (prefix == "s")
        {

        }
        else if (prefix == "usemtl")
        {
            current_index++;
            vertex_position_indicies.emplace_back();
            vertex_texcoord_indicies.emplace_back();
            vertex_normal_indicies.emplace_back();
        }
        else if (prefix == "v") //Vertex position
        {
            ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
            vertex_positions.push_back(temp_vec3);
        }
        else if (prefix == "vt")
        {
            ss >> temp_vec2.x >> temp_vec2.y;
            vertex_texcoords.push_back(temp_vec2);
        }
        else if (prefix == "vn")
        {
            ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
            vertex_normals.push_back(temp_vec3);
        }
        else if (prefix == "f")
        {
            int counter = 0;
            while (ss >> temp_glint)
            {
                //Pushing indices into correct arrays
                if (counter == 0)
                    vertex_position_indicies[current_index].push_back(temp_glint);
                else if (counter == 1)
                    vertex_texcoord_indicies[current_index].push_back(temp_glint);
                else if (counter == 2)
                    vertex_normal_indicies[current_index].push_back(temp_glint);

                //Handling characters
                if (ss.peek() == '/')
                {
                    ++counter;
                    ss.ignore(1, '/');
                }
                else if (ss.peek() == ' ')
                {
                    ++counter;
                    ss.ignore(1, ' ');
                }

                //Reset the counter
                if (counter > 2)
                    counter = 0;
            }
        }
        else
        {

        }
    }

    vertices.resize(current_index+1, std::vector<Vertex>());
    for(int i = 0; i <= current_index; i++) {
        //Build final vertex array (mesh)
        vertices[i].resize(vertex_position_indicies[i].size(), Vertex());

        //Load in all indices
        for (size_t j = 0; j < vertices[i].size(); ++j) {
            vertices[i][j].position = vertex_positions[vertex_position_indicies[i][j] - 1];
            vertices[i][j].texcoord = vertex_texcoords[vertex_texcoord_indicies[i][j] - 1];
            vertices[i][j].normal = vertex_normals[vertex_normal_indicies[i][j] - 1];
            vertices[i][j].color = glm::vec3(1.f, 1.f, 1.f);
        }
    }

    return vertices;
}

#endif //SWTOR_OBJLOADER_H
