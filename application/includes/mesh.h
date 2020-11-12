#pragma once

#include "shader.h"
#include "vertex.h"
#include <string>
#include <vector>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.h"
#include <assimp/IOStream.hpp>

class Mesh
{
  public:
    // Constructor
    Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices,
         const std::vector<Texture> &textures);

    // Render the mesh
    void Draw(Shader &shader);

  private:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    unsigned int VAO, VBO, EBO;

    // Initializes all the buffer objects/arrays
    void setupMesh();
};
