#include "../include/project.hpp"
#include "../include/config_reader.hpp"
#include "../include/colors.hpp"
#include "../include/download.hpp"

#include <bits/stdc++.h>

std::string get_user_input(std::string message) {
    std::cout << message << " ";
    std::string input;
    getline(std::cin, input);
    return input;
}

bool get_user_confirmation(std::string message) {
    std::string rep = get_user_input(message + " [y/N]");
    const std::string responses[] = {"y", "Y"};
    return rep.find(responses[0]) != std::string::npos || rep.find(responses[1]) != std::string::npos;
}

std::pair<std::string, std::string> get_license_choice() {
    std::string rep = get_user_input("Would you like to add a license? [y/N]");

    const std::string responses[] = {"y", "Y"};
    bool no_license = rep.find(responses[0]) == std::string::npos && rep.find(responses[1]) == std::string::npos;
    if (no_license)
        return {"", ""};

    const std::array<std::string, 5> licensesNames = {"GPL", "MIT", "Apache", "BSD", "Mozilla"};
    std::cout << std::endl << "Available licenses:" << std::endl;
    for (size_t i = 0; i < licensesNames.size(); i++)
        std::cout << i + 1 << ". " << licensesNames[i] << std::endl;

    std::string license = get_user_input("Choose a license: ");
    std::map <std::string, std::string> licenses = {
        {"1", "https://www.gnu.org/licenses/gpl-3.0.txt"},
        {"2", "https://pastebin.com/raw/23YFqm6x"},
        {"3", "https://www.apache.org/licenses/LICENSE-2.0.txt"},
        {"4", "https://raw.githubusercontent.com/Illumina/licenses/master/Simplified-BSD-License.txt"},
        {"5", "https://www.mozilla.org/media/MPL/2.0/index.815ca599c9df.txt"}
    };

    std::string licence_url = licenses[license];
    if (licence_url.empty()) {
        std::cerr << RED << "Invalid license, no license added" << RESET << std::endl;
        return {"", ""};
    }

    try { std::stoi(license);} catch (std::invalid_argument& e) { return {"", ""}; };

    return {licensesNames[std::stoi(license) - 1], licence_url};
}

void Project::create_directories() {
    std::filesystem::path path = std::filesystem::current_path() / name;
    std::filesystem::create_directory(path);
    
    const std::array<std::string, 4> directories = {
        "src",
        "include",
        "obj",
        "bin"
    };

    for (const std::string& directory : directories)
        std::filesystem::create_directory(path / directory);
}

void Project::create_config() {
    std::ofstream config_file(path / name / ".mkpj.conf");

    if (!config_file.is_open())
        throw std::runtime_error("Could not create .mkpj.conf in " + path.string() + ", check permissions");

    config_file
        << "# project configuration" << std::endl
        << "# Edit this if you know what you are doing" << std::endl
        << "name = " << name << std::endl
        << "target = " << target << std::endl
        << "extension = " << extension << std::endl
        << "is_licensed = " << ((is_licensed)? "true" : "false") << std::endl
        << "# Additional files to be added to the tarball" << std::endl
        << "additional_files = " << std::endl;

    config_file.close();
}

void Project::create_main() {
    std::ofstream main_file(std::filesystem::current_path() / name  / "src" / "main.cpp");
    (!main_file.is_open())? throw std::runtime_error("Could not create main.cpp in " + path.string() + ", check permissions") : (void)0;

    main_file
        << "// This is the main file of your project" << std::endl << std::endl
        << "#include <iostream>" << std::endl << std::endl
        << "int main(int argc, char **argv) {" << std::endl
        << "    std::cout << \"Hello World!\" << std::endl;" << std::endl
        << "    return 0;" << std::endl
        << "}" << std::endl;

    main_file.close();
}

void Project::create_makefile(bool regen) {
    std::filesystem::path path = std::filesystem::current_path() / name / "Makefile";
    
    if (regen)
        path = std::filesystem::current_path() / "Makefile";

    std::ofstream makefile(path);

    if (!makefile.is_open())
        throw std::runtime_error("Could not create Makefile in " + path.string() + ", check permissions");

    std::string _target = target;    
    (target.empty())? _target = name : _target = target;
    (!extension.empty())? _target += "." + extension : _target = name;

    makefile // Works with the template
        << "CC = g++" << std::endl
        << "CFLAGS = -Wall -std=c++17 -Iinclude" << std::endl
        << "LDFLAGS = " << std::endl
        << "LDLIBS = " << std::endl
        << "SRC = $(wildcard src/*.cpp)" << std::endl
        << "OBJ = $(SRC:src/%.cpp=obj/%.o)" << std::endl
        << "TARGET = " << _target << std::endl
        << std::endl
        << "all: $(TARGET)" << std::endl
        << std::endl
        << "$(TARGET): $(OBJ)" << std::endl
        << "\t$(CC) $(LDFLAGS) -o bin/$@ $(OBJ) $(LDLIBS)" << std::endl
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
        << "configure:" << std::endl
        << "\tmkdir -p obj && mkdir -p bin" << std::endl;

    makefile.close();
}

void Project::create_readme() {
    std::ofstream readme(std::filesystem::current_path() / name / "README.md");
    (!readme.is_open())? throw std::runtime_error("Could not create README.md in " + path.string() + ", check permissions") : (void)0;

    readme
        << "# " << name << "  " << std::endl << std::endl

        << "### Compilation" << std::endl
        << "```" << std::endl
        << "make configure" << std::endl
        << "```" << std::endl << std::endl
        << "```" << std::endl
        << "make" << std::endl
        << "```" << std::endl
        << "---" << std::endl << std::endl

        << "### Execution" << std::endl
        << "```" << std::endl
        << "bin/" << target << std::endl
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
}

void Project::download_pairs() {
    std::string url = "https://raw.githubusercontent.com/BaptisteP31/mkpj/main/pairs.conf";
    std::string path = std::filesystem::current_path() / name / ".pairs.conf";
    download_from_internet(url, path);
}

void Project::download_license() {
    std::pair<std::string, std::string> user_license = get_license_choice();
    const std::string &license = user_license.first;
    const std::string &licence_url = user_license.second;

    // Download license
    std::cout << YELLOW << "Downloading license..." << RESET << std::endl;
    download_from_internet(licence_url, std::filesystem::current_path() / name / "LICENSE");
    std::cout << GREEN << "license downloaded" << RESET << std::endl;

    // Add license to readme
    std::ofstream readme(std::filesystem::current_path() / name / "README.md", std::ios::app);
    (!readme.is_open())? throw std::runtime_error("Could not open README.md in " + path.string() + ", check permissions") : (void)0;

    readme << std::endl << "---" << std::endl << std::endl
           << "### License" << std::endl
           << "```" << std::endl
           << "Licensed under the " << license << " license" << std::endl
           << "```" << std::endl;
    readme.close();

    is_licensed = true;
}

void Project::update_pairs() {
    std::string url = "https://raw.githubusercontent.com/BaptisteP31/mkpj/main/pairs.conf";
    std::string path = std::filesystem::current_path() / name / ".pairs.conf";
    download_from_internet(url, path);
}

void Project::create_project() {
    std::cout << YELLOW << "Creating project..." << RESET << std::endl;

    create_directories();
    create_config();
    create_main();
    create_makefile();
    create_readme();
    download_pairs();
    download_license();

    std::cout << GREEN << "Project created" << RESET << std::endl;
}

void create_project() {
    const std::filesystem::path curr_dir = std::filesystem::current_path();

    std::string name = get_user_input("Project name:");
    
    if (std::filesystem::exists(curr_dir / name)) {
        std::cout << RED << "A project with the same name already exists" << RESET << std::endl;
        return;
    }

    std::string target = get_user_input("Output file name [default: " + name + "]:");
    std::string extension = "";
    
    if (target.find(".") == std::string::npos)
        extension = get_user_input("Output file extension [default: none]:");


    else {
        std::string temp_target = target;
        target = temp_target.substr(0, temp_target.find("."));
        extension = temp_target.substr(temp_target.find(".") + 1);
    }

    Project project(curr_dir, name, target, extension, true);
    project.create_project();
}

void Project::add_cpp_hpp() {
    std::filesystem::path src_path = path / "src";
    std::filesystem::path inc_path = path / "include";
    (std::filesystem::exists(src_path) && std::filesystem::exists(inc_path))? (void)0 : throw std::runtime_error("You must be in a project directory");

    std::string name = get_user_input("Name of the cpp/hpp pair:");
    (name.empty())? throw std::runtime_error("Name cannot be empty") : (void)0;

    (std::filesystem::exists(src_path / (name + ".cpp")) || std::filesystem::exists(inc_path / (name + ".hpp")))? throw std::runtime_error("File already exists") : (void)0;

    std::ofstream cpp(src_path / (name + ".cpp"));
    (!cpp.is_open())? throw std::runtime_error("Could not open " + name + ".cpp in " + src_path.string() + ", check permissions") : (void)0;
    cpp << "#include \"../include/" << name << ".hpp\"" << std::endl << std::endl;
    cpp.close();

    std::ofstream hpp(inc_path / (name + ".hpp"));
    (!hpp.is_open())? throw std::runtime_error("Could not open " + name + ".hpp in " + inc_path.string() + ", check permissions") : (void)0;
    hpp << "#pragma once" << std::endl << std::endl
        << "#ifndef " << name << "_HPP" << std::endl
        << "#define " << name << "_HPP" << std::endl << std::endl << std::endl << std::endl
        << "#endif" << std::endl;

    hpp.close();

    std::cout << GREEN << "Files created" << RESET << std::endl;
    std::cout << "You can now include the header file with" << BLUE << " #include \"../include/" << name << ".hpp\"" << RESET << std::endl; 
}
