#include <iostream>

struct GameConfig {
    static constexpr int GRID_SIZE_X = 10;
    static constexpr int GRID_SIZE_Y = 10;
};

double convertCoordinate(int pos) {
    // New formula
    return ((static_cast<double>(pos) - (-GameConfig::GRID_SIZE_X / 2)) / (GameConfig::GRID_SIZE_X - 1)) * 1.8 - 0.9;
}

double oldConvertCoordinate(int pos) {
    // Old formula
    return (static_cast<double>(pos) / (GameConfig::GRID_SIZE_X / 2.0)) * 0.9;
}

int main() {
    std::cout << "Coordinate conversion test (game coordinate -> screen coordinate):\n" << std::endl;
    
    std::cout << "Position | Old Formula | New Formula" << std::endl;
    std::cout << "---------|-------------|------------" << std::endl;
    
    for (int pos = -5; pos <= 4; ++pos) {
        double old_screen = oldConvertCoordinate(pos);
        double new_screen = convertCoordinate(pos);
        std::cout << "   " << pos << "     |    " << old_screen << "    |    " << new_screen << std::endl;
    }
    
    std::cout << "\nExpected mapping:" << std::endl;
    std::cout << "Position -5 should map to -0.9 (leftmost)" << std::endl;
    std::cout << "Position  4 should map to  0.9 (rightmost)" << std::endl;
    std::cout << "Position  0 should map to ~0.0 (center)" << std::endl;
    
    return 0;
}
