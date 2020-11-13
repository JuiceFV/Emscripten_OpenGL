#pragma once

#include "globals.h"
#include <yaml-cpp/yaml.h>
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
        Config();                         // tested
        Config(const std::string &path);  // tested
        std::string getPath() const;      // tested
        void uploadDefaultCfg();          // tested
        bool isConfigDefault() const;     // tested
        YAML::Node getConfigFile() const; // not tested, mock required
#ifndef __EMSCRIPTEN__
        friend std::ostream &operator<<(std::ostream &out, cfg::Config &config)
        {
            out << "application:" << std::endl;
            out << "    window:" << std::endl;
            out << "        width:" << config.getWindowProp<unsigned int>(cfg::defaults::WIN_PROPS::WIDTH) << std::endl;
            out << "        height:" << config.getWindowProp<unsigned int>(cfg::defaults::WIN_PROPS::HEIGHT)
                << std::endl;
            return (out);
        };
#endif

        template <typename T> T getWindowProp(const defaults::WIN_PROPS _property)
        {
            switch (_property)
            {
            case defaults::WIN_PROPS::WIDTH: return (this->win_size["width"]); break;
            case defaults::WIN_PROPS::HEIGHT: return (this->win_size["height"]); break;
            }
        }; // tested

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
