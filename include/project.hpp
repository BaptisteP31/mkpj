#pragma once

#ifndef PROJECT_HPP
#define PROJECT_HPP

#include <string>
#include <filesystem>
#include "../include/colors.hpp"

class Project {
    private:
        std::filesystem::path path;
        
        std::string name;
        std::string target;
        std::string extension;

        bool is_licensed;
        std::string license_url;

        std::string additional_files;

    public:
        Project() 
            : path(""), name(""), target(""), extension(""), is_licensed(false), additional_files("") {}
        
        Project(std::filesystem::path path, std::string name, std::string target, std::string extension, bool is_licensed, std::string additional_files = "") 
            : path(path), name(name), target(target), extension(extension), is_licensed(is_licensed), license_url(""), additional_files(additional_files) {}

        // Getters
        std::filesystem::path   get_path()              const { return path; }
        std::string             get_name()              const { return name; }
        std::string             get_target()            const { return target; }
        std::string             get_extension()         const { return extension; }
        bool                    get_is_licensed()           const { return is_licensed; }
        std::string             get_additional_files()  const { return additional_files; }

        // Setters
        void                    set_path(std::filesystem::path _path)               { path = _path; }
        void                    set_name(std::string _name)                         { name = _name; }
        void                    set_target(std::string _target)                     { target = _target; }
        void                    set_extension(std::string _extension)               { extension = _extension; }
        void                    set_is_licensed(bool _is_licensed)                  { is_licensed = _is_licensed; }
        void                    set_additional_files(std::string _additional_files) { additional_files = _additional_files; }

        // Methods
        void                    create_directories();
        void                    create_config();
        void                    create_main();
        void                    create_makefile(bool regen=false);
        void                    create_readme();
        void                    download_pairs();
        void                    download_license();
        void                    update_pairs();
        void                    create_project();
        void                    add_cpp_hpp();
};

void create_project();

#endif