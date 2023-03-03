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
        private:
            Configs         configs;
            bool            loaded;
            bool            empty;
            std::fstream    file;

        public:
            ConfigFile();
            
            void                                load_from_file      (fs::path);
            Project                             get_project_info    ();
            std::vector<std::string>            list_configs        ();
            
            inline Configs                      to_vector           () const { return configs; };
            std::map<std::string, std::string>  to_map              (const std::string config_name = "default");
            
            inline bool                         is_empty            () const { return empty;};
            
            inline bool                         is_loaded           () const { return loaded;};
    };

}

#endif
