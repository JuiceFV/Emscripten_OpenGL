#include "light.h"

Light::Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
{
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
}

Light::~Light() {}

PointLight::PointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, unsigned int num,
                       float constant, float linear, float quadratic)
    : Light(ambient, diffuse, specular)
{
    this->position = position;
    this->constant = constant;
    this->linear = linear;
    this->quadratic = quadratic;
    this->num = num;
}

void PointLight::setPosition(const glm::vec3 position) { this->position = position; }

PointLight::~PointLight() {}

void PointLight::sendToShader(Shader &program)
{
    program.setVec3f(this->position, ("pointLights[" + std::to_string(num) + "].position").c_str());
    program.setVec3f(this->ambient, ("pointLights[" + std::to_string(num) + "].ambient").c_str());
    program.setVec3f(this->diffuse, ("pointLights[" + std::to_string(num) + "].diffuse").c_str());
    program.setVec3f(this->specular, ("pointLights[" + std::to_string(num) + "].specular").c_str());
    program.set1f(this->constant, ("pointLights[" + std::to_string(num) + "].constant").c_str());
    program.set1f(this->linear, ("pointLights[" + std::to_string(num) + "].linear").c_str());
    program.set1f(this->quadratic, ("pointLights[" + std::to_string(num) + "].quadratic").c_str());
}

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
    : Light(ambient, diffuse, specular)
{
    this->direction = direction;
}

DirectionalLight::~DirectionalLight() {}

void DirectionalLight::setDirection(const glm::vec3 direction) { this->direction = direction; }

void DirectionalLight::sendToShader(Shader &program)
{
    program.setVec3f(this->direction, "dirLight.direction");
    program.setVec3f(this->ambient, "dirLight.ambient");
    program.setVec3f(this->diffuse, "dirLight.diffuse");
    program.setVec3f(this->specular, "dirLight.specular");
}