#include "snake.h"
#include <algorithm>

namespace SnakeGame {

Snake::Snake() {
    reset();
}

void Snake::reset() {
    body_positions_.clear();
    
    // Initialize snake with head at (0,0) and one body segment at (-1,0)
    body_positions_.push_back({0, 0});  // head
    body_positions_.push_back({-1, 0}); // body
}

bool Snake::move(Direction direction) {
    if (body_positions_.empty()) {
        return false;
    }
    
    Position new_head = calculateNextPosition(body_positions_.front(), direction);
    
    // Check bounds (with wrapping)
    new_head = wrapPosition(new_head);
    
    // Add new head
    body_positions_.push_front(new_head);
    
    // Remove tail (this will be undone if we grow)
    body_positions_.pop_back();
    
    return true;
}

void Snake::grow() {
    if (body_positions_.empty()) {
        return;
    }
    
    // Add back the tail that was removed in the last move
    // We need to calculate where the tail should be based on the second-to-last segment
    if (body_positions_.size() >= 2) {
        const Position& last_segment = body_positions_[body_positions_.size() - 1];
        const Position& second_last = body_positions_[body_positions_.size() - 2];
        
        // Calculate the direction from second-last to last
        Position tail_direction = {
            last_segment[0] - second_last[0],
            last_segment[1] - second_last[1]
        };
        
        // Add new tail segment in the same direction
        Position new_tail = {
            last_segment[0] + tail_direction[0],
            last_segment[1] + tail_direction[1]
        };
        
        new_tail = wrapPosition(new_tail);
        body_positions_.push_back(new_tail);
    } else {
        // If only head exists, add a segment behind it
        body_positions_.push_back({body_positions_[0][0] - 1, body_positions_[0][1]});
    }
}

bool Snake::checkSelfCollision() const {
    if (body_positions_.size() <= 1) {
        return false;
    }
    
    const Position& head = body_positions_.front();
    
    // Check if head collides with any body segment (excluding the head itself)
    for (auto it = body_positions_.begin() + 1; it != body_positions_.end(); ++it) {
        if (head == *it) {
            return true;
        }
    }
    
    return false;
}

bool Snake::isAtPosition(const Position& pos) const {
    return std::find(body_positions_.begin(), body_positions_.end(), pos) != body_positions_.end();
}

size_t Snake::getLength() const {
    return body_positions_.size();
}

const Position& Snake::getHeadPosition() const {
    if (body_positions_.empty()) {
        static Position default_pos = {0, 0};
        return default_pos;
    }
    return body_positions_.front();
}

const PositionDeque& Snake::getAllPositions() const {
    return body_positions_;
}

PositionSet Snake::getAvailablePositions() const { // Need to double check this
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

PositionSet Snake::getOccupiedPositions() const {
    PositionSet occupied;
    for (const auto& pos : body_positions_) {
        occupied.insert(pos);
    }
    return occupied;
}

bool Snake::isValidPosition(const Position& pos) { // Need to double check this.
    return pos.size() >= 2 &&
           pos[0] >= -GameConfig::GRID_SIZE_X / 2 && pos[0] < GameConfig::GRID_SIZE_X / 2 &&
           pos[1] >= -GameConfig::GRID_SIZE_Y / 2 && pos[1] < GameConfig::GRID_SIZE_Y / 2;
}

Position Snake::wrapPosition(const Position& pos) {
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

Position Snake::calculateNextPosition(const Position& current, Direction dir) const {
    if (current.size() < 2) { // Not sure about this. I think this is wrong.
        return current;
    }
    
    Position next = current;
    
    switch (dir) {
        case Direction::UP:
            next[1] += 1;
            break;
        case Direction::DOWN:
            next[1] -= 1;
            break;
        case Direction::LEFT:
            next[0] -= 1;
            break;
        case Direction::RIGHT:
            next[0] += 1;
            break;
        case Direction::NONE:
            // No movement
            break;
    }
    
    return next;
}

} // namespace SnakeGame
