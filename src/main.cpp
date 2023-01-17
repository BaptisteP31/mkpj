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

#include <iostream>
#include <filesystem>
#include <fstream>

int main(int argc, char **argv) {
    cxxopts::Options options("mkpj", "MakeProject - A simple project creator");
    options.add_options()
        ("c,create", "Creates the project")
        ("m,makefile", "Creates or updates the Makefile")
        ("t,tarball", "Creates a tarball of the project")
        ("h,help", "Print usage")
        ;

    try {
        auto result = options.parse(argc, argv);
        options.allow_unrecognised_options();

        if (result.count("help")) {
            std::cout << options.help() << std::endl;
            exit(0);
        }

        else if (result.count("create")) {
            create_project();
            exit(0);
        }

        else if (result.count("makefile")) {
            Config config(std::filesystem::current_path() / ".mkpj.conf");

            if (!config.load()) {
                std::cerr << RED << "Error: Could not load config file" << RESET << std::endl;
                exit(1);
            }

            Project project = config.get_project_info();

            if (!create_makefile(project, true)) {
                std::cerr << RED << "Error: Could not create Makefile" << RESET << std::endl;
                exit(1);
            }
        }

        else if (result.count("tarball")) {
            // The tarball contains the project include, src and Makefile

            Config config(std::filesystem::current_path() / ".mkpj.conf");
            if (!config.load()) {
                std::cerr << RED << "Error: Could not load config file" << RESET << std::endl;
                exit(1);
            }

            std::string command = "tar -czf " + config.name + ".tar.gz " + "src/ " + "include/ " + "Makefile";
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