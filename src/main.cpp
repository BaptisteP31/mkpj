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
#include "../include/pair.hpp"

#include <bits/stdc++.h>
#include <ncurses.h>

#define VERSION "0.6"

void tarball_export(std::string project_name) {
    config::ConfigFile config;
    config.load_from_file(std::filesystem::current_path() / ".mkpj.conf");
    if (!config.is_loaded()) {
        std::cerr << RED << "Error: Could not load config file, make sure you are in a project directory" << RESET << std::endl;
            exit(1);
    }

    Project project = config.get_project_info();

    std::string command = "tar -czf " 
        + project.name + ".tar.gz " + "src/ " + "include/ " + "Makefile " + "README.md" 
        + (project.is_licensed ? " LICENSE " : " ") 
        + (project.is_QT ? ("resources/ interface/ config/ " + project.name + ".pro ") : "") 
        + project.additional_files;
    
    // std::cerr << command << std::endl;
    // If the last command is successful, the program will print a message and exit.
    if (system(command.c_str()) == 0) {
        std::cout << GREEN << "The project has been exported to " << project.name << ".tar.gz" << RESET << std::endl;
        exit(0);
    }
    else {
        std::cerr << RED << "Error: Could not export the project" << RESET << std::endl;
        exit(1);
    }
}

void ncurses_menu(char **argv) {
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);

    // The program prints the title and the choices.
    
    mvprintw(0, 0, "MakeProject - A simple project creator");

    // Check if the user is in a project directory (basically if there is a .mkpj.conf file)
    if (!std::filesystem::exists(".mkpj.conf"))
        mvprintw(1, 0, "You are not in a project directory.");
    else
        {
            config::ConfigFile config;
            config.load_from_file(".mkpj.conf");

            std::string project_name = config.get_project_info().name;

            mvprintw(1, 0, "You are in the project: %s", project_name.c_str());
        }
    

    // There is a list of choices and the user can navigate up and down and make a choice by pressing enter.
    // The program will print the choice and exit.

    // printed in the color blue
    attron(COLOR_PAIR(1));
    mvprintw(3, 0, "Choose an option:");
    attroff(COLOR_PAIR(1));
    std::vector<std::string> choices;

    // If the user is not in a project directory, all he can do is create a project, see the version and help and exit.
    if (!std::filesystem::exists(".mkpj.conf")) {
        choices.push_back("Create a project");
        choices.push_back("See the version");
        choices.push_back("See the help");
        choices.push_back("Exit");
    }

    // If the user is in a project directory, he can add a cpp/hpp file, add a pair from the pair list and list the pairs, regen the makefile and expoa the project and exit.
    else {
        choices.push_back("Add a cpp/hpp file");
        choices.push_back("Add a pair from the pair list");
        choices.push_back("List all the available pairs");
        choices.push_back("Regen the makefile");
        choices.push_back("Export the project (tar.gz)");
        choices.push_back("Exit");
    }

    //choices.push_back("Create a project");
    //choices.push_back("Add a cpp/hpp file");
    //choices.push_back("Add a pair from the pair list");
    //choices.push_back("List all the available pairs");

    int     highlight = 0;
    int     choice = 0;
    int     c;

    // The loop will run until the user presses enter to make a choice.
    // The user can also use the up and down arrow keys to navigate the menu.
    // The program will refresh the screen for each input.
    // The loop will break when the user makes a choice.

    while(1) {
        for(size_t i = 0; i < choices.size(); i++) {
            if(i == size_t(highlight))
                attron(A_REVERSE);
            mvprintw(i+4, 1, choices[i].c_str());
            attroff(A_REVERSE);
        }
        c = getch();
        switch(c) {
            case KEY_UP:
                if(highlight == 0)
                    highlight = choices.size() - 1;
                else
                    highlight--;
                break;
            case KEY_DOWN:
                if(size_t(highlight) == choices.size() - 1)
                    highlight = 0;
                else
                    highlight++;
                break;
            case 10: // Enter
                choice = highlight + 1;
                break;           
            case 'q':
                endwin();
                clear();
                exit(0);
                break;
            default:
                break;
        }
        if(choice)
            break;
    }

    refresh();
    endwin();
    
    // sane the stty settings
    system("stty sane"); 

    // The program will call the function corresponding to the choice.
    std::string pair = "";
    
    collection pairs;

    // If the user is not in a project directory, all he can do is create a project, see the version and help and exit.
    if (!std::filesystem::exists(std::filesystem::current_path() / ".mkpj.conf")) {
        std::string help = "";
        std::string version = "";
        switch(choice) {
            case 1:
                create_project();
                break;
            case 2:
                version = " --version"; // The program will call itself the version function
                version = argv[0] + version;
                system(version.c_str());
                exit(0);
                break;
            case 3:
                help = " --help"; // The program will call itself the help function
                help = argv[0] + help;
                system(help.c_str());
                exit(0);
                break;
            case 4:
                exit(0);
                break;
            default:
                break;
        
        return;
        }
    }

    // If the user is currently in a project directory
    if(std::filesystem::exists(std::filesystem::current_path() / ".mkpj.conf")) {
        collection pairs;
        config::ConfigFile config;
        switch(choice) {
            case 1:
                add_cpp_hpp();
                break;
            case 2:
                std::cout << "Enter the pair you want to add: ";
                getline(std::cin, pair);
                download_pair(pair);
                break;
            case 3:
                pairs = get_pairs();           
                display_pairs(pairs);
                break;
            case 4:
                config.load_from_file(".mkpj.conf");
                create_makefile(config.get_project_info());
                std::cout << GREEN << "The makefile has been regenerated." << RESET << std::endl;
                break;
            case 5:
                config.load_from_file(".mkpj.conf");
                tarball_export(config.get_project_info().name);
                break;
            case 6:
                exit(0);
                break;
            default:
                break;
        }
    }   
}


int main(int argc, char **argv) {
    cxxopts::Options options("mkpj", "MakeProject - A simple project creator");
    options.add_options()
        // The user can use the -c with or without a project name.
        // If the user doesn't specify a project name, the program will ask for one.
        ("create", "Creates the project", cxxopts::value<std::string>()->default_value("pr")) //! this needs to be fixed
        ("add", "Adds a cpp/hpp file to the project")
        ("l,list", "Lists all the available pairs")
        ("u,update", "Updates the pair list")
        ("p, pairs", "Adds a pair from the pair list", cxxopts::value<std::string>())
        ("d,debug", "Prints the debug information")
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

        else if (result.count("debug")) {
            config::ConfigFile config;
            config.load_from_file(".mkpj.conf");
            auto configs = config.to_vector();
            for (auto config : configs) {
                std::cout << config.first << std::endl;
                for (auto key_value : config.second) {
                    std::cout << "\t" << key_value.first << " : " << key_value.second << std::endl;
                }
            }
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

        else if (result.count("create") ) {
            std::cerr << YELLOW << "you passed the create option " << result["create"].as<std::string>() << RESET << std::endl;
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
            config::ConfigFile config;
            config.load_from_file(std::filesystem::current_path() / ".mkpj.conf");
            if (!config.is_loaded()) {
                std::cerr << RED << "Error: Could not load config file, make sure you are in a project directory" << RESET << std::endl;
                exit(1);
            }
            update_pairs();
            std::cout << GREEN << "Pairs updated" << RESET << std::endl;
        }

        else if (result.count("pairs")) {
            std::string pair_name = result["pairs"].as<std::string>();
            download_pair(pair_name);        
        }

        else if (result.count("makefile")) {
            config::ConfigFile config;
            config.load_from_file(std::filesystem::current_path() / ".mkpj.conf");

            if (!config.is_loaded()) {
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
            config::ConfigFile config;
            config.load_from_file(".mkpj.conf");

            if (!config.is_loaded()) {
                std::cerr << RED << "Error: Could not load config file, make sure you are in a project directory" << RESET << std::endl;
                exit(1);
            }
            tarball_export(config.get_project_info().name);
        }

        else {
            ncurses_menu(argv);
            clear();
        }

    } catch (const cxxopts::OptionException& e) {
        std::cout << options.help() << std::endl;
        exit(0);
    }

    return 0;
}
