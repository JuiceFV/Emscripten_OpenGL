#include "application.h"
#include "globals.h"
#include <iostream>

// Emscripten uses its own endless loop.
// In purpose to properly execute it we create
// purposed function, which dispatches the loop
#ifdef __EMSCRIPTEN__
static void dispatch_main(void *fp)
{
    std::function<void()> *func = (std::function<void()> *)fp;
    (*func)();
}
#endif

int main()
{
    Application app(window::title, window::width, window::height, window::resizable, graphics::shader_file,
                    model::startup_model, texture::texture_file, lightning::directional_lights, lightning::point_lights,
                    graphics::MSAA, graphics::line_mode, graphics::is_light_shader, model::model_rotation_angle_x,
                    model::model_rotation_angle_y, model::model_rotation_angle_z, model::model_scaling,
                    model::model_translation, input::speed, input::sensitivity, camera::cam_position, camera::world_up,
                    camera::cam_front);
#ifdef __EMSCRIPTEN__
    std::function<void()> mainLoop = [&]() {
#else
    while (!app.getWindowShouldClose())
    {
#endif
        app.update();
        app.render();
#ifdef __EMSCRIPTEN__
    };
    emscripten_set_main_loop_arg(dispatch_main, &mainLoop, 0, 1);
#else
    }
#endif
}
