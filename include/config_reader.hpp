#pragma once

#ifndef config_HPP
#define config_HPP

#include "../include/project.hpp"
#include <bits/stdc++.h>
namespace fs = std::filesystem;

namespace config {
    using key_value         = std::pair     <std::string, std::string>;
    using key_value_vector  = std::vector   <key_value>;
    using Configs           = std::vector   <std::pair<std::string, key_value_vector>>;
    using ConfigMap         = std::map      <std::string, std::string>;

    class ConfigFile {
        Configs         configs;
        bool            loaded = false;
        bool            empty = true;
        std::fstream    file;

        public:
            ConfigFile();
            
            void                                load_from_file(fs::path);
            Configs                             to_vector();
            std::map<std::string, std::string>  to_map(const std::string config_name = "default");
            bool                                is_empty();
            bool                                is_loaded();
            std::vector<std::string>            list_configs();

            Project                             get_project_info();
    };

}

#endif
