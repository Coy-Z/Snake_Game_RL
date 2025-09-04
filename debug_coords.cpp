#include <iostream>
#include <vector>
#include <set>
#include <deque>

// Copy the relevant types and constants
using Position = std::vector<int>;
using PositionSet = std::set<Position>;
using PositionDeque = std::deque<Position>;

struct GameConfig {
    static constexpr int GRID_SIZE_X = 10;
    static constexpr int GRID_SIZE_Y = 10;
};

// Simple snake class for testing
class TestSnake {
public:
    TestSnake() {
        // Initialize snake with head at (0,0) and one body segment at (-1,0)
        body_positions_.push_back({0, 0});  // head
        body_positions_.push_back({-1, 0}); // body
    }
    
    bool isAtPosition(const Position& pos) const {
        for (const auto& body_pos : body_positions_) {
            if (body_pos == pos) {
                return true;
            }
        }
        return false;
    }
    
    PositionSet getAvailablePositions() const {
        PositionSet available;
        
        // Generate all possible positions on the grid
        for (int x = -GameConfig::GRID_SIZE_X / 2; x < GameConfig::GRID_SIZE_X / 2; ++x) {
            for (int y = -GameConfig::GRID_SIZE_Y / 2; y < GameConfig::GRID_SIZE_Y / 2; ++y) {
                Position pos = {x, y};
                if (!isAtPosition(pos)) {
                    available.insert(pos);
                }
            }
        }
        
        return available;
    }
    
    void printSnakePositions() const {
        std::cout << "Snake positions: ";
        for (const auto& pos : body_positions_) {
            std::cout << "(" << pos[0] << "," << pos[1] << ") ";
        }
        std::cout << std::endl;
    }
    
private:
    PositionDeque body_positions_;
};

int main() {
    TestSnake snake;
    
    std::cout << "Grid size: " << GameConfig::GRID_SIZE_X << "x" << GameConfig::GRID_SIZE_Y << std::endl;
    std::cout << "Grid range: [" << (-GameConfig::GRID_SIZE_X / 2) << ", " << (GameConfig::GRID_SIZE_X / 2 - 1) << "]" << std::endl;
    
    snake.printSnakePositions();
    
    auto available = snake.getAvailablePositions();
    std::cout << "Available positions: " << available.size() << std::endl;
    std::cout << "Total grid positions: " << (GameConfig::GRID_SIZE_X * GameConfig::GRID_SIZE_Y) << std::endl;
    std::cout << "Expected available: " << (GameConfig::GRID_SIZE_X * GameConfig::GRID_SIZE_Y - 2) << std::endl;
    
    std::cout << "\nFirst 10 available positions:" << std::endl;
    int count = 0;
    for (const auto& pos : available) {
        if (count++ >= 10) break;
        std::cout << "(" << pos[0] << "," << pos[1] << ") ";
    }
    std::cout << std::endl;
    
    return 0;
}
