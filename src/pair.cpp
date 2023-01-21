#include "../include/pair.hpp"
#include "../include/colors.hpp"

collection get_pairs() {
    collection pairs;
    std::ifstream file(PAIR_FILE);

    if (!file.is_open()) {
        std::cerr << RED << "Error: Could not open " << PAIR_FILE << RESET << std::endl;
        exit(1);
    }
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string key, value;
        std::getline(ss, key, '=');
        std::getline(ss, value, '=');

        if (!value.empty() && value[0] == ' ') {
            value.erase(0, 1);
        }

        if (key == "name ") {
            pairs.push_back(Pair());
            pairs.back().name = value;
        }

        else if (key == "version ") {
            pairs.back().version = value;
        }

        else if (key == "description ") {
            pairs.back().description = value;
        }

        else if (key == "include ") {
            value.erase(std::remove(value.begin(), value.end(), ' '), value.end());
            pairs.back().include = value;
        }

        else if (key == "source ") {
            value.erase(std::remove(value.begin(), value.end(), ' '), value.end());
            pairs.back().source = value;
        }

        else if (key == "example ") {
            pairs.back().example = value;
        }

        else if (key == "library ") {
            pairs.back().library = value;
        }

    }

    return pairs;
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

    std::cout << "Downloading pair " << pair_name << "..." << std::endl;

    if (!std::filesystem::exists("example"))
        std::filesystem::create_directory("example");

    collection pairs = get_pairs();

    for (auto pair : pairs) {
        if (pair.name == pair_name) {
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