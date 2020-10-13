#include "application.h"

void Application::intitGlfw()
{
    if (glfwInit() == GLFW_FALSE)
    {
        std::cerr << "Something went wrong with GLFW initialization." << std::endl
                  << __FILE__ << "in line:" << __LINE__ << std::endl;
        glfwTerminate();
    }
}

void Application::initWindow() {
   
}