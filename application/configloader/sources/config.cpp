#include "config.h"

cfg::Config::Config()
{
// A little explanation:
// The problem appears when the code compiles via emcc.
// Emscripten "mounts" its own file system in purpose to avoid
// spare files. Actually, emscripten's team came up with the great idea.
// Intending to achive the best performance and size - emcc incorporates
// those files required by code. There are some ways to mount the host
// volume whether through code or using flags. However, it's not demanded.
// We may pass the required files, which are not appear in the code, via
// the flag `--preload-file /form/where/@/to/where`. The problem is
// that the default dir is the root "/". If we run the application
// using non-emcc, to acquire the config.yaml we have to step back to
// the previous directory and then move to config-directory (`../config/config.yaml).
// Opposite to non-emcc the emcc sets the base directory as root one and then 
// move in the config-directory (`/config/config.yaml`).
#if defined(__EMSCRIPTEN__)
    this->Path = fs::current_path() /=
#else
    this->Path = fs::current_path().parent_path() /=
#endif
        "config/appconfig.yaml";
    for (auto &p : fs::directory_iterator(this->getPath())) std::cout << p.path() << '\n';
    try
    {
        if ((this->config = YAML::LoadFile(this->Path.string())["application"]))
        {
            this->win_size = this->parseWinSize();
        }
        else
        {
            throw std::invalid_argument("Missing 'application' parameter in 'appconfig.yaml'.");
        }
    }
    catch (std::exception &error)
    {
        std::cerr << "An error has occured in the " << __FILE__ << " in line " << __LINE__ << "." << std::endl
                  << "Details: " << error.what() << std::endl
                  << "You have to directly assign the application's settings." << std::endl;
        // TODO may be add a default config if one was missing in config.yaml
        exit(-1);
    }
}

cfg::Config::Config(const std::string &path) {}

std::string cfg::Config::getPath() { return (this->Path.parent_path().string()); }

// TODO create dynamical return of a property depends on passed enum arg
auto cfg::Config::getWindowProp(){

};

// The private method which parses and returns width and heigth as dict.
std::unordered_map<std::string, unsigned int> cfg::Config::parseWinSize()
{
    std::unordered_map<std::string, unsigned int> res; // resulted map
    unsigned int width = defaults::WIN_WIDTH;          // base width of window
    unsigned int height = defaults::WIN_HEIGHT;        // base height of window

    // Check if width/height or at least window parameter has been passed.
    // Otherwise, notify an user and set these parameters with default values.
    try
    {
        if (this->config["window"])
        {
            if (this->config["window"]["width"])
                width = this->config["window"]["width"].as<unsigned int>();
            else
                throw std::invalid_argument("Missing 'window.width' parameter in 'appconfig.yaml'.");
            if (this->config["window"]["height"])
                height = this->config["window"]["height"].as<unsigned int>();
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

cfg::Config::~Config() {}
