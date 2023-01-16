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
#include <iostream>
#include <filesystem>
#include <fstream>

// ANSI codes on linux and macos, empty on windows
#if defined (__linux__) || defined (__APPLE__)
    #define RESET "\033[0m"
    #define RED "\033[31m"
    #define GREEN "\033[32m"
    #define YELLOW "\033[33m"
    #define BLUE "\033[34m"
#else
    #define RESET ""
    #define RED ""
    #define GREEN ""
    #define YELLOW ""
    #define BLUE ""
#endif

void create_project() {
    std::cout << BLUE << "Creating project..." << RESET << std::endl;
    
    std::string project_name;
    std::cout << "Project name: ";
    getline(std::cin, project_name);

    std::string target;
    std::cout << "Target [default: " << project_name << "]: ";
    getline(std::cin, target);

    if (target.empty())
        target = project_name;

    std::cout << "Output file extension [default: none]: ";
    std::string extension;
    getline(std::cin, extension);

    if (!extension.empty())
        target += "." + extension;

    try {
        std::filesystem::create_directory(project_name);
        std::filesystem::create_directory(project_name + "/src");
        std::filesystem::create_directory(project_name + "/include");
        std::filesystem::create_directory(project_name + "/obj");
        std::filesystem::create_directory(project_name + "/bin");
    } catch (std::filesystem::filesystem_error& e) {
        std::cout << RED << "Error: " << e.what() << RESET << std::endl;
        exit(1);
    }

    std::ofstream main_file(std::filesystem::current_path() / project_name  / "src" / "main.cpp");

    if (!main_file.is_open()) {
        std::cout << RED << "Error: Could not create main file" << RESET << std::endl;
        exit(1);
    }

    main_file 
        << "// This is the main file of your project" << std::endl << std::endl
        << "int main(int argc, char **argv) {" << std::endl
        << "    return 0;" << std::endl
        << "}" << std::endl;

    main_file.close();

    std::ofstream makefile(std::filesystem::current_path() / project_name / "Makefile");

    if (!makefile.is_open()) {
        std::cout << RED << "Error: Could not create Makefile" << RESET << std::endl;
        exit(1);
    }

    makefile
    << "CC = g++" << std::endl
    << "CFLAGS = -Wall -std=c++17" << std::endl
    << "INCLUDE = -Iinclude" << std::endl
    << "SRC = src" << std::endl
    << "OBJ = obj" << std::endl
    << "BIN = bin" << std::endl
    << "TARGET = " << target << std::endl
    << std::endl
    << "all: $(TARGET)" << std::endl
    << std::endl
    << "$(TARGET): $(OBJ)/main.o" << std::endl
    << "\t@mkdir -p $(BIN)" << std::endl
    << "\t@$(CC) $(CFLAGS) $(INCLUDE) -o $(BIN)/$(TARGET) $^" << std::endl
    << std::endl
    << "$(OBJ)/main.o: $(SRC)/main.cpp" << std::endl
    << "\t@mkdir -p $(OBJ)" << std::endl
    << "\t@$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $<" << std::endl
    << std::endl
    << "clean:" << std::endl
    << "\t@rm -rf $(OBJ) $(BIN)" << std::endl
    << std::endl
    << ".PHONY: all clean" << std::endl
    << "install: all" << std::endl
    << "\t@cp $(BIN)/$(TARGET) /usr/local/bin" << std::endl
    << std::endl;

    std::cout << GREEN << "Project created!" << RESET << std::endl;

}

int main(int argc, char **argv) {
    cxxopts::Options options("mkpj", "MakeProject - A simple project creator");
    options.add_options()
        ("c,create", "Creates the project")
        ("h,help", "Print usage")
        ;

    try {
        auto result = options.parse(argc, argv);
        options.allow_unrecognised_options();

        if (result.count("help")) {
            std::cout << options.help() << std::endl;
            exit(0);
        }

        else if (result.count("init")) {
            create_project();
            exit(0);
        }

        else {
            std::cout 
                << RED << "Install: missing operand" << RESET << std::endl
                << "Try 'mkpj --help' for more information." << std::endl; 

            exit(1);
        }

    } catch (const cxxopts::OptionException& e) {
        std::cout << options.help() << std::endl;
        exit(0);
    }

    return 0;
}