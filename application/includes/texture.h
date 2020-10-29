#pragma once

#include <iostream>
#include <string>

#include <gl/glew.h>
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
    unsigned int type;
};