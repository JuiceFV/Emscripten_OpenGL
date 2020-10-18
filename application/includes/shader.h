#pragma once

#ifdef __EMSCRIPTEN__
    #define GLFW_INCLUDE_ES3
    #include <emscripten/emscripten.h>
#else
    #include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include <iostream>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class Shader
{
  public:
    GLuint Program;
    // Constructor generates the shader on the fly
    Shader(const GLchar *vertexPath, const GLchar *fragmentPath);
    // Uses the current shader
    void Use();
};