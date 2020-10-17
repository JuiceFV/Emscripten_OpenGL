#include "config.h"

cfg::Config::Config() : is_default(false)
{
    this->Path = fs::current_path() /= "config/appconfig.yaml";
    this->defineConfig(this->Path.string());
}

cfg::Config::Config(const std::string &path) : is_default(false)
{
    this->Path = path;
    this->defineConfig(this->Path.string());
}

void cfg::Config::defineConfig(const std::string &path)
{
    try
    {
        if ((this->cfg_file = YAML::LoadFile(path)["application"])) { this->win_size = this->parseWinSize(); }
        else
        {
            throw std::invalid_argument("Missing 'application' parameter in 'appconfig.yaml'.");
        }
    }
    catch (std::exception &error)
    {
        std::cerr << "An error has occured in the " << __FILE__ << " in line " << __LINE__ << "." << std::endl
                  << "Details: " << error.what() << std::endl
                  << "You have to directly assign the application's settings." << std::endl
                  << "The default settings will be uploaded, you may familiarize to them in the globals.h" << std::endl;
        this->uploadDefaultCfg();
    }
}

std::string cfg::Config::getPath() const { return this->Path.parent_path().string(); }

// The private method which parses and returns width and heigth as dict.
std::unordered_map<std::string, unsigned int> cfg::Config::parseWinSize()
{
    std::unordered_map<std::string, unsigned int> res; // resulted map
    unsigned int width = defaults::WIN_WIDTH;          // default width of window
    unsigned int height = defaults::WIN_HEIGHT;        // default height of window

    // Check if width/height or at least window parameter has been passed.
    // Otherwise, notify an user and set these parameters with default values.
    try
    {
        if (this->cfg_file["window"])
        {
            if (this->cfg_file["window"]["width"])
                width = this->cfg_file["window"]["width"].as<unsigned int>();
            else
                throw std::invalid_argument("Missing 'window.width' parameter in 'appconfig.yaml'.");
            if (this->cfg_file["window"]["height"])
                height = this->cfg_file["window"]["height"].as<unsigned int>();
            else
                throw std::invalid_argument("Missing 'window.height' parameter in 'appconfig.yaml'.");
        }
        else
        {
            throw std::invalid_argument("Missing 'window' parameter in 'appconfig.yaml'.");
        }
    }
    catch (std::exception &error)
    {
        std::cerr << "An error has occured in the " << __FILE__ << " in line " << __LINE__ << "." << std::endl
                  << "Details: " << error.what() << std::endl
                  << "The base values, 640x480 will be inserted." << std::endl;
    }

    res.insert({"width", width});
    res.insert({"height", height});
    return res;
}

void cfg::Config::uploadDefaultCfg()
{
    this->is_default = true;
    this->Path = "";
    this->cfg_file = NULL;
    this->win_size["width"] = defaults::WIN_WIDTH;
    this->win_size["height"] = defaults::WIN_HEIGHT;
}

bool cfg::Config::isConfigDefault() const { return (this->is_default); }

YAML::Node cfg::Config::getConfigFile() const { return (this->cfg_file); }

cfg::Config::~Config() {}
