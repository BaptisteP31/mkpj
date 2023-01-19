#pragma once

#ifndef CONFIG_READER_HPP
#define CONFIG_READER_HPP

#include "../include/project.hpp"

#include <string>
#include <filesystem>
#include <fstream>

struct Config {
    Config(std::filesystem::path path);
    std::filesystem::path path;

    std::string name;
    std::string target;
    std::string extension;
    bool is_licenced;
    std::string additional_files;

    bool load();
    bool empty();
    Project get_project_info();
};

#endif