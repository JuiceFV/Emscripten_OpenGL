#include "shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

class Light
{
  protected:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

  public:
    Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
    ~Light();

    // Functions
    virtual void sendToShader(Shader &program) = 0;
};

class PointLight : public Light
{
  protected:
    glm::vec3 position;
    float constant;
    float linear;
    float quadratic;
    unsigned int num;

  public:
    PointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, unsigned int num,
               float constant = 1.0f, float linear = 0.09f, float quadratic = 0.032f);

    ~PointLight();

    void setPosition(const glm::vec3 position);

    void sendToShader(Shader &program);
};

class DirectionalLight : public Light
{
  protected:
    glm::vec3 direction;

  public:
    DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

    ~DirectionalLight();

    void setDirection(const glm::vec3 direction);

    void sendToShader(Shader &program);
};