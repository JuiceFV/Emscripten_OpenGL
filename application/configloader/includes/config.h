#pragma once

#include "yaml-cpp/yaml.h"
#include <filesystem>
#include <iostream>
#include <string>
#include <unordered_map>
#include <exception>

namespace fs =
#if defined(_MSC_VER) || defined(_WIN32)
    std::filesystem;
#else
    std::__fs::filesystem;
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
        std::unordered_map<std::string, unsigned int> getWindowProps();
        ~Config();

      private:
        fs::path Path;
        YAML::Node config;
    };
} // namespace cfg
