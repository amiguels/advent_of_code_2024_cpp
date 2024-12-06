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

void printMap(vector<vector<char>> map)
{
    for (auto& line : map)
    {
        for (auto& c : line)
        {
            cout<<c;
        }
        cout<<endl;
    }    
}

bool isPositionInsideMap(const vector<vector<char>>& map, const size_t& x, const size_t& y)
{
    return (x < map.size()) && (y < map[0].size());
}

enum Direction {
    UP = 0,
    RIGHT,
    DOWN,
    LEFT,
    //
    MAX
};

static const string kDirectionNames[5] = {"UP", "RIGHT", "DOWN", "LEFT", "MAX"};
static constexpr char kDirectionSymbols[5] = {'^', '>', 'v', '<', 'O'};

class Guard {
public:
    size_t x{0};
    size_t y{0};
    Direction direction{Direction::MAX};

    void set(size_t x, size_t y, char direction_char)
    {
        this->x = x;
        this->y = y;

        switch (direction_char)
        {
        case kDirectionSymbols[Direction::UP]:
            this->direction = Direction::UP;
            break;

        case kDirectionSymbols[Direction::RIGHT]:
            this->direction = Direction::RIGHT;
            break;

        case kDirectionSymbols[Direction::DOWN]:
            this->direction = Direction::DOWN;
            break;

        case kDirectionSymbols[Direction::LEFT]:
            this->direction = Direction::LEFT;
            break;
        
        default:
            cout << "Bad guard direction: {" << direction_char << "}" << endl;
            break;
        }
    }

    void print()
    {
        cout << "Guard is at (" << x << ", " << y << ") and facing " << kDirectionNames[direction] << "." << endl;
    }

    bool step(const vector<vector<char>>& map)
    {
        size_t next_x = x;
        size_t next_y = y;
        
        // Get next expected position
        switch (direction)
        {
            case UP:
                next_x--;
                break;
            case DOWN:
                next_x++;
                break;
            case LEFT:
                next_y--;
                break;
            case RIGHT:
                next_y++;
                break;
            
            default:
                cout << "Bad direction." << endl;
                return 0;
                break;
        }

        // Check next position is inside map
        bool is_next_position_in_map = isPositionInsideMap(map, next_x, next_y);
        if(!is_next_position_in_map)
        {
            return false;
        }

        // Check if next position is walkable
        bool is_next_position_walkable  = (map[next_x][next_y] == '.') || (map[next_x][next_y] == 'X');
        if (is_next_position_walkable)
        {
            x = next_x;
            y = next_y;
        }
        else
        {
            // Change direction
            // cout << "Changing direction. Was " << kDirectionNames[direction];
            direction = static_cast<Direction>((direction + 1) % Direction::MAX);
            // cout << " and is now " << kDirectionNames[direction] << endl;
        }

        return true;
    }
};

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

    vector<vector<char>> map{};
    Guard guard{};
    bool guard_found{false};

    while (getline(file, input_line))
    {
        vector<char> line_chars{};

        for (auto it_c = input_line.begin(); it_c < input_line.end(); ++it_c)        
        {
            char c = *it_c;
            line_chars.push_back(c);

            if (c == '^' || c == '<' || c == '>' || c == 'v')
            {
                if( guard_found )
                {
                    cout << "Error: Multiple guards found!" << endl;
                }

                guard.set(map.size(), distance(input_line.begin(), it_c),  c);
            }
            
        }
        
        map.push_back(line_chars);
    }

    // printMap(map);
    // guard.print();

    size_t distinct_positions{1};

    while(isPositionInsideMap(map, guard.x, guard.y))
    {
        
        // std::cin.get();

        size_t start_x = guard.x;
        size_t start_y = guard.y;
        
        bool successful_step = guard.step(map);
        if (!successful_step)
        {
            break;
        }

        if (map[guard.x][guard.y] == '.')
        {
            distinct_positions++;
        }        
        
        map[start_x][start_y] = 'X';
        map[guard.x][guard.y] = kDirectionSymbols[guard.direction];

        // printMap(map);
        // guard.print();
        // cout << endl;
    }

    cout << "Guard was on " << distinct_positions << " distinct positions" << endl;

}