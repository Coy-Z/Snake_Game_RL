#pragma once

#include "common_types.h"
#include <deque>
#include <set>
#include <optional>

namespace SnakeGame {

/**
 * @brief Represents the snake entity in the game
 * 
 * This class manages the snake's position, movement, and collision detection.
 * It provides a clean interface for both game logic and RL agents.
 */
class Snake {
public:
    Snake();
    ~Snake() = default;
    
    // Core functionality
    void reset();
    bool move(Direction direction);
    void grow();
    
    // State queries
    bool checkSelfCollision() const;
    bool isAtPosition(const Position& pos) const;
    size_t getLength() const;
    
    // Position access
    const Position& getHeadPosition() const;
    const PositionDeque& getAllPositions() const;
    PositionSet getAvailablePositions() const;
    PositionSet getOccupiedPositions() const;
    
    // Bounds checking
    static bool isValidPosition(const Position& pos);
    static Position wrapPosition(const Position& pos);
    
private:
    PositionDeque body_positions_;
    
    // Helper methods
    Position calculateNextPosition(const Position& current, Direction dir) const;
    void updateAvailablePositions();
    
    // Copy constructor and assignment operator
    Snake(const Snake&) = delete;
    Snake& operator=(const Snake&) = delete;
    
    // Move constructor and assignment operator  
    Snake(Snake&&) = default;
    Snake& operator=(Snake&&) = default;
};

} // namespace SnakeGame
