#include <iostream>
#include <fstream>
#include <cstdint>
#include <ranges>
#include <vector>
#include <sstream>      // std::istringstream
#include <cmath>
#include <algorithm>
#include <regex>

int main(int argc, char* argv[]){

    // Get input file path and open it
    if (argc != 2) {
        std::cerr << "Incorrect number of arguments. Usage: " << argv[0] << " <input_file_path>" << std::endl;
        return 1;
    }

    std::string input_file_path = argv[1];

    std::ifstream file(input_file_path);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << input_file_path << std::endl;
        return 1;
    }

    std::string input_line;
    std::string input{};
    while (std::getline(file, input_line))
    {
        input += input_line;
    }

    std::regex commands_regex("mul\\([0-9]{1,3},[0-9]{1,3}\\)|do\\(\\)|don't\\(\\)");
    std::regex do_regex("do\\(\\)");
    std::regex dont_regex("don't\\(\\)");
    std::regex numbers_regex("[0-9]{1,3}");

    auto command_words_begin = std::sregex_iterator(input.begin(), input.end(), commands_regex);
    auto commands_words_end = std::sregex_iterator();

    bool enable{true};
    uint32_t sum{0};

    for (auto it_commands_words = command_words_begin; it_commands_words != commands_words_end; ++it_commands_words)
    {
        std::string commands_word = (*it_commands_words).str();
        // std::cout << commands_word << std::endl;

        if(std::regex_match(commands_word, do_regex))
        {
            enable = true;
            // std::cout << "-----> Enable." << std::endl;
            continue;
        }

        if(std::regex_match(commands_word, dont_regex))
        {
            enable = false;
            // std::cout << "-----> Disable." << std::endl;
            continue;
        }

        if(enable){
            std::regex numbers_regex("[0-9]{1,3}");
            auto numbers_words_begin = std::sregex_iterator(commands_word.begin(), commands_word.end(), numbers_regex);
            
            uint32_t one = std::stoi((*numbers_words_begin).str());
            uint32_t two = std::stoi((*(std::next(numbers_words_begin))).str());

            // std::cout << "-----> Add " << one << " * " << two << "." << std::endl;

            sum += one *  two;
        }
    }    

    std::cout << "The total sum is " << sum << "." << std::endl;

}