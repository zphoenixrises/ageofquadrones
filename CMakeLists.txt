cmake_minimum_required(VERSION 2.8)

project(neoquadrotor)

add_executable(neoquadrotor main.cpp)


add_subdirectory(glm/)
include_directories(glm/)

install(TARGETS neoquadrotor RUNTIME DESTINATION bin)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
#find_package(GLM REQUIRED)

set(CMAKE_CXX_FLAGS "-g -Wall")

include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} ${neoquadrotor_SOURCE_DIR})
link_directories(${neoquadrotor_BINARY_DIR})

add_library(NeoQuad NeoQuad.cpp)
add_library(Quadrotor Quadrotor.cpp)
add_library(QuadTimer QuadTimer.cpp)
target_link_libraries(neoquadrotor ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} NeoQuad Quadrotor QuadTimer)
