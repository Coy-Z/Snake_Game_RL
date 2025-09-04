#include <iostream>
#include <vector>

using Position = std::vector<int>;

struct GameConfig {
    static constexpr int GRID_SIZE_X = 10;
    static constexpr int GRID_SIZE_Y = 10;
};

Position wrapPosition(const Position& pos) {
    if (pos.size() < 2) {
        return pos;
    }
    
    Position wrapped = pos;
    
    // Wrap X coordinate
    if (wrapped[0] < -GameConfig::GRID_SIZE_X / 2) {
        wrapped[0] = GameConfig::GRID_SIZE_X / 2 - 1;
    } else if (wrapped[0] >= GameConfig::GRID_SIZE_X / 2) {
        wrapped[0] = -GameConfig::GRID_SIZE_X / 2;
    }
    
    // Wrap Y coordinate
    if (wrapped[1] < -GameConfig::GRID_SIZE_Y / 2) {
        wrapped[1] = GameConfig::GRID_SIZE_Y / 2 - 1;
    } else if (wrapped[1] >= GameConfig::GRID_SIZE_Y / 2) {
        wrapped[1] = -GameConfig::GRID_SIZE_Y / 2;
    }
    
    return wrapped;
}

Position calculateNextPosition(const Position& current, int dir) {
    if (current.size() < 2) {
        return current;
    }
    
    Position next = current;
    
    switch (dir) {
        case 0: // UP
            next[1]++;
            break;
        case 1: // DOWN
            next[1]--;
            break;
        case 2: // LEFT
            next[0]--;
            break;
        case 3: // RIGHT
            next[0]++;
            break;
    }
    
    return next;
}

int main() {
    std::cout << "Grid size: " << GameConfig::GRID_SIZE_X << "x" << GameConfig::GRID_SIZE_Y << std::endl;
    std::cout << "Valid range: [" << (-GameConfig::GRID_SIZE_X / 2) << ", " << (GameConfig::GRID_SIZE_X / 2 - 1) << "]" << std::endl;
    
    // Test right boundary wrapping
    std::cout << "\nTesting right boundary wrapping:" << std::endl;
    for (int x = 2; x <= 6; ++x) {
        Position pos = {x, 0};
        Position next = calculateNextPosition(pos, 3); // RIGHT
        Position wrapped = wrapPosition(next);
        
        std::cout << "Position (" << pos[0] << ",0) -> move RIGHT -> (" 
                  << next[0] << ",0) -> wrap -> (" << wrapped[0] << ",0)" << std::endl;
    }
    
    // Test left boundary wrapping
    std::cout << "\nTesting left boundary wrapping:" << std::endl;
    for (int x = -3; x >= -7; --x) {
        Position pos = {x, 0};
        Position next = calculateNextPosition(pos, 2); // LEFT
        Position wrapped = wrapPosition(next);
        
        std::cout << "Position (" << pos[0] << ",0) -> move LEFT -> (" 
                  << next[0] << ",0) -> wrap -> (" << wrapped[0] << ",0)" << std::endl;
    }
    
    return 0;
}
