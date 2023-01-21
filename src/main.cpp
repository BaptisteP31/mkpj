/*

mkpj a simple project creator for C++ projects

Copyright (C) 2023  Paqueriaud Baptiste

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/

#include "../include/cxxopts.hpp"
#include "../include/config_reader.hpp"
#include "../include/project.hpp"
#include "../include/colors.hpp"
#include "../include/download.hpp"
#include "../include/pair.hpp"

#include <bits/stdc++.h>

#define VERSION "0.6"

int main(int argc, char **argv) {
    cxxopts::Options options("mkpj", "MakeProject - A simple project creator");
    options.add_options()
        ("c,create", "Creates the project")
        ("a,add", "Adds a cpp/hpp file to the project")
        ("l,list", "Lists all the available pairs")
        ("u,update", "Updates the pair list")
        ("p, pairs", "Adds a pair from the pair list", cxxopts::value<std::string>())
        ("m,makefile", "Creates or updates the Makefile")
        ("t,tarball", "Creates a tarball of the project")
        ("v,version", "Prints the version of mkpj")
        ("g,languages", "Prints all the languages supported by mkpj")
        ("h,help", "Print usage")
        ;

    try {
        auto result = options.parse(argc, argv);
        options.allow_unrecognised_options();

        if (result.count("help")) {
            std::cout << options.help() << std::endl;
            exit(0);
        }

        else if (result.count("languages")) {
            std::cout
                << "Available languages : " << std::endl
                << BLUE << "\t• C++" << std::endl
                        << "\t• C++ (QT)" << std::endl
                << RESET <<
            std::endl;
        }

        else if (result.count("version")) {
            std::cout 
                << "mkpj "<< VERSION << std::endl
                << "Copyright (C) 2023  Paqueriaud Baptiste" << std::endl
                << "License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>." << std::endl
                << "This is free software; see the source for copying conditions.  There is NO" << std::endl
                << "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE." << std::endl
                << std::endl
                << "Written by Paqueriaud Baptiste and Nini Rabah." << std::endl;
            exit(0);
        }

        else if (result.count("create")) {
            create_project();
            exit(0);
        }

        else if (result.count("add")) {
            add_cpp_hpp();
            exit(0);
        }

        else if (result.count("list")) {
            collection pairs = get_pairs();
            display_pairs(pairs);
        }

        else if (result.count("update")) {
            Config config(std::filesystem::current_path() / ".mkpj.conf");
            if (!config.load()) {
                std::cerr << RED << "Error: Could not load config file, make sure you are in a project directory" << RESET << std::endl;
                exit(1);
            }
            update_pairs(config.get_project_info());
            std::cout << GREEN << "Pairs updated" << RESET << std::endl;
        }

        else if (result.count("pairs")) {
            std::string pair_name = result["pairs"].as<std::string>();
            download_pair(pair_name);        
        }

        else if (result.count("makefile")) {
            Config config(std::filesystem::current_path() / ".mkpj.conf");

            if (!config.load()) {
                std::cerr << RED << "Error: Could not load config file, make sure you are in a project directory" << RESET << std::endl;
                exit(1);
            }

            Project project = config.get_project_info();

            if (!create_makefile(project, true)) {
                std::cerr << RED << "Error: Could not create Makefile" << RESET << std::endl;
                exit(1);
            }
        }

        else if (result.count("tarball")) {
            Config config(std::filesystem::current_path() / ".mkpj.conf");
            if (!config.load()) {
                std::cerr << RED << "Error: Could not load config file, make sure you are in a project directory" << RESET << std::endl;
                exit(1);
            }

            std::string command = "tar -czf " + config.name + ".tar.gz " + "src/ " + "include/ " + "Makefile " + "README.md" + (config.is_licensed ? " LICENSE " : " ") + config.additional_files;
            std::cerr << command << std::endl;
            system(command.c_str());
        }

        else {
            std::cout 
                << RED << "Missing operand" << RESET << std::endl
                << "Try 'mkpj --help' for more information." << std::endl; 

            exit(1);
        }

    } catch (const cxxopts::OptionException& e) {
        std::cout << options.help() << std::endl;
        exit(0);
    }

    return 0;
}
