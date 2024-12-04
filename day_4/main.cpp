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

// static constexpr char XMAS[] = {'X', 'M', 'A', 'S'};
// static constexpr size_t LAST_CHAR_INDEX = (sizeof(XMAS) / sizeof(char))-1;

// size_t countOccurrencesInLine(vector<char>::iterator it, vector<char>::iterator end_it, size_t char_idx)
// {
//     size_t count{0};

//     if (it == end_it)
//     {
//         return count;
//     }
    
//     while(it != end_it){
//         // cout << "Looking for " << XMAS[char_idx] << " starting at idx " << distance(line.begin(), it) << endl;
//         it = find(it, end_it, XMAS[char_idx]);

//         if (char_idx == LAST_CHAR_INDEX)
//         {
//             if( it != end_it )
//             {
//                 // cout << "Found one!" << endl;
//                 it++;
//                 count++;
//             }
//             else
//             {
//                 return count;
//             }
//         }
//         else
//         {
//             if( it != end_it )
//             {
//                 count += countOccurrencesInLine(it+1, end_it, char_idx+1);
//                 it++;
//             }
//             else
//             {
//                 return count;
//             }
//         }
//     }

//     return count;
// }


static const vector<char> XMAS = {'X', 'M', 'A', 'S'};

size_t countOccurrencesInLine(vector<char>& line)
{
    size_t count{0};
    
    auto it = line.begin();

    while(it != line.end()){
       
        it = std::search(it, line.end(), XMAS.begin(), XMAS.end());
        if (it != line.end())
        {
            count++;
            it += XMAS.size();
        }
    }

    return count;
}

size_t countOccurrencesInDatabase(vector<vector<char>> vec_of_lines){
    size_t count{0};

    for (auto& line : vec_of_lines)
    {      
        size_t new_count = countOccurrencesInLine(line);
        // cout << "Found " << new_count << endl;
        count += new_count; 

        auto reversed_line = vector<char>(line.rbegin(), line.rend());
        new_count = countOccurrencesInLine(reversed_line);
        // cout << "Found " << new_count << endl;
        count += new_count; 
    }

    return count;
}

vector<vector<char>> getVerticalMatrixFrom(vector<vector<char>> matrix)
{
    vector<vector<char>> new_matrix{};
    
    for (size_t i = 0; i < matrix.size(); ++i) {
        new_matrix.push_back({});

        for (size_t j = 0; j < matrix[i].size(); ++j) {
            new_matrix[new_matrix.size()-1].push_back(matrix[j][i]);
        }
    }

    return new_matrix;
}

void printMatrix(vector<vector<char>> matrix)
{
    for( auto& line : matrix){
        for ( auto& c : line)
        {
            cout << c;
        }

        cout << endl;        
    }
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

    // printMatrix(chars);
    count += countOccurrencesInDatabase(chars);
    cout << endl;

    auto vertical_chars = getVerticalMatrixFrom(chars);
    // printMatrix(vertical_chars);
    count += countOccurrencesInDatabase(vertical_chars);
    cout << endl;

    cout << "Count: " << count << endl;

}