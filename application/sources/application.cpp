#include "application.h"

Application::Application() { this->config = new cfg::Config(); }

Application::Application(const cfg::Config &config) { (*this->config) = config; }

cfg::Config Application::getConfig() { return (*this->config); }

void Application::intitGlfw()
{
    if (glfwInit() == GLFW_FALSE)
    {
        std::cerr << "Something went wrong with GLFW initialization." << std::endl
                  << __FILE__ << "in line:" << __LINE__ << std::endl;
        glfwTerminate();
    }
}

void Application::initWindow(std::string title, bool resizable)
{
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, this->GL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, this->GL_VERSION_MINOR);
    glfwWindowHint(GLFW_RESIZABLE, resizable);

    this->window = glfwCreateWindow(this->config->getWindowProp<unsigned int>(cfg::defaults::WIN_PROPS::WIDTH),
                                    this->config->getWindowProp<unsigned int>(cfg::defaults::WIN_PROPS::HEIGHT),
                                    title.c_str(), NULL, NULL);
    if (this->window == nullptr)
    {
        std::cout << "ERROR::GLFW_WINDOW_INIT_FAILED"
                  << "\n";
        glfwTerminate();
    }

    glfwGetFramebufferSize(this->window, &this->fb_width, &this->fb_height);
    glfwSetFramebufferSizeCallback(window, Application::fb_resize_callback);
    // IMPORTANT WHITH PERSPECTIVE MATRIX!!!

    // glViewport(0, 0, framebufferWidth, framebufferHeight);

    glfwMakeContextCurrent(this->window); // IMPORTANT!!
}

#ifndef __EMSCRIPTEN__
void Application::initGlew() {
    // INIT GLEW (NEEDS WINDOW AND OPENGL CONTEXT)
    glewExperimental = GL_TRUE;

    // Error
    if (glewInit() != GLEW_OK)
    {
        std::cout << "ERROR::MAIN.CPP::GLEW_INIT_FAILED"
                  << "\n";
        glfwTerminate();
    }
}
#endif

void Application::initOpenGLOptions()
{
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Input
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Application::fb_resize_callback(GLFWwindow *window, int fb_w, int fb_h) { glViewport(0, 0, fb_w, fb_h); };
