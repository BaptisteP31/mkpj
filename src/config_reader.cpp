#include "../include/config_reader.hpp"
#include "../include/project.hpp"

#include <fstream>
#include <map>

/// @brief Split a string by a delimiter
/// @param str the string to split
/// @param delimiter the delimiter to split the string by
/// @return a vector of strings containing the splitted string
std::vector<std::string> split(std::string str, char delimiter) {
    std::vector<std::string> internal;
    std::stringstream ss(str);
    std::string tok;

    while(getline(ss, tok, delimiter)) {
        internal.push_back(tok);
    }

    return internal;
}

namespace config {
    
        /// @brief Construct a new Config object
        ConfigFile::ConfigFile() 
                  : configs({}), loaded(false), empty(true) {}

        /// @brief Load the config file and parse it, the config file must be in the following format: key = value and comments must be preceded by a #
        /// @param path The path to the config file
        void ConfigFile::load_from_file(fs::path path) {
            if (!fs::exists(path)) {
                loaded = false;
                return;
            }

            file.open(path, std::ios::in);
            loaded = file.is_open();

            std::string line;
            std::string name = "default";
            while(getline(file, line)) {
                // If the line is empty, we skip it
                if (line.empty() || line == "#")
                    continue;

                // If the line starts with [ and ends with ], it's a config name
                if (line[0] == '[' && line[line.size() - 1] == ']') {
                    name = line.substr(1, line.size() - 2);
                    // We add the config to the list of configs
                    this->configs.push_back({name, {}});
                    continue;
                }

                // Read the key and the value
                std::vector<std::string> keyValue = split(line, '=');
                
                // Resize the key_value vector to 2, used to prevent segfaults later
                keyValue.resize(2);

                // remove the space at the end of the key and the beginning of the value if they exist, it is often the case ( key = value )
                if (keyValue[0].back() == ' ')
                    keyValue[0].resize(keyValue[0].size() - 1);
                if (keyValue[1][0] == ' ')
                    keyValue[1].erase(0, 1);
                
                if (keyValue[0].empty() || keyValue[1].empty())
                    continue;

                // If there is no config, we create a default one
                if (configs.empty())
                    configs.push_back({"default", {}});

                // Search for the key in the config and if it's found, we skip it, we don't want to add the same key twice otherwise the will shadow the previous one
                if (std::find_if(configs.back().second.begin(), configs.back().second.end(), [&keyValue](key_value kv) { return kv.first == keyValue[0]; }) != configs.back().second.end())
                    continue;



                // Add the key-value pair to the config
                configs.back().second.push_back({keyValue[0], keyValue[1]});
            }
            if (!configs.empty())
                empty = false;
                
            file.close();
        }

        /// @brief Convert a given config to a map
        /// @param config_name The name of the config to convert (default if not specified) or the first config if there is no default config
        /// @return A map of key-value pairs
        std::map<std::string, std::string> ConfigFile::to_map(const std::string config_name) {
            std::map<std::string, std::string> map;
            bool found_default = false;
            for (auto config : configs) {
                if (config.first == config_name) {
                    found_default = true;
                    for (auto kv : config.second) {
                        map[kv.first] = kv.second;
                    }
                }
                if (!found_default) {
                    for (auto kv : config.second) {
                        map[kv.first] = kv.second;
                    }
                }
            }
            return map;
        }

        /// @brief List all the configs in the config file
        /// @return A vector of strings containing the names of the configs
        std::vector<std::string> ConfigFile::list_configs() {
            std::vector<std::string> configs;
            for (auto config : this->configs) {
                configs.push_back(config.first);
            }
            return configs;
        }

        /// @brief Get the project info from the config file
        /// @return a Project struct containing the project info
        Project ConfigFile::get_project_info() {
            Project project;

            if (configs.empty())
                return project;

            std::map<std::string, std::string> map = to_map();

            const std::array<std::string, 3> trues = {"true", "True", "TRUE"};

            project.set_name(map["name"]);
            project.set_target(map["target"]);
            project.set_extension(map["extension"]);
            project.set_is_licensed(std::find(trues.begin(), trues.end(), map["is_licensed"]) != trues.end());
            project.set_additional_files(map["additional_files"]);
                        
            
            return project;
        }
} 