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

static constexpr char XMAS[] = {'X', 'M', 'A', 'S'};
static constexpr size_t LAST_CHAR_INDEX = (sizeof(XMAS) / sizeof(char))-1;


size_t countOccurrences(vector<char>& line, vector<char>::iterator it, size_t char_idx)
{
    size_t count{0};

    if (it == line.end())
    {
        return count;
    }
    
    while(it != line.end()){
        // cout << "Looking for " << XMAS[char_idx] << " starting at idx " << distance(line.begin(), it) << endl;
        it = find(it, line.end(), XMAS[char_idx]);

        if (char_idx == LAST_CHAR_INDEX)
        {
            if( it != line.end() )
            {
                // cout << "Found one!" << endl;
                it++;
                count++;
            }
            else
            {
                return count;
            }
        }
        else
        {
            if( it != line.end() )
            {
                count += countOccurrences(line, it+1, char_idx+1);
                it++;
            }
            else
            {
                return count;
            }
        }
    }

    return count;
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
    
    vector<vector<char>> chars{};

    while (getline(file, input_line))
    {
        vector<char> line_chars{};

        for (auto& c : input_line)
        {
            line_chars.push_back(c);
        }
        
        chars.push_back(line_chars);
    }

    size_t count{0};

    for (auto& line : chars)
    {
        for (auto& c : line)
        {
            cout << c;
        }
        cout << endl;

        size_t new_count = countOccurrences(line, line.begin(), 0);
        cout << "Found " << new_count << endl;
        count += new_count; 
    }
    
    cout << "Count: " << count << endl;

}