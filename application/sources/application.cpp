#include "application.h"

static float rand_float()
{
    union UNION {
        uint32_t i;
        float f;
    } r;
    // 3 because it's 0011, the first bit is the float's sign.
    // Clearing the second bit eliminates values > 1.0f.
    r.i = (rand() & 0xffff) + ((rand() & 0x3fff) << 16);
    return r.f;
}

int rand_int(int min, int max)
{
    static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
    return static_cast<int>(rand() * fraction * (max - min + 1) + min);
}

/*void Application::initGLFW()
{
    if (glfwInit() == GLFW_FALSE)
    {
        std::cerr << "An error has occured in the " << __FILE__ << " in line " << __LINE__ << "." << std::endl
                  << "Details: "
                  << "Cannot initialize GLFW" << std::endl;
        glfwTerminate();
    }
}

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

void Application::initOpenGLOptions()
{
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    if (this->MSAA) glEnable(GL_MULTISAMPLE);

    // Input
    glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Application::initMatrices()
{
    // TODO decipher if it works
    this->view.view_obj = glm::mat4(1.f);
    this->view.view_obj = glm::lookAt(this->camera.cam_position, this->camera.cam_position + this->camera.cam_front,
                                      this->camera.world_up);

    this->projection.projection_obj = glm::mat4(1.f);
    this->projection.projection_obj =
        glm::perspective(glm::radians(this->camera.camera_obj.GetZoom()),
                         static_cast<float>(this->frame_buffer_height) / this->frame_buffer_height,
                         this->projection.near_plane, this->projection.far_plane);

    this->model.model_obj = glm::mat4(1.f);
    this->model.model_obj = glm::translate(this->model.model_obj, this->model.translation);
    this->model.model_obj = glm::scale(this->model.model_obj, this->model.scaling);
    this->model.model_obj = glm::rotate(this->model.model_obj, this->model.rotation_angle, this->model.rotation_axis);
}

void Application::initShaders()
{
#ifndef __EMSCRIPTEN__
    this->shaders.push_back(new Shader("assets/shaders/lightning.vs", "assets/shaders/lightning.frag"));
#else
    this->shaders.push_back(new Shader("assets/shaders/lightning.wvs", "assets/shaders/lightning.wfrag"));
#endif
}

void Application::initTextures(const std::vector<std::string> &textures)
{
    for (auto &file_name : textures)
        this->textures.push_back(new Texture(("assets/images" + file_name).c_str(), GL_TEXTURE_2D));
}

void Application::initModels(const std::string file_name)
{
    if (file_name != "")
    {
        std::string extension = file_name.substr(file_name.find_last_of(".") + 1);
        if (extension != file_name)
        {
            if (extension == "obj" || extension == "stl")
                this->models.push_back(new Model((char *)("assets/models/" + file_name).c_str()));
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

//TODO rebuild
void Application::initPointLights(std::vector<PointLight> pls)
{
    srand(time(NULL));
    if (pls.empty())
    {
        int pl_quantity = rand_int(1, 10);
        for (unsigned int i = 0; i < pl_quantity; i++)
            this->light.point_lights.push_back(new PointLight(
                {rand_float(), rand_float(), rand_float()}, {rand_float(), rand_float(), rand_float()},
                {rand_float(), rand_float(), rand_float()}, {rand_float(), rand_float(), rand_float()}, i));
    }
    else
    {
        for (auto &pl : pls) this->light.point_lights.push_back(&pl);
    }

}

//TODO rebuild
void Application::initDirectionalLights(std::vector<DirectionalLight> dls)
{
    srand(time(NULL));
    if (dls.empty())
    {
        this->light.dir_lights.push_back(new DirectionalLight(
            {rand_float(), rand_float(), rand_float()}, {rand_float(), rand_float(), rand_float()},
            {rand_float(), rand_float(), rand_float()}, {rand_float(), rand_float(), rand_float()}));
    }
    else
    {
        for (auto &dl : dls) this->light.dir_lights.push_back(&dl);
    }
}

void Application::initLights(std::vector<PointLight> point_ligts, std::vector<DirectionalLight> directional_lights)
{
    this->initDirectionalLights(directional_lights);
    this->initPointLights(point_ligts);
}

void Application::initUniforms()
{
    this->shaders[0]->setVec3f({this->camera.camera_obj.GetPosition().x, this->camera.camera_obj.GetPosition().y,
                                this->camera.camera_obj.GetPosition().z},
                               "viewPos");
    this->shaders[0]->set1f(32.0f, "material.shininess");
    for (auto &dl : this->light.dir_lights) dl->sendToShader(*this->shaders[0]);
    for (auto &pl : this->light.point_lights) pl->sendToShader(*this->shaders[0]);

    // change the shader's enum
    this->shaders[0]->setMat4fv(this->view.view_obj, "view");
    this->shaders[0]->setMat4fv(this->projection.projection_obj, "projection");
    this->shaders[0]->setMat4fv(this->model.model_obj, "model");
}

void Application::updateUniforms()
{
    this->shaders[0]->setVec3f({this->camera.camera_obj.GetPosition().x, this->camera.camera_obj.GetPosition().y,
                                this->camera.camera_obj.GetPosition().z},
                               "viewPos");
    this->shaders[0]->set1f(32.0f, "material.shininess");

    for (auto &dl : this->light.dir_lights) dl->sendToShader(*this->shaders[0]);
    for (auto &pl : this->light.point_lights) pl->sendToShader(*this->shaders[0]);
    // Update view matrix (camera)
    this->view.view_obj = this->camera.camera_obj.GetViewMatrix();
    this->projection.projection_obj =
        glm::perspective(glm::radians(this->camera.camera_obj.GetZoom()),
                         static_cast<float>(this->frame_buffer_height) / this->frame_buffer_height,
                         this->projection.near_plane, this->projection.far_plane);

    this->shaders[0]->setMat4fv(this->view.view_obj, "view");
    this->shaders[0]->setMat4fv(this->projection.projection_obj, "projection");

    // Update framebuffer size and projection matrix
    glfwGetFramebufferSize(this->window, &this->frame_buffer_width, &this->frame_buffer_height);

    this->model.model_obj = glm::translate(
        this->model.model_obj, this->model.translation); // Translate it down a bit so it's at the center of the scene
    this->model.model_obj =
        glm::scale(this->model.model_obj, this->model.scaling); // It's a bit too big for our scene, so scale it down
    this->model.model_obj = glm::rotate(this->model.model_obj, this->model.rotation_angle, this->model.rotation_axis);

    this->shaders[0]->setMat4fv(this->model.model_obj, "model");
}

void Application::framebufferResizeCallback(GLFWwindow *window, int fb_w, int fb_h) { glViewport(0, 0, fb_w, fb_h); };

Application::Application(const char *title, const int WINDOW_WIDTH, const int WINDOW_HEIGHT, bool resizable,
                         std::vector<std::string> models, bool MSAA, glm::vec3 cam_position, glm::vec3 world_up,
                         glm::vec3 cam_front)
    : WINDOW_WIDTH(WINDOW_WIDTH), WINDOW_HEIGHT(WINDOW_HEIGHT)
{
    this->window = nullptr;
    this->frame_buffer_height = this->WINDOW_HEIGHT;
    this->frame_buffer_width = this->WINDOW_WIDTH;

    this->camera.cam_position = cam_position;
    this->camera.world_up = world_up;
    this->camera.cam_front = cam_front;
    this->camera.camera_obj = Camera(cam_position, world_up, -156.0f, -12.4f);

    this->projection.near_plane = 0.1f;
    this->projection.far_plane = 100.f;

    this->model.rotation_angle = -1.5708f;
    this->model.rotation_axis = glm::vec3(1.0f, 0.0f, 0.0f);
    this->model.scaling = glm::vec3(0.01f, 0.01f, 0.01f);
    this->model.translation = glm::vec3(-1.75f, -1.75f, 0.0f);

    this->MSAA = MSAA;

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
    this->initMatrices();
    this->initShaders();
    this->initTextures();
    this->initModels(models[0]);
    this->initLights();
    this->initUniforms();
}

Application::~Application()
{
    glfwDestroyWindow(this->window);
    glfwTerminate();

    for (auto &shader : this->shaders) delete shader;
    for (auto &texture : this->textures) delete texture;
    for (auto &_model : this->models) delete _model;
    for (auto &pl : this->light.point_lights) delete pl;
    for (auto &dl : this->light.dir_lights) delete dl;
}

int Application::getWindowShouldClose() { return glfwWindowShouldClose(this->window); }

void Application::setWindowShouldClose() { glfwSetWindowShouldClose(this->window, GLFW_TRUE); }

void Application::updateDeltaTime()
{
    this->current_frame = static_cast<float>(glfwGetTime());
    this->delta_time = this->current_frame - this->last_frame;
    this->last_frame = this->current_frame;
}

void Application::keyCallBack()
{
    if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) this->setWindowShouldClose();

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

    this->last_x = this->x_pos;
    this->last_y = this->y_pos;

    this->camera.camera_obj.ProcessMouseMovement(this->x_offset, this->y_offset);
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
    // UPDATE ---
    // updateInput(window);

    // DRAW ---
    // Clear
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Update the uniforms
    this->updateUniforms();

    // Render models
    // TODO modify shader enum
    for (auto &_model : this->models) 
        _model->Draw(*this->shaders[0]);

    // End Draw
    glfwSwapBuffers(this->window);
}*/
