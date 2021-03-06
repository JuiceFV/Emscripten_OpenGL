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

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
    void set1i(int value, const char *name);
    void set1f(float value, const char *name);
    void setVec2f(glm::fvec2 value, const char *name);
    void setVec3f(glm::fvec3 value, const char *name);
    void setVec4f(glm::fvec4 value, const char *name);
    void setMat3fv(glm::mat3 value, const char *name, unsigned char transpose = GL_FALSE);
    void setMat4fv(glm::mat4 value, const char *name, unsigned char transpose = GL_FALSE);
    ~Shader();
};
