#pragma once

#ifdef __EMSCRIPTEN__
#define GLFW_INCLUDE_ES3
#include <emscripten/bind.h>
#include <emscripten/emscripten.h>
using namespace emscripten;
#else
#include "filedialog.h"
#include <GL/glew.h>
#endif
#include "camera.h"
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
    float rotation_angle_x;
    float rotation_angle_y;
    float rotation_angle_z;
} ModelMatrix;

typedef struct Lights
{
    std::vector<PointLight *> point_lights;
    std::vector<DirectionalLight *> dir_lights;
};

class Application
{
  public:
    Application(const char *title, const int WINDOW_WIDTH, const int WINDOW_HEIGHT, bool resizable,
                const std::string &shader_file, const std::string &model, const std::string &texture,
                std::vector<DirectionalLight> &dls, std::vector<PointLight> &pls, const bool &MSAA,
                const bool &line_mode, const bool &is_light_shader, const float &model_rotation_angle_x,
                float &model_rotation_angle_y, float &model_rotation_angle_z, glm::vec3 &model_scaling,
                glm::vec3 &model_translation, const float &speed, const float &sensitivity, glm::vec3 &cam_position,
                glm::vec3 &world_up, glm::vec3 &cam_front);
    int getWindowShouldClose();
    void setWindowShouldClose();
    void updateDeltaTime();
    void updateInput();
    void update();
    void render();
    static void uploadModel(std::string path);
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

    bool line_mode;

    static AppCamera camera;
    float sensitivity;
    float speed;

    // Pointer to the shader
    Shader *shader;
    bool is_light_shader;

    // Texture
    Texture *texture;

    // Model
    static Model *uploaded_model;

    // Light
    Lights light;

    // Matricies
    ViewMatrix view;
    ProjectionMatrix projection;
    static ModelMatrix model;

    void initGLFW();
    void initWindow(const char *title, bool resizable);
    void initOpenGLOptions();
    void initMatrices();
    void initShaders(std::string shader_file = "");
    void initTextures(const std::string texture = "");
    void initModel(const std::string file_name = "");
    void initPointLights(std::vector<PointLight> &pls);
    void initDirectionalLights(std::vector<DirectionalLight> &dls);
    void initLights(std::vector<PointLight> &point_ligts, std::vector<DirectionalLight> &directional_lights);
    void initUniforms();
    void updateUniforms();
    void keyCallBack();
    void mouseCallback();
    void rotateModel();

    static void framebufferResizeCallback(GLFWwindow *window, int fb_w, int fb_h);
    static void ScrollCallback(GLFWwindow *window, double x_offset, double y_offset);
#ifndef __EMSCRIPTEN__
    void initGLEW();
#endif
};
