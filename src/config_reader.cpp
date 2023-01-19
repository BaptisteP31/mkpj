#include "../include/config_reader.hpp"

#include <sstream>
#include <filesystem>

Config::Config(std::filesystem::path path) {
    this->path = path;
}

bool Config::load() {
    std::ifstream config_file(this->path);

    if (!config_file.is_open())
        return false;

    std::string line;

    // Read the config file line by line, values are separated by '='
    // ignore comments (lines starting with '#')
    while (getline(config_file, line)) {
        if (line[0] == '#')
            continue;

        std::stringstream ss(line);
        std::string key, value;

        getline(ss, key, '=');
        getline(ss, value, '=');

        if (key == "name ")
            // Remove the first character (space) if the value is not empty using ternary operator
            this->name = value.empty() ? value : value.substr(1);
        
        else if (key == "target ")
            this->target = value.empty() ? value : value.substr(1);
        
        else if (key == "extension ")
            this->extension = value.empty() ? value : value.substr(1);
        
        else if (key == "additional_files ")
            this->additional_files = value.empty() ? value : value.substr(1);
        
        else if (key == "is_licenced ") {
            if (value == " true") this->is_licenced = true;
            else if (value == " false") this->is_licenced = false;
        }
            
    }

    return true;
}

bool Config::empty() {
    return this->name.empty() || this->target.empty() || this->extension.empty();
}

Project Config::get_project_info() {
    Project project;
    project.name = this->name;
    project.path = std::filesystem::current_path();
    project.target = this->target;
    project.extension = this->extension;
    return project;
}