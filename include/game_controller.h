#pragma once

#include "common_types.h"
#include <memory>
#include <functional>

namespace SnakeGame {

// Forward declarations
class Snake;
class Apple;
class Graphics;

/**
 * @brief Main game controller that manages the game state and coordinates between components
 * 
 * This class serves as the central hub for the Snake Game, managing game state,
 * coordinating between game entities, and providing interfaces for both human players
 * and RL agents.
 */
class Game {
public:
    Game();
    ~Game();

    // Game lifecycle management
    void initialize();
    void reset();
    void step();
    bool isGameOver() const;
    
    // Game state management
    void setState(GameStateType state);
    GameStateType getState() const;
    
    // Score management
    unsigned int getScore() const;
    unsigned int getHighScore() const;
    void updateHighScore();
    
    // Speed management
    void setGameSpeed(int speed);
    int getGameSpeed() const;
    
    // Direction control (for human players)
    void setDirection(Direction dir);
    Direction getCurrentDirection() const;
    
    // RL Interface methods
    std::vector<double> getStateVector() const;
    double getReward() const;
    bool performAction(Direction action);
    void registerRewardCallback(std::function<void(double)> callback);
    
    // Graphics interface
    void setGraphics(std::unique_ptr<Graphics> graphics);
    void render();
    
    // Component access (for advanced users)
    const Snake& getSnake() const;
    const Apple& getApple() const;
    Graphics* getGraphics() const;
    
private:
    // Core game components
    std::unique_ptr<Snake> snake_;
    std::unique_ptr<Apple> apple_;
    std::unique_ptr<Graphics> graphics_;
    
    // Game state
    GameStateType current_state_;
    unsigned int score_;
    unsigned int high_score_;
    int game_speed_;
    Direction current_direction_;
    double last_reward_;
    
    // RL callback
    std::function<void(double)> reward_callback_;
    
    // Internal helper methods
    void handleCollision();
    void handleAppleEaten();
    double calculateReward() const;
    void updateGameLogic();
    
    // Prevent copying
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    
    // Allow moving
    Game(Game&&) = default;
    Game& operator=(Game&&) = default;
};

} // namespace SnakeGame
