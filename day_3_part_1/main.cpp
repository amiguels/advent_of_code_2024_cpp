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

    uint32_t sum{0};

    std::regex multiplication_regex("mul\\([0-9]{1,3},[0-9]{1,3}\\)");
    auto multiplication_words_begin = std::sregex_iterator(input.begin(), input.end(), multiplication_regex);
    auto multiplication_words_end = std::sregex_iterator();

    for (auto it_multiplication_words = multiplication_words_begin; it_multiplication_words != multiplication_words_end; ++it_multiplication_words)
    {
        std::string multiplication_word = (*it_multiplication_words).str();
        // std::cout << multiplication_word << std::endl;

        std::regex numbers_regex("[0-9]{1,3}");
        auto numbers_words_begin = std::sregex_iterator(multiplication_word.begin(), multiplication_word.end(), numbers_regex);
        
        uint32_t one = std::stoi((*numbers_words_begin).str());
        uint32_t two = std::stoi((*(std::next(numbers_words_begin))).str());

        sum += one *  two;
    }
    

    std::cout << "The total sum is " << sum << "." << std::endl;

}