#include "application.h"
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
    Application app("test", 1280, 720, true, "", "11803_Airplane_v1_l1.obj");
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