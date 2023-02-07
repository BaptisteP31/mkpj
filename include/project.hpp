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
    bool        is_licensed;
    bool        is_QT;
    std::string additional_files;
};

bool create_qt_project          ();
bool create_qt_files            (const Project& project);
bool create_project_directory   (const Project& project);
bool create_main                (const Project& project);
bool create_makefile            (const Project& project, bool regen=false);
bool create_config              (const Project& project);
bool create_readme              (const Project& project);
bool get_license                (const Project& project);
void download_pairs             (const Project& project);
void update_pairs               ();
void create_project             ();
void add_cpp_hpp                ();

#endif