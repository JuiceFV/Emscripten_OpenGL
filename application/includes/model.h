#pragma once

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#ifdef __EMSCRIPTEN__
#define GLFW_INCLUDE_ES3
#include <emscripten/emscripten.h>
#else
#include <GL/glew.h>
#endif

#include <SOIL2.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "mesh.h"

class Model
{
  public:
    Model(char *path);
    void Draw(Shader &shader);
    void loadModel(const std::string &path);

  private:
    std::vector<Mesh> meshes;
    std::string directory;
    // textures_loaded - Stores all the textures loaded so far, optimization to make sure textures
    // aren't loaded more than once.
    std::vector<Texture> textures_loaded;

    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string type_name);
};
