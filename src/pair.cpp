#include "../include/pair.hpp"
#include "../include/colors.hpp"
#include "../include/config_reader.hpp"

collection get_pairs() {
    collection pairs_collection;
    config::ConfigFile config;
    config.load_from_file(PAIR_FILE);

    std::vector<std::string> pairs = config.list_configs();

    for (auto pair : pairs) {
        Pair p;
        std::map<std::string, std::string> pair_config = config.to_map(pair);
        
        p.name = pair;
        p.version = pair_config["version"];
        p.description = pair_config["description"];
        p.include = pair_config["include"];
        p.source = pair_config["source"];
        p.example = pair_config["example"];
        p.library = pair_config["library"];

        pairs_collection.push_back(p);
    }
    return pairs_collection;
}

void display_pairs(collection pairs) {
    std::cout << BLUE << "Here is a list of available pairs:" << RESET << std::endl;
    for (auto pair : pairs) {
        std::cout << std::endl;
        std::cout << GREEN << pair.name << RESET << std::endl;
        std::cout << "Version: " << pair.version << std::endl;
        std::cout << "Description: " << pair.description << std::endl;
    }
}

void download_pair(std::string pair_name) {
    // check if the user is has a include, src, and example directory
    if (!std::filesystem::exists("include") || !std::filesystem::exists("src")) {
        std::cerr << RED << "You are not currently in a project directory." << RESET << std::endl;
        exit(1);
    }


    if (!std::filesystem::exists("example"))
        std::filesystem::create_directory("example");

    collection pairs = get_pairs();

    for (auto pair : pairs) {
        if (pair.name == pair_name) {
            std::cout << "Downloading pair " << pair_name << "..." << std::endl;
            // if the pair property is none, then we don't need to download anything
            if (pair.include != "none") download_from_internet(pair.include, std::filesystem::current_path() / "include" / (pair.name + ".hpp"));
            if (pair.source != "none") download_from_internet(pair.source, std::filesystem::current_path() / "src" / (pair.name + ".cpp"));
            if (pair.example != "none") download_from_internet(pair.example, std::filesystem::current_path() / "example" / (pair.name + ".cpp"));
            std::cout << GREEN << "Pair " << pair_name << " downloaded successfully." << RESET << std::endl;
            std::cout << "To use " << pair.name << ", add the following line to the top of your source file:" << std::endl;
            std::cout << YELLOW << "\t#include \"../include/" << pair.name << ".hpp\"" << RESET << std::endl;

            if (pair.library != "none") {
                std::cout << YELLOW << "You will need to add the following library (LDLIBS) to your makefile:" << RESET << std::endl;
                std::cout << YELLOW << "\t" << pair.library << RESET << std::endl;
            }
            return;
        }
    }
    std::cerr << RED << "Error: Could not find pair " << pair_name << RESET << std::endl;
    exit(1);
}