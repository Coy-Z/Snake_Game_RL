#include "apple.h"
#include <algorithm>
#include <iterator>

namespace SnakeGame {

Apple::Apple() : gen_(rd_()) {
    reset();
}

Apple::Apple(const PositionSet& forbidden_positions) : gen_(rd_()) {
    generateNewPosition(forbidden_positions);
}

void Apple::generateNewPosition(const PositionSet& forbidden_positions) {
    auto new_pos = findRandomValidPosition(forbidden_positions);
    if (new_pos.has_value()) {
        position_ = new_pos.value();
    } else {
        // Fallback to a default position if no valid position is found
        // This should rarely happen unless the grid is almost full
        position_ = {0, 0};
    }
}

bool Apple::isAtPosition(const Position& pos) const {
    return position_ == pos;
}

const Position& Apple::getPosition() const {
    return position_;
}

bool Apple::hasValidPosition() const {
    return position_.size() >= 2;
}

void Apple::reset() {
    position_ = {GameConfig::GRID_SIZE_X / 2 - 1, GameConfig::GRID_SIZE_Y / 2 - 1};
}

std::optional<Position> Apple::findRandomValidPosition(const PositionSet& forbidden_positions) const {
    // Create a vector of all possible positions
    std::vector<Position> all_positions;
    
    for (int x = -GameConfig::GRID_SIZE_X / 2; x < GameConfig::GRID_SIZE_X / 2; ++x) {
        for (int y = -GameConfig::GRID_SIZE_Y / 2; y < GameConfig::GRID_SIZE_Y / 2; ++y) {
            Position pos = {x, y};
            if (isValidApplePosition(pos, forbidden_positions)) {
                all_positions.push_back(pos);
            }
        }
    }
    
    if (all_positions.empty()) {
        return std::nullopt;
    }
    
    // Select a random position from valid positions
    std::uniform_int_distribution<size_t> dist(0, all_positions.size() - 1);
    return all_positions[dist(gen_)];
}

bool Apple::isValidApplePosition(const Position& pos, const PositionSet& forbidden_positions) const {
    // Check if position is within bounds
    if (pos.size() < 2) {
        return false;
    }
    
    if (pos[0] < -GameConfig::GRID_SIZE_X / 2 || pos[0] >= GameConfig::GRID_SIZE_X / 2 ||
        pos[1] < -GameConfig::GRID_SIZE_Y / 2 || pos[1] >= GameConfig::GRID_SIZE_Y / 2) {
        return false;
    }
    
    // Check if position is not forbidden (not occupied by snake)
    return forbidden_positions.find(pos) == forbidden_positions.end();
}

} // namespace SnakeGame
