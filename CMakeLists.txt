cmake_minimum_required(VERSION 3.21)
project(SWTOR)

set(CMAKE_CXX_STANDARD 14)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_executable(SWTOR main.cpp src/Engine/Game.h src/Engine/Game.cpp src/Components/Camera.h src/Components/Light.h src/Engine/Material.h src/Engine/GameObject.cpp src/Engine/GameObject.h src/Engine/Mesh.h src/Engine/MeshRenderer.h src/Components/Model.h src/Engine/OBJLoader.h src/Engine/Primitives.h src/Engine/Scene.cpp src/Engine/Scene.h src/Engine/Shader.h src/Engine/Texture.h src/Engine/Vertex.h src/Engine/Input.h src/Engine/Input.cpp src/Engine/CollisionDetector.cpp src/Engine/CollisionDetector.h src/Components/Collider.h src/Components/SphereCollider.h)

add_subdirectory(./glfw)
target_link_libraries(SWTOR glfw)
target_link_libraries(SWTOR OpenGL::GL)

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
target_link_libraries(SWTOR GLEW)

find_library(SOIL libsoil2)
target_link_libraries(SWTOR SOIL)

find_package(OpenGL REQUIRED)


