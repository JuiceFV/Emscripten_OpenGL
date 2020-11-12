#include <functional>

#ifdef __EMSCRIPTEN__
#define GLFW_INCLUDE_ES3
#include <emscripten/emscripten.h>
#else
#include <GL/glew.h>
#endif

#include "camera.h"
#include "shader.h"
#include "texture.h"
#include "model.h"
#include "light.h"
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

const unsigned int WIDTH = 1280, HEIGHT = 720;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void ScrollCallback(GLFWwindow *window, double xOffset, double yOffset);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();

// Camera
Camera camera(glm::vec3(26.0704f, 8.20109f, 17.1831f), glm::vec3(0.0f, 1.0f, 0.0f), -156.0f, -12.4f);
bool keys[1024];
float lastX = WIDTH / 2.0;
float lastY = HEIGHT / 2.0;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
    // Init GLFW
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Airplane Representation", nullptr, nullptr);

    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    // Set the required callback functions
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);
    glfwSetScrollCallback(window, ScrollCallback);

    // GLFW Options
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


#ifndef __EMSCRIPTEN__
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }
#endif
    // Define the viewport dimensions
    // Define the viewport dimensions
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // OpenGL options
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_MULTISAMPLE);  

    // 1) aloft the left wing
    // 2) under the right wing
    // 3) in front of the head (shifted left slightly)
    // 4) in tail of (shifted right slightly)
    glm::vec3 point_light_positions[] = {
        glm::vec3(-4.94625f, 15.0549f, 6.95857f), glm::vec3(3.96999f, -5.70809f, -13.2864f),
        glm::vec3(18.0768f, 3.03289f, 3.76213f), glm::vec3(-17.0855f, 7.29996f, -3.69458f)};

#ifndef __EMSCRIPTEN__
    Shader shader("assets/shaders/lightning.vs", "assets/shaders/lightning.frag");
#else
    Shader shader("assets/shaders/lightning.wvs", "assets/shaders/lightning.wfrag");
#endif
    Model depict_model("assets/models/11803_Airplane_v1_l1.obj");

    // Draw in wireframe
    // glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    DirectionalLight dir_light({4.07789f, -1.60387f, -0.588997f}, {0.5f, 0.5f, 0.5f}, {0.4f, 0.4f, 0.4f},
                               {0.5f, 0.5f, 0.5f});
    std::vector<PointLight> point_lights;
    for (unsigned int i = 0; i < 4; i++)
    {
        point_lights.push_back({{point_light_positions[i].x, point_light_positions[i].y, point_light_positions[i].z},
                                {0.8f, 0.8f, 0.8f},
                                {0.8f, 0.8f, 0.8f},
                                {1.0f, 1.0f, 1.0f},
                                i});
    }
#ifdef __EMSCRIPTEN__
    std::function<void()> mainLoop = [&]() {
#else
    while (!glfwWindowShouldClose(window))
    {
#endif

        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        DoMovement();

        // Clear the colorbuffer
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Use cooresponding shader when setting uniforms/drawing objects

        shader.setVec3f({camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z}, "viewPos");
        shader.set1f(32.0f, "material.shininess");
        // Directional light
        dir_light.sendToShader(shader);
        
        // Point lights 
        for (auto &pl : point_lights) pl.sendToShader(shader);
        glm::mat4 view(1.f);
        view = camera.GetViewMatrix();

        glm::mat4 projection(1.f);
        projection =
            glm::perspective(glm::radians(camera.GetZoom()), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);


        // Pass the matrices to the shader
        shader.setMat4fv(view, "view");
        shader.setMat4fv(projection, "projection");

        glm::mat4 model(1);
        model = glm::translate(
            model, glm::vec3(-1.75f, -1.75f, 0.0f)); // Translate it down a bit so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f)); // It's a bit too big for our scene, so scale it down
        model = glm::rotate(model, -1.5708f, glm::vec3(1.0f, 0.0f, 0.0f));
        shader.setMat4fv(model, "model");
        depict_model.Draw(shader);
        // Swap the buffers
        glfwSwapBuffers(window);
#ifdef __EMSCRIPTEN__
    };
    emscripten_set_main_loop_arg(dispatch_main, &mainLoop, 0, 1);
#else
    }
#endif
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();

    return EXIT_SUCCESS;
}
// Moves/alters the camera positions based on user input
void DoMovement()
{
    // Camera controls
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP]) { camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime); }

    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN]) { camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime); }

    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]) { camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime); }

    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) { camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime); }
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) { glfwSetWindowShouldClose(window, GL_TRUE); }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS) { keys[key] = true; }
        else if (action == GLFW_RELEASE)
        {
            keys[key] = false;
        }
    }
}

void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos; // Reversed since y-coordinates go from bottom to left

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}

void ScrollCallback(GLFWwindow *window, double xOffset, double yOffset) { camera.ProcessMouseScroll(yOffset); }
