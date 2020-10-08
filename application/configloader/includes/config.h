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
        std::string getPath();
        // TODO create dynamical return of a property depends on passed enum arg
        auto getWindowProp();
        ~Config();

      private:
        fs::path Path;
        YAML::Node config;
        std::unordered_map<std::string, unsigned int> win_size;

        std::unordered_map<std::string, unsigned int> parseWinSize();
    };
} // namespace cfg
