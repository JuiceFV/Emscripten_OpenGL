#pragma once

#ifdef __EMSCRIPTEN__
#define GLFW_INCLUDE_ES3
#include <emscripten/emscripten.h>
#else
#include <GL/glew.h>
#endif
#include "config.h"
#include <GLFW/glfw3.h>
#include <iostream>

class Application
{
  public:
    Application();
    Application(const cfg::Config &config);
    cfg::Config getConfig();
    void intitGlfw();
    void initWindow(std::string title, bool resizable);
#ifndef __EMSCRIPTEN__
    void initGlew();
#endif
    void initOpenGLOptions();

  private:
    cfg::Config *config;

    int GL_VERSION_MAJOR;
    int GL_VERSION_MINOR;

    int fb_width;
    int fb_height;
    GLFWwindow *window;

    static void fb_resize_callback(GLFWwindow *window, int fb_w, int fb_h);
};
