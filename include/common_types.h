#pragma once

#include <vector>
#include <set>
#include <deque>
#include <memory>
#include <optional>

namespace SnakeGame {

// Type aliases for better readability
using Position = std::vector<int>;
using PositionSet = std::set<Position>;
using PositionDeque = std::deque<Position>;

// Game configuration constants
struct GameConfig {
    static constexpr int WINDOW_WIDTH = 500;
    static constexpr int WINDOW_HEIGHT = 500;
    static constexpr int GRID_SIZE_X = 10;
    static constexpr int GRID_SIZE_Y = 10;
    static constexpr double CELL_WIDTH = 2.0 / GRID_SIZE_X;
    static constexpr double CELL_HEIGHT = 2.0 / GRID_SIZE_Y;
    static constexpr unsigned long MAX_DELAY = 200000;
    static constexpr int DEFAULT_GAME_SPEED = 1;
};

// Direction enumeration with better naming
enum class Direction {
    UP = 0,
    DOWN = 1, 
    LEFT = 2,
    RIGHT = 3,
    NONE = 4  // For RL agents when no action is taken
};

// Object type enumeration for rendering
enum class EntityType {
    SNAKE_BODY = 0,
    APPLE = 1,
    SNAKE_HEAD = 2
};

// Game state enumeration
enum class GameStateType {
    PLAYING,
    PAUSED,
    GAME_OVER,
    HELP_DISPLAY
};

// Reward types for RL
enum class RewardType {
    APPLE_EATEN = 10,
    COLLISION = -100,
    TIME_PENALTY = -1,
    NO_REWARD = 0
};

} // namespace SnakeGame
