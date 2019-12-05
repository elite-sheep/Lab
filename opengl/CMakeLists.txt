cmake_minimum_required(VERSION 3.0)

project(LearnOpenGL)

if(APPLE)
  include(CMakeFindFrameworks)
endif(APPLE)

set( INCLUDE_DIRS
  "/usr/include" 
  "/usr/local/include" 
  )
include_directories( ${INCLUDE_DIRS} )

find_package(X11)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLUT REQUIRED)

find_library(glut_lib GLUT)

find_library(glew_lib GLEW)

find_library(glm_lib GLM)

add_executable(helloworld
  ${CMAKE_SOURCE_DIR}/common/shaderutils.cpp
  ${CMAKE_SOURCE_DIR}/common/shaderutils.h
  ${CMAKE_SOURCE_DIR}/helloworld/main.cpp

  ${CMAKE_SOURCE_DIR}/helloworld/hellofrag.glsl
  ${CMAKE_SOURCE_DIR}/helloworld/hellovertex.glsl
  )

target_link_libraries(helloworld ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${glew_lib} glfw)
set_target_properties(helloworld PROPERTIES OUTPUT_NAME "helloworld")
