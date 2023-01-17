#pragma once

#ifndef PROJECT_HPP
#define PROJECT_HPP

#include <string>
#include <filesystem>
#include "../include/colors.hpp"

struct Project {
    std::filesystem::path path;

    std::string name;
    std::string target;
    std::string extension;
};

bool create_project_directory(const Project& project);
bool create_main(const Project& project);
bool create_makefile(const Project& project, bool regen=false);
bool create_config(const Project& project);
bool create_readme(const Project& project);
bool get_licence(const Project& project);
void create_project();

#endif