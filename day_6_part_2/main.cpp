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
static constexpr char kDirectionSymbols[5] = {'^', '>', 'v', '<'};

enum ExitReason {
    ERROR = 0,
    TURNED,
    STEPPED,
    OUT_OF_MAP,
    LOOP
};

class GuardPosition {
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

    ExitReason step(const vector<vector<char>>& map)
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
                return ExitReason::ERROR;
                break;
        }

        // Check next position is inside map
        bool is_next_position_in_map = isPositionInsideMap(map, next_x, next_y);
        if(!is_next_position_in_map)
        {
            return ExitReason::OUT_OF_MAP;
        }
        
        // Check if next position is walkable
        bool is_next_position_walkable =
            (map[next_x][next_y] == '.') ||
            (map[next_x][next_y] == 'X') ||
            (map[next_x][next_y] == kDirectionSymbols[Direction::UP]) ||
            (map[next_x][next_y] == kDirectionSymbols[Direction::RIGHT]) ||
            (map[next_x][next_y] == kDirectionSymbols[Direction::DOWN]) ||
            (map[next_x][next_y] == kDirectionSymbols[Direction::LEFT]);

        if (is_next_position_walkable)
        {
            x = next_x;
            y = next_y;
        }
        else
        {
            direction = static_cast<Direction>((direction + 1) % Direction::MAX);
        }
        
        return ExitReason::STEPPED;
    }
};

bool operator==(const GuardPosition& lhs, const GuardPosition& rhs)
{
    return std::tie(lhs.x, lhs.y, lhs.direction) == std::tie(rhs.x, rhs.y, rhs.direction);
}

bool operator<(const GuardPosition& lhs, const GuardPosition& rhs)
{
    return std::tie(lhs.x, lhs.y, lhs.direction) < std::tie(rhs.x, rhs.y, rhs.direction);
}

ExitReason solveMap(const vector<vector<char>>& map, GuardPosition guard_position, std::optional<std::set<GuardPosition>>& position_storage)
{ 
    GuardPosition initial_guard_position{guard_position};

    std::set<GuardPosition> previous_positions{};
    previous_positions.insert(initial_guard_position);

    while(isPositionInsideMap(map, guard_position.x, guard_position.y))
    {        
        ExitReason step_result = guard_position.step(map);
        if ((step_result == ExitReason::ERROR) || (step_result == ExitReason::OUT_OF_MAP))
        {
            if (position_storage.has_value())
            {
                previous_positions.erase(initial_guard_position);
                position_storage.value().insert(previous_positions.begin(), previous_positions.end());
            }
            
            return step_result;
        }

        if(previous_positions.contains(guard_position))
        {
            return ExitReason::LOOP;
        }

        previous_positions.insert(guard_position);
    }

    if (position_storage.has_value())
    {
        previous_positions.erase(initial_guard_position);
        position_storage.value().insert(previous_positions.begin(), previous_positions.end());
    }
    return ExitReason::OUT_OF_MAP;
}

struct CartesianCoords{
    size_t x;
    size_t y;
};

bool operator<(const CartesianCoords& lhs, const CartesianCoords& rhs)
{
    return std::tie(lhs.x, lhs.y) < std::tie(rhs.x, rhs.y);
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

    vector<vector<char>> map{};
    GuardPosition guard_position{};
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

                guard_position.set(map.size(), distance(input_line.begin(), it_c),  c);
            }
            
        }
        
        map.push_back(line_chars);
    }

    std::optional<set<GuardPosition>> free_walk_positions_opt{set<GuardPosition>{}};
    solveMap(map, guard_position, free_walk_positions_opt);

    set<CartesianCoords> coords_to_try{};
    transform(free_walk_positions_opt.value().begin(), free_walk_positions_opt.value().end(), std::inserter(coords_to_try, coords_to_try.begin()),
        [](auto& guard_position){
            return CartesianCoords(guard_position.x, guard_position.y);
        });

    double effective_obstacle_placements{0};    

    std::optional<set<GuardPosition>> empty_optional;

    for (auto& obstacle_position_to_try : coords_to_try)
    {
        map[obstacle_position_to_try.x][obstacle_position_to_try.y] = 'O';

        ExitReason exit_reason = solveMap(map, guard_position, empty_optional);
        if (exit_reason == ExitReason::LOOP)
        {
            effective_obstacle_placements++;
        }

        map[obstacle_position_to_try.x][obstacle_position_to_try.y] = '.';
    }

    cout << "Possible obstacle placements: " << effective_obstacle_placements << endl; 
   
}