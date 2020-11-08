#pragma once

#include <iostream>
#include <string>

#ifdef __EMSCRIPTEN__
#define GLFW_INCLUDE_ES3
#include <emscripten/emscripten.h>
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

#include <SOIL2.h>

class Texture
{
  public:
    Texture(const char *file_name, unsigned int type);
    inline unsigned int getID() const;
    void bind(const int texture_unit);
    void unbind();
    void loadFromFile(const char *file_name);
    ~Texture();

  private:
    unsigned int id;
    int width;
    int height;
    std::string texture_type;
    unsigned int type;
};