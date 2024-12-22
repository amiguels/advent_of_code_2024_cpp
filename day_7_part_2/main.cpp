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

enum Operation { Multiply, Sum, Concat };

void generateOpCombinations(int n, const vector<Operation>& possible_ops, vector<Operation>& current, vector<vector<Operation>>& result) {
    if (current.size() == n) {
        result.push_back(current);
        return;
    }

    for (Operation op : possible_ops) {
        current.push_back(op);
        generateOpCombinations(n, possible_ops, current, result);
        current.pop_back();
    }
}

size_t calculateTotal(const vector<size_t>& numbers, vector<Operation> operations)
{
    // cout << "Mask: " << mask << "." << endl;
    size_t total{numbers[0]};

    for (size_t i = 1; i < numbers.size(); i++)
    {
        switch (operations[i - 1])
        {
        case Operation::Multiply:
            total *= numbers[i];
            break;

        case Operation::Sum:
            total += numbers[i];
            break;

        case Operation::Concat:
            total = stoull(to_string(total) + to_string(numbers[i]));
            break;
        
        default:
            break;
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

        // Get all possible combinations
        vector<Operation> possible_operations = {Multiply, Sum, Concat};
        vector<vector<Operation>> operations_to_try;
        vector<Operation> current;

        generateOpCombinations(number_of_operations, possible_operations, current, operations_to_try);
    
        auto comparator = [&numbers, &expected_sum](const auto operations){
            const size_t total = calculateTotal(numbers, operations);
            return total == expected_sum;
        };

        auto correct_mask = std::find_if(operations_to_try.cbegin(), operations_to_try.cend(), comparator);

        if (correct_mask!=operations_to_try.end())
        {
            sum += calculateTotal(numbers, *correct_mask);
        }
    }

    cout << "Sum:" << sum << endl;

   
}