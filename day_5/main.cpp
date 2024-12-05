#include <iostream>
#include <fstream>
#include <cstdint>
#include <ranges>
#include <vector>
#include <sstream>      // istringstream
#include <cmath>
#include <algorithm>
#include <regex>

using namespace std;

map<size_t, vector<size_t>> ordering_rules{};

void printOrderingRules(map<size_t, vector<size_t>>& rules){
    for (auto const& [key, vals] : rules)
    {
        cout << key << " | ";

        for (auto& val : vals)
        {
            cout << val << ", ";
        }

        cout << endl;
    }
}

class PageNumber {
    public:
    size_t number;

    PageNumber(size_t number) : number(number){};
};

bool operator<(const PageNumber& lhs, const PageNumber& rhs) {

    auto second_rule = ordering_rules.find(rhs.number);
    if (second_rule == ordering_rules.end())
    {
        // Rule not found. Order is ok.
        return true;
    }

    auto first = std::find(second_rule->second.begin(), second_rule->second.end(), lhs.number);
    if( first == second_rule->second.end())
    {
        // Did not find. Order is ok.
        return true;
    }
    else
    {
        // Found. Order is not ok.
        return false;;
    }
}

bool operator==(const PageNumber& lhs, const PageNumber& rhs) {
    return lhs.number == rhs.number;
}

void printManualPages(vector<PageNumber>& manual_pages)
{
    for (auto& page : manual_pages)
    {
        cout << page.number << ", ";
    }
    cout << endl;    
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

    while (getline(file, input_line))
    {
        // cout << input_line << endl;

        const std::string split_token{"|"};

        auto split_pos = input_line.find(split_token);
        if (split_pos == string::npos)
        {
            // We have read all rules.
            break;
        }

        auto first = std::stoi(input_line.substr(0, split_pos));
        auto second = std::stoi(input_line.substr(split_pos + 1));

        if (!ordering_rules.contains(first))
        {
            ordering_rules[first] = vector<size_t>{};
        }
        
        ordering_rules[first].push_back(second);
    }

    // printOrderingRules(ordering_rules);

    vector<vector<PageNumber>> manuals_pages{};

    while (getline(file, input_line))
    {
        // cout << input_line << endl;

        std::vector<PageNumber> numbers;
        std::stringstream sstream(input_line);
        std::string token;

        // Parse the string using comma as a delimiter
        while (std::getline(sstream, token, ',')) {
            numbers.push_back(PageNumber(std::stoi(token)));
        }

        manuals_pages.push_back(numbers);        
    }

    size_t sum_of_originally_ordered_middle_values{0};
    size_t sum_of_originally_non_ordered_middle_values{0};

    for (auto& manual_page_numbers : manuals_pages)
    {
        // printManualPages(manual_page_numbers);

        vector<PageNumber> new_manual_page_numbers(manual_page_numbers);
        std::sort(new_manual_page_numbers.begin(), new_manual_page_numbers.end());

        // printManualPages(new_manual_page_numbers);

        size_t middle_idx = new_manual_page_numbers.size() / 2;
        size_t middle_value = new_manual_page_numbers.at(middle_idx).number;

        if(manual_page_numbers == new_manual_page_numbers)
        {
            // Was originally ordered;            
            sum_of_originally_ordered_middle_values += middle_value;
        }
        else
        {
            // Was originally ordered;            
            sum_of_originally_non_ordered_middle_values += middle_value;
        }

        cout << endl;


    }

    cout << sum_of_originally_ordered_middle_values << endl;
    cout << sum_of_originally_non_ordered_middle_values << endl;

}