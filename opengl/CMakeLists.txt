cmake_minimum_required(VERSION 3.0)

project(LearnOpenGL)

find_library(opengl_lib OpenGL)

find_library(glut_lib GLUT)

find_library(glew_lib GLEW)

find_library(glm_lib GLM)

add_executable(helloworld
  ./helloworld/main.cpp
  ./common/shaderutils.cpp
  ./common/shaderutils.h

  ./helloworld/hellofrag.glsl
  ./helloworld/hellovertex.glsl
  )

target_link_libraries(helloworld glew ${opengl_lib} ${glut_lib} ${glew_lib})
set_target_properties(helloworld PROPERTIES OUTPUT_NAME "helloworld")
