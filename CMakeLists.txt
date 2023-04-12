cmake_minimum_required(VERSION 3.21)
project(SWTOR)

set(CMAKE_CXX_STANDARD 14)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_executable(SWTOR main.cpp 
src/Engine/Game.cpp 
src/Engine/GameObject.cpp 
src/Engine/Mesh.cpp 
src/Engine/Scene.cpp 
src/Engine/Vertex.cpp
src/Engine/Input.cpp 
src/Engine/AssimpLoader.cpp 
src/BaseComponents/Animator.cpp
src/GameComponents/CharacterController.cpp
src/Physics/bounds.cpp 
src/Physics/meshcollider.cpp  
src/Physics/environment.cpp 
src/Physics/linalg.cpp 
src/Physics/octree.cpp 
src/Physics/ray.cpp 
src/Physics/rigidbody.cpp 
src/Utilities/StateMachine.cpp
src/FieldScene.cpp)

add_subdirectory(./glfw)
target_link_libraries(SWTOR glfw)
target_link_libraries(SWTOR OpenGL::GL)

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
target_link_libraries(SWTOR GLEW)

find_library(SOIL libsoil2)
target_link_libraries(SWTOR SOIL)

SET(BUILD_SHARED_LIBS OFF)
SET(ASSIMP_BUILD_TESTS OFF)
SET(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF)
SET(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF)
SET(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
SET(ASSIMP_BUILD_FBX_IMPORTER ON)
SET(ASSIMP_BUILD_OBJ_IMPORTER ON)
add_subdirectory(assimp)

target_link_libraries(SWTOR assimp)

find_package(OpenGL REQUIRED)


