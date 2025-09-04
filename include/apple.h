#pragma once

#include "common_types.h"
#include <random>
#include <optional>

namespace SnakeGame {

/**
 * @brief Represents the apple entity in the game
 * 
 * This class manages the apple's position and provides methods for
 * generating new apple positions when consumed.
 */
class Apple {
public:
    Apple();
    explicit Apple(const PositionSet& forbidden_positions);
    ~Apple() = default;
    
    // Core functionality
    void generateNewPosition(const PositionSet& forbidden_positions);
    bool isAtPosition(const Position& pos) const;
    
    // Position access
    const Position& getPosition() const;
    
    // Utility methods
    bool hasValidPosition() const;
    void reset();
    
private:
    Position position_;
    mutable std::random_device rd_;
    mutable std::mt19937 gen_;
    
    // Helper methods
    std::optional<Position> findRandomValidPosition(const PositionSet& forbidden_positions) const;
    bool isValidApplePosition(const Position& pos, const PositionSet& forbidden_positions) const;
    
    // Copy constructor and assignment operator
    Apple(const Apple&) = delete;
    Apple& operator=(const Apple&) = delete;
    
    // Move constructor and assignment operator
    Apple(Apple&&) = default;
    Apple& operator=(Apple&&) = default;
};

} // namespace SnakeGame
