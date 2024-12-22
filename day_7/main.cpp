#include <iostream>
#include <fstream>
#include <cstdint>
#include <ranges>
#include <vector>
#include <set>
#include <sstream>      // istringstream
#include <cmath>
#include <algorithm>
#include <regex>
#include <iterator>
#include <numeric> //std::iota

using namespace std;

size_t calculateTotal(const vector<size_t>& numbers, size_t mask)
{
    // cout << "Mask: " << mask << "." << endl;
    size_t total{numbers[0]};

    for (size_t i = 1; i < numbers.size(); i++)
    {
        const bool op_bit = mask & 1;
        // cout << "Op bit: " << op_bit << "." << endl;
        mask = mask >> 1;

        if (op_bit == 0)
        {
            // cout << total << "+=" << numbers[i] << endl;
            total += numbers[i];
        }
        else
        {
            // cout << total << "*=" << numbers[i] << endl;
            total *= numbers[i];
        }                    
    }
    
    // cout << "Resulting total:" << total << endl;
    return total;
}


int main(int argc, char* argv[]){

    // Get input file path and open it
    if (argc != 2) {
        cerr << "Incorrect number of arguments. Usage: " << argv[0] << " <input_file_path>" << endl;
        return 1;
    }

    string input_file_path = argv[1];

    ifstream file(input_file_path);
    if (!file.is_open()) {
        cerr << "Error opening file: " << input_file_path << endl;
        return 1;
    }

    string input_line;
    size_t sum = 0;

    while (getline(file, input_line))
    {
        // cout << "Read line: {" << input_line << "}" << endl;

        const std::string split_token{":"};

        auto split_pos = input_line.find(split_token);
        if (split_pos == string::npos)
        {
            // We have read all rules.
            break;
        }

        auto expected_sum = std::stoull(input_line.substr(0, split_pos));
        (void) expected_sum;

        // Remove blank space
        split_pos += 1;

        vector<size_t> numbers{};

        std::istringstream stream(input_line.substr(split_pos + 1));
        std::string str{};
        while (stream >> str)
        {
            numbers.push_back(std::stoi(str));
        }

        // Check inputs were read correctly.
        // cout << "Expected sum: " << expected_sum << endl;
        // for (auto& number : numbers)
        // {
        //     cout << number << " ";
        // }
        // cout << endl;

        size_t number_of_operations = numbers.size() - 1;

        size_t max_combination_mask{ (1u << number_of_operations) - 1 };
        // cout << "Max combination mask:" << max_combination_mask << endl;

        std::vector<size_t> combinations(max_combination_mask + 1);
        std::iota(std::begin(combinations), std::end(combinations), 0);

        auto comparator = [&numbers, &expected_sum](const size_t mask){
            const size_t total = calculateTotal(numbers, mask);
            return total == expected_sum;
        };

        auto correct_mask = std::find_if(combinations.cbegin(), combinations.cend(), comparator);
        // cout << "Is possible ? " << (correct_mask!=combinations.end()) << endl << endl;

        if (correct_mask!=combinations.end())
        {
            sum += calculateTotal(numbers, *correct_mask);
        }
    }

    cout << "Sum:" << sum << endl;

   
}