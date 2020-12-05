# Emscripten_OpenGL

This project is the example of usage of the [Emscripten](https://emscripten.org). The application itself is the 3D-Model Render, based on OpenGL. I would like to turn some constraints:
- If you're working on Windows, you have several options to build. In case you have [Docker](https://www.docker.com) you can build it in few steps. Otherwise you can build for the Visual Studio 2019 or for the emscripten. In case if you chose the emscripten, you're required to use the [WSL2](https://en.wikipedia.org/wiki/Windows_Subsystem_for_Linux). 
- If you working on Linux, you may use only emscripten compiler in both ways via Docker or rawly via Cmake.
- Also all models, shaders and images should be pre-loaded in `assets/models`, `assets/shaders` or `assets/images`, respectively.

All installation's instructions you can read at [Installation section](#installation)
 
- [Emscripten_OpenGL](#emscripten_opengl)
  - [Installation requirements](#installation-requirements)
  - [Installation](#installation)
    - [Assets preparation](#assets-preparation)
      - [Shaders](#shaders)
      - [Models](#models)
      - [Textures](#textures)
    - [Application's Configuration](#applications-configuration)
    - [Build](#build)
  - [Usage](#usage)
    - [Emscripten](#emscripten)
    - [Visual Studio 2019](#visual-studio-2019)
  - [Rebuild](#rebuild)
  - [TODO](#todo)

## Installation requirements

Before installation you have to be familiar with some pre-installation requrements.

<details>
<summary>Docker</summary>
Docker is enough.
</details>

<details>
<summary>Emscripten</summary>
<ul>
<li>If you work on Uinux, then follow steps below. Otherwise, install WSL2 and do the same steps,</li>
<li>First of all you have to have emscripten, installation's instuctions are <a href="https://emscripten.org/docs/getting_started/downloads.html">here</a></li>
<li>Secondly - <a href="https://cmake.org/download">CMake</a> (I use 3.18.3)</li>
<li>Thirdly - Python3. (I use python 3.8.2)</li>
</ul>
</details>

<details>
<summary>Visual Studio 2019</summary>
<ul>
<li><a href="https://cmake.org/download">CMake</a> is enough (I use 3.18.3)</li>
</ul>
</details>

## Installation

Be sure you are familiar with [installation requirements](#installation-requirements)

### Assets preparation

Before compile the code, be sure you passed everything into right directories. 

#### Shaders

The application handles fragment and vertex shaders for both WebGL and OpenGL. All shaders are placed in `assets/shaders`:
* Vertex shader for the OpenGL has `.vs` extension.
* Fragment shader for the OpenGL has `.frag` extension.
* Vertex shader for the WebGL has `.wvs` extension.
* Fragment shader for the WebGL has `.wfrag` extension.


![image](https://user-images.githubusercontent.com/35202460/101246753-52a8a100-3726-11eb-83d7-3f0c3767daed.png)

#### Models

The application able to process only the `.obj` and `.stl` models. Before building the code, pass the models with its entities and relates into the `assets/models`.

![image](https://user-images.githubusercontent.com/35202460/101246698-01001680-3726-11eb-8ea1-e8c03ebc5f3d.png)

#### Textures
The application handles any extension admitted by [SOIL2](https://github.com/SpartanJ/SOIL2). All textures are placed in `assets/images`
![image](https://user-images.githubusercontent.com/35202460/101246928-686a9600-3727-11eb-8560-3e990af5478c.png)

### Application's Configuration

Before we start, let's familiarize with configuration
```cpp
namespace window
{
    const char *title = "Emscripten Graphics";
    const int width = 1280;
    const int height = 720;
    bool resizable = false;
} // namespace window

namespace input
{
    // camera's movment speed
    float speed = 6.0;

    // mouse sensitivity
    float sensitivity = 0.15f;
} // namespace input

namespace graphics
{
    // the name of shader file w/o extension (default: "core")
    // (ex: if a shader file is my_shader.vs, my_shader.frag, then shader_file="my_shader"
    std::string shader_file = "";

    // antialiasing
    bool MSAA = true;

    // lines vertex to vertex (w/o fullfilling)
    bool line_mode = false;

    // essential to be false, when a shader doesn't consider light handling
    bool is_light_shader = true;
} // namespace graphics

namespace lightning
{
    // 1. dir-light direction
    // 2. dir-light ambient
    // 3. dir-light diffuse
    // 4. dir-light specular
    std::vector<DirectionalLight> directional_lights = {
        {{4.07789f, -1.60387f, -0.588997f}, {0.5f, 0.5f, 0.5f}, {0.4f, 0.4f, 0.4f}, {0.5f, 0.5f, 0.5f}}};

    // 1. point-light direction
    // 2. point-light ambient
    // 3. point-light diffuse
    // 4. point-light specular
    // 5. number of a pl (its index)
    // 6. constant (default: 1.0)
    // 7. linear (default: 0.09)
    // 8. quadratic (default: 0.032)
    std::vector<PointLight> point_lights = {
        {{-4.94625f, 15.0549f, 6.95857f}, {0.8f, 0.8f, 0.8f}, {0.8f, 0.8f, 0.8f}, {1.0f, 1.0f, 1.0f}, 0},
        {{3.96999f, -5.70809f, -13.2864f}, {0.8f, 0.8f, 0.8f}, {0.8f, 0.8f, 0.8f}, {1.0f, 1.0f, 1.0f}, 1},
        {{18.0768f, 3.03289f, 3.76213f}, {0.8f, 0.8f, 0.8f}, {0.8f, 0.8f, 0.8f}, {1.0f, 1.0f, 1.0f}, 2},
        {{-17.0855f, 7.29996f, -3.69458f}, {0.8f, 0.8f, 0.8f}, {0.8f, 0.8f, 0.8f}, {1.0f, 1.0f, 1.0f}, 3},
    };
} // namespace lightning

namespace model
{
    // The startup model, you also can remain it empty
    // Only the .obj and .stl are admissible 
    std::string startup_model = "";

    // I'd recomend to leave these vars with these values
    float model_rotation_angle_x = 0.0f;
    float model_rotation_angle_y = 0.0f;
    float model_rotation_angle_z = 0.0f;
    glm::vec3 model_scaling = {0.01f, 0.01f, 0.01f};
    glm::vec3 model_translation = {-1.75f, -1.75f, 0.0f};
} // namespace model

namespace texture
{
    // Any texture file
    std::string texture_file = "";
}

namespace camera
{
    // Also recomend leave them like that
    glm::vec3 cam_position = {0.0f, 0.0f, 0.0f};
    glm::vec3 world_up = {0.f, 1.f, 0.f};
    glm::vec3 cam_front = {-1.75f, -1.75f, 0};
} // namespace camera
```

* `title` - the title of a window if you build the app via Visual Studio 2019
* `width` - width of a window
* `height` - height of a window
* `resizable` - ability of window's resize 
* `speed` - camera's movement speed
* `sensitivity` - mouse's sensitivity
* `shader_file` - the file with your own shaders. The file should be placed at `assets/shaders`, they have to coincide with pattern: `shader_file.vs`, `shader_file.frag`, `shader_file.wvs`, `shader_file.wfrag`. If you leave this field empty (`""`), then the base file will be loading (the name of base shaders is `core`, DO NOT REMOVE THIS FILE).
* `MSAA` - antialiasing. I use only MSAA ([shorthand of MSAA](https://mynameismjp.wordpress.com/2012/10/24/msaa-overview)).
* `line_mode` - the mode, where textures displays as lines between vertices without filling inside.
* `is_light_shader` - set **true** if shader handles lights. Note, I can handle only the directional and point lights.
* `directional_lights` - set settings for the directional lights:
  * 1'st parameter - `direction`
  * 2'nd parameter - `ambient`
  * 3'rd parameter - `diffuse`
  * 4'th parameter - `specular`
* `point_lights` - set settings for the point lights:
  * 1'st parameter - `direction`
  * 2'nd parameter - `ambient`
  * 3'rd parameter - `diffuse`
  * 4'th parameter - `specular`
  * 5'th parameter - index of a light unit, in ascending order.
  * 6'th parameter - `constant` (default: 1.0)
  * 7'th parameter - `linear` (default: 0.09)
  * 8'th parameter - `quadratic` (default: 0.032)
* `startup_model` - the model, which will be pre-loaded on startup. I'd rcomend you to remain this field empty (`""`), the load speed will be increased. Only the `.obj` and `.stl` are admissible. Models should be placed in `assets/models`. 
* `texture_file` - the texture's file. Textures should be placed in `assets/images`.
  
**Note**: Following settings better to remain the same.
* `model_rotation_angle_x` - base rotation along OX.
* `model_rotation_angle_y` - base rotation along OY.
* `model_rotation_angle_z` - base rotation along OZ.
* `model_scaling` - base model rescaling. 
* `model_translation` - base model translation. World's shifting.
* `cam_position` - default camera's position.
* `world_up` - the up vector of the world.
* `cam_front` - camera's default view direction.

### Build
There are several ways to build the application. The option depends on which system you work, except Docker, it is suitable for each one.
<details>
<summary>Docker</summary>

- Launch Docker.
- >\> git clone https://github.com/JuiceFV/Emscripten_OpenGL.git
- >\> cd Emscripten_OpenGL
- Perform [application's configuration](#applications-configuration) and [assets preparation](#assets-preparation)
- >\> docker-compose up application
- Once console is freezing, follow to the localhost, port: 8080

</details>

<details>
<summary>Windows 10</summary>
<details>
<summary>Visual Studio 2019</summary>

- Familiarize with [pre-requirements](#installation-requirements)
- >\> git clone https://github.com/JuiceFV/Emscripten_OpenGL.git
- >\> cd Emscripten_OpenGL
- Perform [application's configuration](#applications-configuration) and [assets preparation](#assets-preparation)
- >\> mkdir build
- >\> cd build 
- >\> cmake [-G%generator%:optional] ..
</details>

<details>
<summary>Emscripten</summary>

- Open the WSL2
- Familiarize with [pre-requirements](#installation-requirements) for Linux
- >\> git clone https://github.com/JuiceFV/Emscripten_OpenGL.git
- >\> cd Emscripten_OpenGL
- Perform [application's configuration](#applications-configuration) and [assets preparation](#assets-preparation)
- >\> mkdir build && cd build 
- >\> emcmake cmake -DFOR_EMSDK=ON .. && emmake make
- >\> python3 -m http.server
- Follow the link and follow the path: **application->out->Emscripten_Graphics.html**
</details>

</details>

<details>
<summary>Unix</summary>

- Familiarize with [pre-requirements](#installation-requirements)
- >\> git clone https://github.com/JuiceFV/Emscripten_OpenGL.git
- >\> cd Emscripten_OpenGL
- Perform [application's configuration](#applications-configuration) and [assets preparation](#assets-preparation)
- >\> mkdir build && cd build 
- >\> emcmake cmake -DFOR_EMSDK=ON .. && emmake make
- >\> python3 -m http.server
- Follow the link and follow the path: **application->out->Emscripten_Graphics.html**
</details>

## Usage

### Emscripten

If you built code correctly, then you should see the main page:

![image](https://user-images.githubusercontent.com/35202460/101258865-6c51e480-3736-11eb-9674-edbc05bfc15b.png)
By clicking on the  "Emscripten Graphics" you have to see the menu.

![image](https://user-images.githubusercontent.com/35202460/101258910-a7541800-3736-11eb-8999-6e062035e352.png)

- **Home** - main page with the title "Emscripten Graphics"
- **About** - the page with the main information about the application and its usage.
- **Canvas** - the canvas itself
- **Code** - link to this repository.
- **Contacts** - my contacts.

About page looks like that. By hovering over bubble it will be expanded and additional information appears.

![image](https://user-images.githubusercontent.com/35202460/101259197-796fd300-3738-11eb-8204-a17572fdfea2.png)

By hovering over "**settings**" and clicking on "**upload obj**" you'ill be able to upload [pre-loaded](#models) models.
![image](https://user-images.githubusercontent.com/35202460/101259272-f13dfd80-3738-11eb-856d-4f64129bad8b.png)

You have two options: normal mode and rotation mode

* **Normal mode**
  * `W` `A` `S` `D` - forward, leftwise, backward, rightwise, respectively camera's movement.
  * `mouse movement` - camera's rotation
  * `mouse wheel` - zoom
  * `esc` - out from canvas (cursor appears)
* **Rotation mode** (to turn it on - hold RMB, right mouse button)
  * `W` `A` `S` `D` - forward, leftwise, backward, rightwise, respectively camera's movement.
  * `left\right mouse movements` - OX rotation
  * `up\down mouse movements` - OY rotation
  * `mouse wheel` - OZ rotation

In **rotation mode** camera's view direction is freezing.
As a result you see your model:

![image](https://user-images.githubusercontent.com/35202460/101259756-e0db5200-373b-11eb-94fb-3bb1d7651d9d.png)

### Visual Studio 2019

To open filebrowser press `Cntrl+O`:
![image](https://user-images.githubusercontent.com/35202460/101260163-f69e4680-373e-11eb-84ff-dee049618866.png)

You have two options: normal mode and rotation mode
* **Normal mode**
  * `W` `A` `S` `D` - forward, leftwise, backward, rightwise, respectively camera's movement.
  * `mouse movement` - camera's rotation
  * `mouse wheel` - zoom
* **Rotation mode** (to turn it on - hold RMB, right mouse button)
  * `W` `A` `S` `D` - forward, leftwise, backward, rightwise, respectively camera's movement.
  * `left\right mouse movements` - OX rotation
  * `up\down mouse movements` - OY rotation
  * `mouse wheel` - OZ rotation

In **rotation mode** camera's view direction is freezing. 
As a result you see your model:
![image](https://user-images.githubusercontent.com/35202460/101260071-2bf66480-373e-11eb-8727-8cde3b7156b4.png)

## Rebuild

If you modified content of assets and want to rebuild the project:

* In case you built rawly, just type:
  >\> make

  or 
  >\> emmake make

* In case you built via Docker, just type:
  >\> docker-compose build application

  >\> docker-compose up application

## TODO
* Add ability to select any model, not only prepared
* Fix troubles with axis of different models