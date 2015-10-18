cmake_minimum_required(VERSION 2.8)

project(AgeofQuadrones)

add_executable(AgeofQuadrones main.cpp NeoQuad.cpp Dronedemort.cpp MamaQuad.cpp Quadrotor.cpp Background.cpp QuadTimer.cpp camera.cpp)


add_subdirectory(glm/)
include_directories(glm/)

add_subdirectory(Data/)
install(TARGETS AgeofQuadrones RUNTIME DESTINATION bin)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
#find_package(GLM REQUIRED)

set(CMAKE_CXX_FLAGS "-g -Wall")

include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} ${AgeofQuadrones_SOURCE_DIR})
link_directories(${AgeofQuadrones_BINARY_DIR})


target_link_libraries(AgeofQuadrones ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )
