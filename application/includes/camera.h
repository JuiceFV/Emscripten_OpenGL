#pragma once

#ifdef __EMSCRIPTEN__
#define GLFW_INCLUDE_ES3
#include <emscripten/emscripten.h>
#else
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include <iostream>

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific
// input methods
enum class Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
constexpr float YAW = -90.0f;
constexpr float PITCH = 0.0f;
constexpr float SPEED = 6.0f;
constexpr float SENSITIVTY = 0.15f;
constexpr float ZOOM = 45.0f;

// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for
// use in OpenGL
class Camera
{
  public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
           float yaw = YAW, float pitch = PITCH);
    Camera(float pos_x, float pos_y, float pos_z, float up_x, float up_y, float up_z, float yaw, float pitch);
    glm::mat4 GetViewMatrix();
    void ProcessKeyboard(Camera_Movement direction, float delta_time);
    void ProcessMouseMovement(float x_offset, float y_offset, unsigned char constrain_pitch = true);
    void ProcessMouseScroll(float y_offset);
    float GetZoom() const;
    glm::vec3 GetPosition() const;
    glm::vec3 GetFront() const;

  private:
    glm::mat4 view_matrix;

    // Camera Attributes
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 world_up;

    // Eular Angles
    float yaw;
    float pitch;
    float roll;

    // Camera options
    float movement_speed;
    float mouse_sensitivity;
    float zoom;

    // Calculates the front vector from the Camera's (updated) Eular Angles
    void updateCameraVectors();
};
