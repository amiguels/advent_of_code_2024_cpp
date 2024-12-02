#include <iostream>
#include <fstream>
#include <cstdint>
#include <ranges>
#include <vector>
#include <sstream>      // std::istringstream
#include <cmath>
#include <algorithm>

bool check(std::vector<int32_t>& levels){
    const bool direction{std::signbit(levels.at(0) - levels.at(1))}; 
    
    for (auto it = levels.begin(); it < levels.end() - 1; it++ )
    {
        const int32_t delta = (*it) - (*(it+1));

        if( direction != std::signbit(delta) )
        {
            // std::cout << "Direction check failed at {" << this_level << "} and {" << next_level << "}. Expected " << direction << " but got " << next_direction << "." << std::endl;
            return false;
        }

        const int32_t abs_delta = std::abs(delta);

        if( abs_delta < 1 || abs_delta > 3)
        {
            // std::cout << "Delta check failed at {" << this_level << "} and {" << next_level << "}: " << abs_delta << "." << std::endl;
            return false;
        }
    }

    return true;
}

bool check_reduced(std::vector<int32_t>& levels){
    for (uint32_t index_to_avoid = 0; index_to_avoid < levels.size() ; index_to_avoid++)
    {
        std::vector<int32_t> reduced_vector{levels};
        reduced_vector.erase(reduced_vector.begin() + index_to_avoid);

        if (check(reduced_vector))
        {
            return true;
        }
    }

    return false;
}

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
    std::vector<int32_t> levels{}; 

    uint32_t ok_count{0};
    uint32_t ok_reduced_count{0};

    while (std::getline(file, input_line)) {
        levels.clear();
        std::istringstream stream(input_line);

        std::string str{};
        while (stream >> str)
        {
            levels.push_back(std::stoi(str));
        }

        // This loop is only for easy benchmarking.
        for (size_t i = 0; i < 1; i++)
        {
            bool check_is_ok{check(levels)};
            ok_count += check_is_ok;

            if (not check_is_ok)
            {
                check_is_ok = check_reduced(levels);
            }
            
            ok_reduced_count += check_is_ok;
            
        } 
    }

    std::cout << "Counted " << ok_count << " good reports." << std::endl;  
    std::cout << "Counted " << ok_reduced_count << " good reduced reports." << std::endl;  
    
}