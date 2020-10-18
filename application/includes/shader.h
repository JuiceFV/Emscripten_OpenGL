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
  private:
    // Member vars
    unsigned int program;
    std::string loadShaderFile(const char *path);
    unsigned int compileShader(unsigned int type, const char *path);
    void linkProgram(unsigned int vertex_shader, unsigned int fragment_shader);
  public:
    // Constructor generates the shader on the fly
    Shader(const char *vertex_path, const char *fragment_path);
    // Uses the current shader
    void Use();
    void Unuse();
    ~Shader();
};
