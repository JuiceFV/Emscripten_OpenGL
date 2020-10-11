#pragma once

#include "globals.h"
#include "yaml-cpp/yaml.h"
#include <exception>
#include <filesystem>
#include <iostream>
#include <string>
#include <unordered_map>

namespace fs =
#if defined(__EMSCRIPTEN__)
    std::__fs::filesystem;
#else
    std::filesystem;
#endif

namespace cfg
{
    class Config
    {
      public:
        Config();
        Config(const std::string &path);
        std::string getPath() const;
        void uploadDefaultCfg();
        bool isConfigDefault() const;
        YAML::Node getConfigFile() const;
        
        template<typename T>
        T getWindowProp(const defaults::WIN_PROPS _property)
        {
            switch (_property)
            {
                case defaults::WIN_PROPS::WIDTH: return this->win_size["width"]; break;
                case defaults::WIN_PROPS::HEIGHT: return this->win_size["height"]; break;
            }
        };

        ~Config();

      private:
        fs::path Path;
        YAML::Node cfg_file;

        std::unordered_map<std::string, unsigned int> win_size;

        void defineConfig(const std::string &path);
        std::unordered_map<std::string, unsigned int> parseWinSize();

        bool is_default;
    };
} // namespace cfg
