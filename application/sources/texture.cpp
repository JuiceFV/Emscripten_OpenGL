#include "texture.h"

Texture::Texture(const char *file_name, unsigned int type)
{
    this->type = type;

    unsigned char *image = SOIL_load_image(file_name, &this->width, &this->height, NULL, SOIL_LOAD_RGBA);

    glGenTextures(1, &this->id);
    glBindTexture(type, this->id);

    // Set our texture parameters
    glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering
    glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    if (image)
    {
        glTexImage2D(type, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(type);
    }
    else
    {
        std::cout << "ERROR::TEXTURE::TEXTURE_LOADING_FAILED: " << file_name << "\n";
    }

    glActiveTexture(0);
    glBindTexture(type, 0);
    SOIL_free_image_data(image);
}

inline unsigned int Texture::getID() const { return this->id; }

void Texture::bind(const int texture_unit)
{
    glActiveTexture(GL_TEXTURE0 + texture_unit);
    glBindTexture(this->type, this->id);
}

void Texture::unbind()
{
    glActiveTexture(0);
    glBindTexture(this->type, 0);
}

void Texture::loadFromFile(const char *file_name)
{
    if (this->id) { glDeleteTextures(1, &this->id); }

    unsigned char *image = SOIL_load_image(file_name, &this->width, &this->height, NULL, SOIL_LOAD_RGBA);

    glGenTextures(1, &this->id);
    glBindTexture(this->type, this->id);

    glTexParameteri(this->type, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(this->type, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(this->type, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(this->type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    if (image)
    {
        glTexImage2D(this->type, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(this->type);
    }
    else
    {
        std::cerr << "An error has occured in the " << __FILE__ << " in line " << __LINE__ << "." << std::endl
                  << "Error occured in the: " << file_name << std::endl;
    }

    glActiveTexture(0);
    glBindTexture(this->type, 0);
    SOIL_free_image_data(image);
}

Texture::~Texture() { glDeleteTextures(1, &this->id); }
