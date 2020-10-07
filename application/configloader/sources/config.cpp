#include "config.h"

cfg::Config::Config()
{
    this->Path = fs::current_path().parent_path() /= "config.yaml";

    try
    {
        if (!(this->config = YAML::LoadFile(this->Path.string())["application"]))
            throw std::invalid_argument("Missing 'application' parameter in 'config.yaml'.");
    }
    catch (std::exception &error)
    {
        std::cerr << "An error has occured: " << error.what() << std::endl
                  << "You have to directly assign the application's settings." << std::endl;
        //TODO may be add a default config if one was missing in config.yaml
        exit(-1);
    }
}

cfg::Config::Config(const std::string &path) {}

std::string cfg::Config::getPath() { return (this->Path.string()); }

// TODO create dynamical return of a property depends on passed enum arg
std::unordered_map<std::string, unsigned int> cfg::Config::getWindowProps()
{
    //The result returns as dict. If a user doesn't assign the width and height 
    //manually it becomes 640 and 480 respectively
    std::unordered_map<std::string, unsigned int> res;
    unsigned int width = 640;
    unsigned int height = 480;

    try
    {
        if (this->config["window"])
        {
            if (this->config["window"]["width"])
                width = this->config["window"]["width"].as<unsigned int>();
            else
                throw std::invalid_argument("Missing 'window.width' parameter in 'config.yaml'.");
            if (this->config["window"]["height"])
                height = this->config["window"]["height"].as<unsigned int>();
            else
                throw std::invalid_argument("Missing 'window.height' parameter in 'config.yaml'.");
        }
        else
        {
            throw std::invalid_argument("Missing 'window' parameter in 'config.yaml'.");
        }
    }
    catch (std::exception &error)
    {
        std::cerr << "An error has occured: " << error.what() << std::endl
                  << "The base values, 640x480 will be inserted." << std::endl;
    }

    res.insert({"width", width});
    res.insert({"height", height});
    return res;
};

cfg::Config::~Config() {}
