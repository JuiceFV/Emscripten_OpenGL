#include "camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch, float speed,
               float sensivity)
    : front(glm::vec3(0.0f, 0.0f, -1.0f)), movement_speed(speed), mouse_sensitivity(sensivity), zoom(ZOOM)
{
    this->view_matrix = glm::mat4(1.f);
    this->position = position;
    this->world_up = up;
    this->yaw = yaw;
    this->pitch = pitch;
    this->updateCameraVectors();
}

Camera::Camera(float pos_x, float pos_y, float pos_z, float up_x, float up_y, float up_z, float yaw, float pitch,
               float speed, float sensivity)
    : front(glm::vec3(0.0f, 0.0f, -1.0f)), movement_speed(speed), mouse_sensitivity(SENSITIVTY), zoom(sensivity)
{
    this->view_matrix = glm::mat4(1.f);
    this->position = glm::vec3(pos_x, pos_y, pos_z);
    this->world_up = glm::vec3(up_x, up_y, up_z);
    this->yaw = yaw;
    this->pitch = pitch;
    this->updateCameraVectors();
}

// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
glm::mat4 Camera::GetViewMatrix()
{
    this->updateCameraVectors();
    this->view_matrix = glm::lookAt(this->position, this->position + this->front, this->up);
    return this->view_matrix;
}

// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera
// defined ENUM (to abstract it from windowing systems)
void Camera::ProcessKeyboard(Camera_Movement direction, float delta_time)
{
    float velocity = this->movement_speed * delta_time;
    switch (direction)
    {
    case Camera_Movement::FORWARD: this->position += this->front * velocity; break;
    case Camera_Movement::BACKWARD: this->position -= this->front * velocity; break;
    case Camera_Movement::LEFT: this->position -= this->right * velocity; break;
    case Camera_Movement::RIGHT: this->position += this->right * velocity; break;
    default: break;
    }
}

// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::ProcessMouseMovement(float x_offset, float y_offset, unsigned char constrain_pitch)
{
    x_offset *= this->mouse_sensitivity;
    y_offset *= this->mouse_sensitivity;

    this->yaw += x_offset;
    this->pitch += y_offset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrain_pitch)
    {
        if (this->pitch > 89.0f) { this->pitch = 89.0f; }

        if (this->pitch < -89.0f) { this->pitch = -89.0f; }
    }

    // Update Front, Right and Up Vectors using the updated Eular angles
    this->updateCameraVectors();
}

// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Camera::ProcessMouseScroll(float y_offset)
{
    if (this->zoom >= 1.0f && this->zoom <= 45.0f) { this->zoom -= y_offset; }

    if (this->zoom <= 1.0f) { this->zoom = 1.0f; }

    if (this->zoom >= 45.0f) { this->zoom = 45.0f; }
}

float Camera::GetZoom() const { return this->zoom; }

void Camera::updateCameraVectors()
{
    // Calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    front.y = sin(glm::radians(this->pitch));
    front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    this->front = glm::normalize(front);
    // Also re-calculate the Right and Up vector
    this->right = glm::normalize(
        glm::cross(this->front, this->world_up)); // Normalize the vectors, because their length gets closer to 0 the
                                                  // more you look up or down which results in slower movement.
    this->up = glm::normalize(glm::cross(this->right, this->front));
}

glm::vec3 Camera::GetPosition() const { return this->position; }

glm::vec3 Camera::GetFront() const { return this->front; }
