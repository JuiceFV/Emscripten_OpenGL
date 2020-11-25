#pragma once

#include <glm/glm.hpp>

namespace window
{
    const char *title = "Emscripten Graphics";
    const int width = 1280;
    const int height = 720;
    bool resizable = false;
} // namespace window

namespace input
{
    // camera's movment speed
    float speed = 6.0;

    // mouse sensitivity
    float sensitivity = 0.15f;
} // namespace input

namespace graphics
{
    // the name of shader file w/o extension (default: "core")
    // (ex: if a shader file is my_shader.vs, my_shader.frag, then shader_file="my_shader"
    std::string shader_file = "";

    // antialiasing
    bool MSAA = true;

    // lines vertex to vertex (w/o fullfilling)
    bool line_mode = false;

    // essential to be false, when a shader doesn't consider light handling
    bool is_light_shader = true;
} // namespace graphics

namespace lightning
{
    // 1. dir-light direction
    // 2. dir-light ambient
    // 3. dir-light diffuse
    // 4. dir-light specular
    std::vector<DirectionalLight> directional_lights = {
        {{4.07789f, -1.60387f, -0.588997f}, {0.5f, 0.5f, 0.5f}, {0.4f, 0.4f, 0.4f}, {0.5f, 0.5f, 0.5f}}};

    // 1. point-light direction
    // 2. point-light ambient
    // 3. point-light diffuse
    // 4. point-light specular
    // 5. number of a pl (its index)
    // 6. constant (default: 1.0)
    // 7. linear (default: 0.09)
    // 8. quadratic (default: 0.032)
    std::vector<PointLight> point_lights = {
        {{-4.94625f, 15.0549f, 6.95857f}, {0.8f, 0.8f, 0.8f}, {0.8f, 0.8f, 0.8f}, {1.0f, 1.0f, 1.0f}, 0},
        {{3.96999f, -5.70809f, -13.2864f}, {0.8f, 0.8f, 0.8f}, {0.8f, 0.8f, 0.8f}, {1.0f, 1.0f, 1.0f}, 1},
        {{18.0768f, 3.03289f, 3.76213f}, {0.8f, 0.8f, 0.8f}, {0.8f, 0.8f, 0.8f}, {1.0f, 1.0f, 1.0f}, 2},
        {{-17.0855f, 7.29996f, -3.69458f}, {0.8f, 0.8f, 0.8f}, {0.8f, 0.8f, 0.8f}, {1.0f, 1.0f, 1.0f}, 3},
    };
} // namespace lightning

namespace model
{
    // The startup model, you also can remain it empty
    // Only the .obj and .stl are admissible 
    std::string startup_model = "";

    // I'd recomend to leave these vars with these values
    float model_rotation_angle_x = 0.0f;
    float model_rotation_angle_y = 0.0f;
    float model_rotation_angle_z = 0.0f;
    glm::vec3 model_scaling = {0.01f, 0.01f, 0.01f};
    glm::vec3 model_translation = {-1.75f, -1.75f, 0.0f};
} // namespace model

namespace texture
{
    // Any texture file
    std::string texture_file = "";
}

namespace camera
{
    // Also recomend leave them like that
    glm::vec3 cam_position = {0.0f, 0.0f, 0.0f};
    glm::vec3 world_up = {0.f, 1.f, 0.f};
    glm::vec3 cam_front = {-1.75f, -1.75f, 0};
} // namespace camera
