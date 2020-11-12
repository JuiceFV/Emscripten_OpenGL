#pragma once
#include <assimp/IOStream.hpp>
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
    Texture(const char *file_name, unsigned int type, std::string tex_type = "", std::string directory = "");
    unsigned int getID() const;
    void bind(const int texture_unit);
    void unbind();
    void loadFromFile(const char *file_name, std::string tex_type = "", std::string directory = "");
    std::string getTexType() const;
    aiString getPath() const;
    inline void setTexType(const std::string &tex_type);
    inline void setPath(const std::string &path);
    ~Texture();

  private:
    unsigned int id;
    int width;
    int height;
    std::string texture_type;
    aiString path;
    unsigned int type;
};