#include <iostream>
#include <fstream>
#include <cstdint>
#include <ranges>
#include <vector>
#include <map>
#include <algorithm>
#include <execution>
#include <numeric> // For std::accumulate

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

    // Read input file contents
    std::vector<int32_t> lhs_vector{};
    std::vector<int32_t> rhs_vector{};
    
    std::string input_line;
    while (std::getline(file, input_line)) {
        const std::string split_token{"   "};

        auto split_start = input_line.find(split_token);
        auto split_end = split_start + split_token.length();
        
        lhs_vector.push_back(std::stoi(input_line.substr(0, split_start)));
        rhs_vector.push_back(std::stoi(input_line.substr(split_end, input_line.length() - split_end)));
    }

    // Part 1 --------------------------------------------------------------------------------------------------
    
    // Sort both lists
    std::sort(lhs_vector.begin(), lhs_vector.end());
    std::sort(rhs_vector.begin(), rhs_vector.end());

    // Lambda that gets the absolute difference.
    auto abs_diff = [](auto& elements) {
        auto& [lhs_element, rhs_element] = elements;
        return abs(lhs_element - rhs_element);
    };
 
    // Zip that hold references to both input vectors, plus another vector to hold the outputs.
    auto elements_zip = std::views::zip(lhs_vector, rhs_vector);
    std::vector<int32_t> differences(lhs_vector.size());

    // Calculate the differences for each element.
    std::transform(std::execution::par, elements_zip.begin(), elements_zip.end(), differences.begin(), abs_diff);

    // Sum the differences.
    auto list_distance{std::accumulate(differences.begin(), differences.end(), 0)};
    
    std::cout << "The distance between lists is " << list_distance << "." << std::endl;
    // Expected for test input: 11.
    // Expected for full input: 2057374.


    // Part 2 --------------------------------------------------------------------------------------------------    

    // Lambda to calculate the score of each element.
    // I tried pre-calculating the number of occurrence, so it was only done once for each value,
    // but for the amount of duplicated values in the given input, it proved to not be wort it.
    // The pre-calculation plus storing the results took longer than calculating the number of occurrences every time.
    auto calculate_score = [&rhs_vector](auto lhs_element) { return lhs_element * std::count(rhs_vector.begin(), rhs_vector.end(), lhs_element); };

    // Apply the lambda to each element and save in the score results vector.
    std::vector<int32_t> scores(lhs_vector.size());

    std::transform(std::execution::par, lhs_vector.begin(), lhs_vector.end(), scores.begin(), calculate_score);

    // Sum the scores.
    int32_t list_similarity{std::accumulate(scores.begin(), scores.end(), 0)};

    std::cout << "The similarity between lists is " << list_similarity << "." << std::endl;
    // Expected for test input: 31.
    // Expected for full input: 23177084.
    
}