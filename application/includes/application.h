#pragma once

#ifdef __EMSCRIPTEN__
#define GLFW_INCLUDE_ES3
#include <emscripten/emscripten.h>
#else
#include <GL/glew.h>
#endif
#include "camera.h"
#include "config.h"
#include "light.h"
#include "model.h"
#include <GLFW/glfw3.h>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <iostream>

typedef struct AppCamera
{
    Camera camera_obj;
    glm::vec3 cam_position;
    glm::vec3 world_up;
    glm::vec3 cam_front;
} AppCamera;

typedef struct ViewMatrix
{
    glm::mat4 view_obj;
} ViewMatrix;

typedef struct ProjectionMatrix
{
    glm::mat4 projection_obj;
    float near_plane;
    float far_plane;
} ProjectionMatrix;

typedef struct ModelMatrix
{
    glm::mat4 model_obj;
    glm::vec3 translation;
    glm::vec3 scaling;
    float rotation_angle;
    glm::vec3 rotation_axis;
} ModelMatrix;

typedef struct Lights
{
    std::vector<PointLight *> point_lights;
    std::vector<DirectionalLight *> dir_lights;
};

/*class Application
{
  public:
    Application(const char *title, const int WINDOW_WIDTH, const int WINDOW_HEIGHT, bool resizable,
                std::vector<std::string> models = {""}, bool MSAA = true,
                glm::vec3 cam_position = {0.f, 0.f, 1.f}, glm::vec3 world_up = {0.f, 1.f, 0.f},
                glm::vec3 cam_front = {0.f, 0.f, -1.f});
    int getWindowShouldClose();
    void setWindowShouldClose();
    void updateDeltaTime();
    void updateInput();
    void update();
    void render();
    ~Application();

  private:
    GLFWwindow *window;
    int WINDOW_WIDTH;
    int WINDOW_HEIGHT;
    int frame_buffer_width;
    int frame_buffer_height;
    bool MSAA;

    float delta_time;
    float current_frame;
    float last_frame;

    double last_x;
    double last_y;
    double x_pos;
    double y_pos;
    double x_offset;
    double y_offset;
    bool first_mouse;

    bool keys[1024];

    AppCamera camera;

    std::vector<Shader *> shaders;

    // Textures
    std::vector<Texture *> textures;

    // Models
    std::vector<Model *> models;

    // Lights
    Lights light;

    // Matricies
    ViewMatrix view;
    ProjectionMatrix projection;
    ModelMatrix model;

    void initGLFW();
    void initWindow(const char *title, bool resizable);
    void initOpenGLOptions();
    void initMatrices();
    // TODO add up the shader's file selection
    void initShaders();
    void initTextures(const std::vector<std::string> &textures = {});
    void initModels(const std::string file_name = "");
    void initPointLights(std::vector<PointLight> pls);
    void initDirectionalLights(std::vector<DirectionalLight> dls);
    void initLights(std::vector<PointLight> point_ligts = {}, std::vector<DirectionalLight> directional_lights = {});
    void initUniforms();
    void updateUniforms();
    void keyCallBack();
    void mouseCallback();

    static void framebufferResizeCallback(GLFWwindow *window, int fb_w, int fb_h);
#ifndef __EMSCRIPTEN__
    void initGLEW();
#endif
};
*/
