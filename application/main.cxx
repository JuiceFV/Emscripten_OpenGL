#include <functional>

#ifdef __EMSCRIPTEN__
#define GLFW_INCLUDE_ES3
#include <emscripten/emscripten.h>
#else
#include <GL/glew.h>
#endif

#include "application.h"
#include <GLFW/glfw3.h>
#include <SOIL2.h>
#include <functional>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifdef __EMSCRIPTEN__
static void dispatch_main(void *fp)
{
    std::function<void()> *func = (std::function<void()> *)fp;
    (*func)();
}
#endif

int main()
{
    Application app("test", 1280, 720, true, "", {"11803_Airplane_v1_l1.obj"});
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