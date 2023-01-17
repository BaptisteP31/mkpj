#include "../include/project.hpp"
#include "../include/config_reader.hpp"
#include "../include/colors.hpp"
#include "../include/download.hpp"

#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <map>


std::string get_user_input(std::string message) {

    std::cout << message << " ";
    std::string input;
    getline(std::cin, input);
    return input;
}

bool create_project_directory(const Project& project) {

    try {
        std::filesystem::create_directory(project.name);
        std::filesystem::create_directory(project.name + "/src");
        std::filesystem::create_directory(project.name + "/include");
        std::filesystem::create_directory(project.name + "/obj");
        std::filesystem::create_directory(project.name + "/bin");

    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;

    }
    return true;
}

bool create_main(const Project& project) {
    std::ofstream main_file(std::filesystem::current_path() / project.name  / "src" / "main.cpp");

    if (!main_file.is_open())
        return false;

    main_file 
        << "// This is the main file of your project" << std::endl << std::endl
        << "int main(int argc, char **argv) {" << std::endl
        << "    return 0;" << std::endl
        << "}" << std::endl;

    main_file.close();

    return true;
}

bool create_makefile(const Project& project, bool regen) {
    std::filesystem::path path = std::filesystem::current_path() / project.name / "Makefile";
    
    if (regen)
        path = std::filesystem::current_path() / "Makefile";

    std::ofstream makefile(path);

    if (!makefile.is_open())
        return false;

    std::string target = project.target;
    if (target.empty())
        target = project.name;

    if (!project.extension.empty())
        target += "." + project.extension;

    makefile // Works with the template
        << "CC = g++" << std::endl
        << "CFLAGS = -Wall -std=c++17 -Iinclude" << std::endl
        << "LDFLAGS = " << std::endl
        << "LDLIBS = " << std::endl
        << "SRC = $(wildcard src/*.cpp)" << std::endl
        << "OBJ = $(SRC:src/%.cpp=obj/%.o)" << std::endl
        << "TARGET = " << target << std::endl
        << std::endl
        << "all: $(TARGET)" << std::endl
        << std::endl
        << "$(TARGET): $(OBJ)" << std::endl
        << "\t$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)" << std::endl
        << std::endl
        << "obj/%.o: src/%.cpp" << std::endl
        << "\t$(CC) $(CFLAGS) -o $@ -c $<" << std::endl
        << std::endl
        << "clean:" << std::endl
        << "\trm -f $(OBJ)" << std::endl
        << std::endl
        << "fclean: clean" << std::endl
        << "\trm -f $(TARGET)" << std::endl
        << std::endl
        << "re: fclean all" << std::endl;
    

    makefile.close();

    return true;
}

bool create_config(const Project& project) {
    Config config(std::filesystem::current_path() / project.name / ".mkpj.conf");

    std::ofstream config_file(config.path);

    if (!config_file.is_open())
        return false;

    config_file
        << "# project configuration" << std::endl
        << "# Edit this if you know what you are doing" << std::endl
        << "name = " << project.name << std::endl
        << "target = " << project.target << std::endl
        << "extension = " << project.extension << std::endl;

    config_file.close();

    return true;
}

bool create_readme(const Project& project) {
    std::ofstream readme(std::filesystem::current_path() / project.name / "README.md");

    if (!readme.is_open())
        return false;

    readme
        << "# " << project.name << "  " << std::endl << std::endl

        << "### Compilation" << std::endl
        << "```" << std::endl
        << "make" << std::endl
        << "```" << std::endl
        << "---" << std::endl << std::endl

        << "### Execution" << std::endl
        << "```" << std::endl
        << "bin/" << project.target << std::endl
        << "```" << std::endl
        << "---" << std::endl << std::endl

        << "### Cleaning" << std::endl
        << "```" << std::endl
        << "make clean" << std::endl
        << "```" << std::endl
        << "---" << std::endl << std::endl

        << "### Installation" << std::endl
        << "```" << std::endl
        << "sudo make install" << std::endl
        << "```" << std::endl;

    readme.close();

    return true;
}

bool get_licence(const Project& project) {
    std::string rep = get_user_input("Would you like to add a licence? [y/n]");

    const std::string reponses[] = {"y", "Y"};
    bool no_licence = rep.find(reponses[0]) == std::string::npos && rep.find(reponses[1]) == std::string::npos;

    if (no_licence)
        return false;

    std::cout << std::endl << "Available licences:" << std::endl;
    std::cout << "1. GPL" << std::endl;
    std::cout << "2. MIT" << std::endl;
    std::cout << "3. Apache" << std::endl;
    std::cout << "4. BSD" << std::endl;
    std::cout << "5. Mozilla" << std::endl << std::endl;

    std::string licence = get_user_input("Licence number:");

    std::map <std::string, std::string> licences = {
        {"1", "https://www.gnu.org/licenses/gpl-3.0.txt"},
        {"2", "https://pastebin.com/raw/23YFqm6x"},
        {"3", "https://www.apache.org/licenses/LICENSE-2.0.txt"},
        {"4", "https://raw.githubusercontent.com/Illumina/licenses/master/Simplified-BSD-License.txt"},
        {"5", "https://www.mozilla.org/media/MPL/2.0/index.815ca599c9df.txt"}
    };

    std::string licence_url = licences[licence];
    std::cout << YELLOW << "Downloading licence..." << RESET << std::endl;
    download_from_internet(licence_url, std::filesystem::current_path() / project.name / "LICENCE");
    std::cout << GREEN << "Licence downloaded" << RESET << std::endl;

    return true;
}

void create_project() {
    Project project;

    project.name = get_user_input("Project name:");
    if (project.name.empty()) {
        std::cerr << RED << "Error: Project name cannot be empty" << RESET << std::endl; 
        exit(1);
    } 

    project.path = std::filesystem::current_path() / project.name;
    if (std::filesystem::exists(project.path)) {
        std::cerr << RED << "Error: Project already exists" << RESET << std::endl;
        exit(1);
    }

    project.target = get_user_input("Output file name [default: " + project.name + "]:");
    project.extension = get_user_input("Output file extension [default: none]:");

    if (project.target.empty())
        project.target = project.name;

    if (!project.extension.empty())
        project.target += "." + project.extension;

    if (!create_project_directory(project) || !create_main(project) || !create_makefile(project)) {
        std::cerr<< RED << "Error: Could not create project" << RESET << std::endl;
        exit(1);
    }

    if (!create_config(project)) {
        std::cerr<< RED << "Error: Could not create config file" << RESET << std::endl;
        exit(1);
    }

    if (!create_readme(project)) {
        std::cerr<< RED << "Error: Could not create README file" << RESET << std::endl;
        exit(1);
    }

    get_licence(project);

    std::cout << GREEN << "Project created!" << RESET << std::endl;
}