#include "shader.h"

Shader::Shader(const char *vertex_path, const char *fragment_path)
{
    unsigned int vertex_shader = 0;
    unsigned int fragment_shader = 0;

    vertex_shader = compileShader(GL_VERTEX_SHADER, vertex_path);
    fragment_shader = compileShader(GL_FRAGMENT_SHADER, fragment_path);

    this->linkProgram(vertex_shader, fragment_shader);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

std::string Shader::loadShaderFile(const char *path)
{
    std::string shader_code;   // The code of shaders as a string
    std::ifstream shader_file; // The very file
    // ensures ifstream objects able to throw exceptions
    shader_file.exceptions(std::ifstream::badbit);
    try
    {
        // Open file
        shader_file.open(path);
        std::stringstream shader_stream;
        // Read file's buffer contents into stream
        shader_stream << shader_file.rdbuf();
        // close file handler
        shader_file.close();
        // Convert stream into string
        shader_code = shader_stream.str();
    }
    catch (std::ifstream::failure &error)
    {
        std::cerr << "An error has occured in the " << __FILE__ << " in line " << __LINE__ << "." << std::endl
                  << "Details: " << error.what() << std::endl;
    }
    return (shader_code);
}

unsigned int Shader::compileShader(unsigned int type, const char *path)
{
    char log_info[512]; // log information if something wrong
    int success;        // the variable indicates if a shader's compilation went wrong

    // shader of a specific type (vertex/fragment/geometry)
    unsigned int shader = glCreateShader(type);
    // load source code from a file
    std::string str_shader_code = loadShaderFile(path);
    const char *shader_code = str_shader_code.c_str();
    // compile shaders itself
    glShaderSource(shader, 1, &shader_code, NULL);
    glCompileShader(shader);

    // check if a compilation done well
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, log_info);
        std::cerr << "An error has occured in the " << __FILE__ << " in line " << __LINE__ << "." << std::endl
                  << "Error occured in the: " << path << std::endl
                  << "Details: " << log_info << std::endl;
    }
    return shader;
}

void Shader::linkProgram(unsigned int vertex_shader, unsigned int fragment_shader)
{
    char log_info[512];
    int success;

    this->program = glCreateProgram();

    glAttachShader(this->program, vertex_shader);
    glAttachShader(this->program, fragment_shader);
    glLinkProgram(this->program);

    glGetProgramiv(this->program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(this->program, 512, NULL, log_info);
        std::cerr << "An error has occured in the " << __FILE__ << " in line " << __LINE__ << "." << std::endl
                  << "Details: " << log_info << std::endl;
    }
    glUseProgram(0);
}

void Shader::Use() { glUseProgram(this->program); }

void Shader::Unuse() { glUseProgram(0); }

void Shader::set1i(int value, const char *name)
{
    this->Use();

    glUniform1i(glGetUniformLocation(this->program, name), value);

    this->Unuse();
}

Shader::~Shader() { glDeleteProgram(this->program); }
