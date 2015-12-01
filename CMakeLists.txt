cmake_minimum_required(VERSION 2.8)

project(AgeofQuadrones)

add_executable(AgeofQuadrones main.cpp NeoQuad.cpp Dronedemort.cpp MamaQuad.cpp Quadrotor.cpp Background.cpp QuadTimer.cpp camera.cpp Timeline.cpp Ammo.cpp raygl/raygl.cpp)
#add_executable(AgeofQuadrones main.cpp NeoQuad.cpp Dronedemort.cpp MamaQuad.cpp Quadrotor.cpp Background.cpp QuadTimer.cpp camera.cpp Timeline.cpp pgmIO.cpp)


add_subdirectory(glm/)
include_directories(glm/)

add_custom_target(build-time-make-directory ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory "frames")
    
#copy all files form Data/ to build/Data
#file(COPY Data/ DESTINATION Data/)
add_custom_command(TARGET AgeofQuadrones PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/Data $<TARGET_FILE_DIR:AgeofQuadrones>/Data)
#install(TARGETS AgeofQuadrones RUNTIME DESTINATION bin)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

find_package(Threads REQUIRED)
#INCLUDE(FindFreeGLUT.cmake)

#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR})
#FIND_PACKAGE(FreeGLUT REQUIRED)

#ADD_DEFINITIONS(-DFREEGLUT_STATIC=1)
#INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include ${FREEGLUT_INCLUDE_DIRS})
#SET(GLUT_LIB ${FREEGLUT_LIBRARIES})

set(CMAKE_CXX_FLAGS "-g -Wall")

include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} ${AgeofQuadrones_SOURCE_DIR})
link_directories(${AgeofQuadrones_BINARY_DIR})


target_link_libraries(AgeofQuadrones ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${CMAKE_THREAD_LIBS_INIT} )
