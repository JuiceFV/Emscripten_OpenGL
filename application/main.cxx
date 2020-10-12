// emcc application/main.cxx -I"application/dependencies/yamlprsr/include" -l"application/dependencies/build/libyaml-cpp.so" -o index.html -s USE_WEBGL2=1 -s USE_GLFW=3 -s WASM=1 -std=c++1z
#include <functional>
#ifdef __EMSCRIPTEN__
    #define GLFW_INCLUDE_ES3
    #include <emscripten/emscripten.h>
#else
    #include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include <iostream>
#include "application.h"

std::function<void()> loop;
void main_loop() { loop(); }

int main(void)
{

    return 0;
}