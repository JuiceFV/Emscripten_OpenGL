#include "application.h"

// I defined the camera, model, models variables as static in
// purpose to use the zoom. Due to I won't use
// more than 1 application's object - it shouldn't hurt the app
// however, apperently there appears some bottlenecks.
AppCamera Application::camera = {};
ModelMatrix Application::model = {};
Model *Application::uploaded_model = nullptr;

// I use the random float numbers to determine the light
// in case if it's not been passed.
static float rand_float(float min, float max)
{
    union UNION {
        uint32_t i;
        float f;
    } r;
    // 3 because it's 0011, the first bit is the float's sign.
    // Clearing the second bit eliminates values > 1.0f.
    r.i = (rand() & 0xffff) + ((rand() & 0x3fff) << 16);

    float diff = max - min;

    return (min + r.f * diff);
}

int rand_int(int min, int max)
{
    static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
    return static_cast<int>(rand() * fraction * (max - min + 1) + min);
}

static float calculate_yaw(float x, float y) { return std::atan2(x, y) * (180.0 / 3.141592653589793238463); }

static float calculate_pitch(float x, float y, float z)
{
    float projection_length = std::sqrt(x * x + y * y);
    return std::atan2(z, projection_length) * (180.0 / 3.141592653589793238463);
}

// Initialize GLFW
// DONE
void Application::initGLFW()
{
    if (glfwInit() == GLFW_FALSE)
    {
        std::cerr << "An error has occured in the " << __FILE__ << " in line " << __LINE__ << "." << std::endl
                  << "Details: "
                  << "Cannot initialize GLFW" << std::endl;
        glfwTerminate();
    }
}

// Determine window
// DONE
void Application::initWindow(const char *title, bool resizable)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, resizable);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    // Create a GLFWwindow object that we can use for GLFW's functions
    this->window = glfwCreateWindow(this->WINDOW_WIDTH, this->WINDOW_HEIGHT, title, nullptr, nullptr);

    if (nullptr == this->window)
    {
        std::cerr << "An error has occured in the " << __FILE__ << " in line " << __LINE__ << "." << std::endl
                  << "Details: "
                  << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }

    glfwMakeContextCurrent(this->window);

    glfwGetFramebufferSize(this->window, &this->frame_buffer_width, &frame_buffer_height);
    glfwSetFramebufferSizeCallback(this->window, Application::framebufferResizeCallback);
    glfwSetScrollCallback(this->window, Application::ScrollCallback);
}

#ifndef __EMSCRIPTEN__
void Application::initGLEW()
{
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (GLEW_OK != glewInit())
    {
        std::cerr << "An error has occured in the " << __FILE__ << " in line " << __LINE__ << "." << std::endl
                  << "Details: "
                  << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
    }
}
#endif

// DONE
void Application::framebufferResizeCallback(GLFWwindow *window, int fb_w, int fb_h) { glViewport(0, 0, fb_w, fb_h); }

// DONE
int Application::getWindowShouldClose() { return glfwWindowShouldClose(this->window); }

// DONE
void Application::setWindowShouldClose() { glfwSetWindowShouldClose(this->window, GLFW_TRUE); }

// DONE
void Application::initOpenGLOptions()
{
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#ifndef __EMSCRIPTEN__
    if (this->line_mode) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif // !__EMSCRIPTEN__

    if (this->MSAA) glEnable(GL_MULTISAMPLE);

    // Input
    glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

// DONE
void Application::initMatrices()
{
    this->view.view_obj = glm::mat4(1.f);
    this->projection.projection_obj = glm::mat4(1.f);
    this->model.model_obj = glm::mat4(1.f);
}

// DONE
void Application::initShaders(std::string shader_file)
{
    std::string filename = shader_file == "" ? "core" : shader_file;
#ifndef __EMSCRIPTEN__
    this->shader =
        new Shader(("assets/shaders/" + filename + ".vs").c_str(), ("assets/shaders/" + filename + ".frag").c_str());
#else
    this->shader =
        new Shader(("assets/shaders/" + filename + ".wvs").c_str(), ("assets/shaders/" + filename + ".wfrag").c_str());
#endif
}

// DONE
void Application::initTextures(const std::string texture)
{
    if (texture != "") this->texture = new Texture(("assets/images/" + texture).c_str(), GL_TEXTURE_2D);
}

// Initialize model via stl or obj files
// DONE
void Application::initModel(const std::string file_name)
{
    if (file_name != "")
    {
        std::string extension = file_name.substr(file_name.find_last_of(".") + 1);
        if (extension != file_name)
        {
            if (extension == "obj" || extension == "stl")
                this->uploaded_model = new Model((char *)("assets/models/" + file_name).c_str());
            else
            {
                std::cerr
                    << "An error has occured in the " << __FILE__ << " in line " << __LINE__ << "." << std::endl
                    << "Details: "
                    << "Worng model's file extension. The program able to handle only the '.obj' and '.stl' ones!"
                    << std::endl
                    << "You can fix it by recompiling the ASSIMP library at "
                       "Emscripten_OpenGL/application/dependencies/lib_sources/ASSIMP along with setting"
                    << std::endl
                    << "Importer admitted extensions: "
                       "Emscripten_OpenGL/application/dependencies/lib_sources/ASSIMP/code/excludeimporters.cmake"
                    << std::endl
                    << "Exporter admitted extensions: "
                       "Emscripten_OpenGL/application/dependencies/lib_sources/ASSIMP/code/excludeexporters.cmake";
                exit(-1);
            }
        }
        else
        {
            std::cerr << "An error has occured in the " << __FILE__ << " in line " << __LINE__ << "." << std::endl
                      << "Details: "
                      << "File has no extension!" << std::endl;
            exit(-1);
        }
    }
}

// DONE
void Application::initPointLights(std::vector<PointLight> pls)
{
    if (this->is_light_shader)
    {
        srand(time(0));

        // In the case if pointligt's positions and their intensity weren't passed
        // generate them dynamically
        if (pls.empty())
        {
            // Generate amount of lights [3;10]
            int pl_quantity = rand_int(3, 10);
            for (unsigned int i = 0; i < pl_quantity; i++)
                this->light.point_lights.push_back(
                    new PointLight({rand_float(-10.0f, 10.0f), rand_float(10.0f, 20.0f), rand_float(0.0f, 10.0f)},
                                   {rand_float(0.5f, 1.0f), rand_float(0.5f, 1.0f), rand_float(0.5f, 1.0f)},
                                   {rand_float(0.5f, 1.0f), rand_float(0.5f, 1.0f), rand_float(0.5f, 1.0f)},
                                   {rand_float(0.5f, 1.0f), rand_float(0.5f, 1.0f), rand_float(0.5f, 1.0f)}, i));
        }
        else
        {
            for (auto &pl : pls) this->light.point_lights.push_back(&pl);
        }
    }
}

// DONE
void Application::initDirectionalLights(std::vector<DirectionalLight> dls)
{
    if (this->is_light_shader)
    {
        srand(time(0));
        if (dls.empty())
        {
            this->light.dir_lights.push_back(new DirectionalLight({4.07789f, -1.60387f, -0.588997f}, {0.5f, 0.5f, 0.5f},
                                                                  {0.4f, 0.4f, 0.4f}, {0.5f, 0.5f, 0.5f}));
        }
        else
        {
            for (auto &dl : dls) this->light.dir_lights.push_back(&dl);
        }
    }
}

void Application::initLights(std::vector<PointLight> point_ligts, std::vector<DirectionalLight> directional_lights)
{
    if (this->is_light_shader)
    {
        this->initDirectionalLights(directional_lights);
        this->initPointLights(point_ligts);
    }
}

void Application::initUniforms()
{
    this->shader->setVec3f({this->camera.camera_obj.GetPosition().x, this->camera.camera_obj.GetPosition().y,
                             this->camera.camera_obj.GetPosition().z},
                            "viewPos");

    if (this->is_light_shader)
    {
        this->shader->set1f(32.0f, "material.shininess");
        for (auto &dl : this->light.dir_lights) dl->sendToShader(*this->shader);
        for (auto &pl : this->light.point_lights) pl->sendToShader(*this->shader);
    }

    // change the shader's enum
    this->shader->setMat4fv(this->view.view_obj, "view");
    this->shader->setMat4fv(this->projection.projection_obj, "projection");
    this->shader->setMat4fv(this->model.model_obj, "model");
}

void Application::updateUniforms()
{
    this->initMatrices();

    // Update view matrix (camera)
    this->view.view_obj = this->camera.camera_obj.GetViewMatrix();

    this->projection.projection_obj =
        glm::perspective(glm::radians(this->camera.camera_obj.GetZoom()),
                         static_cast<float>(this->frame_buffer_width) / this->frame_buffer_height,
                         this->projection.near_plane, this->projection.far_plane);

    // Update framebuffer size and projection matrix
    glfwGetFramebufferSize(this->window, &this->frame_buffer_width, &this->frame_buffer_height);

    // Translate it down a bit so it's at the center of the scene
    this->model.model_obj = glm::translate(this->model.model_obj, this->model.translation);
    // It's a bit too big for our scene, so scale it down
    this->model.model_obj = glm::scale(this->model.model_obj, this->model.scaling);
    this->rotateModel();
    this->initUniforms();
}

Application::Application(const char *title, const int WINDOW_WIDTH, const int WINDOW_HEIGHT, bool resizable,
                         std::string shader_file, std::string model, std::string texture,
                         std::vector<DirectionalLight> dls, std::vector<PointLight> pls, bool MSAA, bool line_mode,
                         bool is_light_shader, float model_rotation_angle_x, float model_rotation_angle_y,
                         float model_rotation_angle_z, glm::vec3 model_scaling, glm::vec3 model_translation,
                         float speed, float sensitivity, glm::vec3 cam_position, glm::vec3 world_up,
                         glm::vec3 cam_front)
    : WINDOW_WIDTH(WINDOW_WIDTH), WINDOW_HEIGHT(WINDOW_HEIGHT)
{
    this->window = nullptr;

    this->sensitivity = sensitivity;
    this->speed = speed;

    this->camera.cam_position = cam_position;
    this->camera.world_up = world_up;
    this->camera.cam_front = cam_front;

    this->camera.camera_obj = Camera(cam_position, world_up, calculate_yaw(cam_front.x, cam_front.y),
                                     calculate_pitch(cam_front.x, cam_front.y, cam_front.z), speed, sensitivity);

    this->projection.near_plane = 0.1f;
    this->projection.far_plane = 100.f;

    this->model.rotation_angle_x = model_rotation_angle_x;
    this->model.rotation_angle_y = model_rotation_angle_y;
    this->model.rotation_angle_z = model_rotation_angle_z;
    this->model.scaling = model_scaling;
    this->model.translation = model_translation;

    this->MSAA = MSAA;
    this->line_mode = line_mode;
    this->is_light_shader = is_light_shader;

    this->delta_time = 0.f;
    this->current_frame = 0.f;
    this->last_frame = 0.f;

    this->last_x = this->WINDOW_WIDTH / 2.0;
    this->last_y = this->WINDOW_HEIGHT / 2.0;
    this->first_mouse = true;

    this->initGLFW();
    this->initWindow(title, resizable);
#ifndef __EMSCRIPTEN__
    this->initGLEW();
#endif // ! __EMSCRIPTEN__
    this->initOpenGLOptions();
    this->initShaders(shader_file);
    this->initTextures(texture);
    this->initModel(model);
    this->initLights(pls, dls);
}

Application::~Application()
{
    glfwDestroyWindow(this->window);
    glfwTerminate();

    if (this->shader) delete this->shader;
    if (this->texture) delete this->texture;
    if (this->uploaded_model) delete this->uploaded_model;
    for (auto &pl : this->light.point_lights) delete pl;
    for (auto &dl : this->light.dir_lights) delete dl;
}

// DONE
void Application::updateDeltaTime()
{
    this->current_frame = static_cast<float>(glfwGetTime());
    this->delta_time = this->current_frame - this->last_frame;
    this->last_frame = this->current_frame;
}

void Application::keyCallBack()
{
    if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) this->setWindowShouldClose();

#ifndef __EMSCRIPTEN__
    if (glfwGetKey(this->window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS &&
        glfwGetKey(this->window, GLFW_KEY_O) == GLFW_PRESS)
    {
        auto path = FileDialog::Open();
        this->uploadModel((path ? path : ""));
    }
#endif
    if (glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(this->window, GLFW_KEY_UP) == GLFW_PRESS)
        this->camera.camera_obj.ProcessKeyboard(Camera_Movement::FORWARD, this->delta_time);

    if (glfwGetKey(this->window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(this->window, GLFW_KEY_DOWN) == GLFW_PRESS)
        this->camera.camera_obj.ProcessKeyboard(Camera_Movement::BACKWARD, this->delta_time);

    if (glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(this->window, GLFW_KEY_LEFT) == GLFW_PRESS)
        this->camera.camera_obj.ProcessKeyboard(Camera_Movement::LEFT, this->delta_time);

    if (glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(this->window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        this->camera.camera_obj.ProcessKeyboard(Camera_Movement::RIGHT, this->delta_time);
}

void Application::mouseCallback()
{
    glfwGetCursorPos(this->window, &this->x_pos, &this->y_pos);

    if (this->first_mouse)
    {
        this->last_x = this->x_pos;
        this->last_y = this->y_pos;
        this->first_mouse = false;
    }

    this->x_offset = this->x_pos - this->last_x;
    this->y_offset = this->last_y - this->y_pos; // Reversed since y-coordinates go from bottom to left

    if (glfwGetKey(this->window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        if (glfwGetMouseButton(this->window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
        {

            this->model.rotation_angle_z += this->x_offset * 6.0f * this->delta_time;
            this->model.rotation_angle_y += (this->y_pos - this->last_y) * 6.0f * this->delta_time;
        }
    }
    else
        this->camera.camera_obj.ProcessMouseMovement(this->x_offset, this->y_offset);

    this->last_x = this->x_pos;
    this->last_y = this->y_pos;
}

void Application::ScrollCallback(GLFWwindow *window, double x_offset, double y_offset)
{

#if defined(__EMSCRIPTEN__)
    y_offset = -y_offset / 100;
#endif
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        model.rotation_angle_x += y_offset * 6.0f;
    else
        camera.camera_obj.ProcessMouseScroll(y_offset);
}

void Application::updateInput()
{
    glfwPollEvents();
    this->keyCallBack();
    this->mouseCallback();
}

void Application::update()
{
    // UPDATE INPUT ---
    this->updateDeltaTime();
    this->updateInput();
}

void Application::render()
{
    // DRAW ---
    // Clear
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Update the uniforms
    this->updateUniforms();

    // Render models
    if (this->uploaded_model) this->uploaded_model->Draw(*this->shader);
    // End Draw
    glfwSwapBuffers(this->window);
}

void Application::rotateModel()
{
    this->model.model_obj =
        glm::rotate(this->model.model_obj, glm::radians(this->model.rotation_angle_x), glm::vec3(1.0f, 0.0f, 0.0f));
    this->model.model_obj =
        glm::rotate(this->model.model_obj, glm::radians(this->model.rotation_angle_y), glm::vec3(0.0f, 1.0f, 0.0f));
    this->model.model_obj =
        glm::rotate(this->model.model_obj, glm::radians(this->model.rotation_angle_z), glm::vec3(0.0f, 0.0f, 1.0f));
}

void Application::uploadModel(std::string path)
{
    if (path != "")
    {
        if (uploaded_model) delete uploaded_model;
        uploaded_model = new Model((char *)path.c_str());
    }
}
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_BINDINGS(application_ex)
{
    class_<Application>("Application").class_function("uploadModel", &Application::uploadModel);
}
#endif
