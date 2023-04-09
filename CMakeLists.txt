cmake_minimum_required(VERSION 3.21)
project(SWTOR)

set(CMAKE_CXX_STANDARD 23)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_executable(SWTOR main.cpp 
src/Engine/Game.h 
src/Engine/Game.cpp 
src/BaseComponents/Camera.h 
src/BaseComponents/Light.h 
src/Engine/Material.h 
src/Engine/GameObject.cpp 
src/Engine/GameObject.h 
src/Engine/Mesh.cpp 
src/Engine/Mesh.h 
src/Engine/MeshRenderer.h 
src/BaseComponents/Model.h 
src/Engine/OBJLoader.h 
src/Engine/Scene.cpp 
src/Engine/Scene.h 
src/Engine/Shader.h 
src/Engine/Texture.h 
src/Engine/Vertex.h 
src/Engine/Vertex.cpp
src/Engine/Input.h 
src/Engine/Input.cpp 
src/Engine/AssimpLoader.cpp 
src/Engine/AssimpLoader.h 
src/Physics/bounds.cpp 
src/Physics/bounds.h 
src/Physics/meshcollider.cpp  
src/Physics/environment.cpp 
src/Physics/environment.h 
src/Physics/linalg.cpp 
src/Physics/linalg.h 
src/Physics/list.hpp 
src/Physics/octree.cpp 
src/Physics/octree.h 
src/Physics/ray.cpp 
src/Physics/ray.h 
src/Physics/rigidbody.cpp 
src/Physics/rigidbody.h 
src/FieldScene.h 
src/FieldScene.cpp
src/Navigation/NavMesh.cpp
src/Navigation/polypartition.cpp
src/Navigation/Struct_utilities.cpp
)

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


