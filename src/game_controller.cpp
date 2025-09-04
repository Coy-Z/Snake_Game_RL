#include "game_controller.h"
#include "snake.h"
#include "apple.h"
#include "graphics.h"
#include <stdexcept>
#include <algorithm>

namespace SnakeGame {

Game::Game() // Member initialization list since we can't assign to references.
    : snake_(std::make_unique<Snake>())
    , apple_(std::make_unique<Apple>())
    , graphics_(nullptr)
    , current_state_(GameStateType::PLAYING)
    , score_(0)
    , high_score_(0)
    , game_speed_(GameConfig::DEFAULT_GAME_SPEED)
    , current_direction_(Direction::RIGHT)
    , last_reward_(0.0)
    , reward_callback_(nullptr) {
}

Game::~Game() = default;

void Game::initialize() {
    reset();
    if (graphics_) {
        graphics_->initialize();
    }
}

void Game::reset() {
    snake_->reset();
    apple_->generateNewPosition(snake_->getOccupiedPositions());
    current_state_ = GameStateType::PLAYING;
    score_ = 0;
    current_direction_ = Direction::RIGHT;
    last_reward_ = 0.0;
}

void Game::step() {
    if (current_state_ != GameStateType::PLAYING) {
        return;
    }
    
    updateGameLogic();
    
    if (graphics_) {
        render();
    }
}

bool Game::isGameOver() const {
    return current_state_ == GameStateType::GAME_OVER;
}

void Game::setState(GameStateType state) {
    current_state_ = state;
}

GameStateType Game::getState() const {
    return current_state_;
}

unsigned int Game::getScore() const {
    return score_;
}

unsigned int Game::getHighScore() const {
    return high_score_;
}

void Game::updateHighScore() {
    if (score_ > high_score_) {
        high_score_ = score_;
    }
}

void Game::setGameSpeed(int speed) {
    game_speed_ = std::max(1, speed);
}

int Game::getGameSpeed() const {
    return game_speed_;
}

void Game::setDirection(Direction dir) {
    // Prevent reversing direction
    if ((current_direction_ == Direction::UP && dir == Direction::DOWN) ||
        (current_direction_ == Direction::DOWN && dir == Direction::UP) ||
        (current_direction_ == Direction::LEFT && dir == Direction::RIGHT) ||
        (current_direction_ == Direction::RIGHT && dir == Direction::LEFT)) {
        return;
    }
    
    current_direction_ = dir;
}

Direction Game::getCurrentDirection() const {
    return current_direction_;
}

std::vector<double> Game::getStateVector() const {
    std::vector<double> state;
    
    // Snake head position (normalized)
    const auto& head = snake_->getHeadPosition();
    state.push_back(static_cast<double>(head[0]) / GameConfig::GRID_SIZE_X);
    state.push_back(static_cast<double>(head[1]) / GameConfig::GRID_SIZE_Y);
    
    // Apple position (normalized)
    const auto& apple_pos = apple_->getPosition();
    state.push_back(static_cast<double>(apple_pos[0]) / GameConfig::GRID_SIZE_X);
    state.push_back(static_cast<double>(apple_pos[1]) / GameConfig::GRID_SIZE_Y);
    
    // Direction as one-hot encoding
    for (int i = 0; i < 4; ++i) {
        state.push_back(static_cast<int>(current_direction_) == i ? 1.0 : 0.0);
    }
    
    // Distance to walls (normalized) - Need to double check this
    state.push_back(static_cast<double>(head[0] + GameConfig::GRID_SIZE_X/2) / GameConfig::GRID_SIZE_X); // left wall
    state.push_back(static_cast<double>(GameConfig::GRID_SIZE_X/2 - head[0]) / GameConfig::GRID_SIZE_X); // right wall
    state.push_back(static_cast<double>(head[1] + GameConfig::GRID_SIZE_Y/2) / GameConfig::GRID_SIZE_Y); // bottom wall
    state.push_back(static_cast<double>(GameConfig::GRID_SIZE_Y/2 - head[1]) / GameConfig::GRID_SIZE_Y); // top wall
    
    // Check for obstacles in 4 directions
    Position next_pos = head;
    next_pos[1] += 1; // up
    state.push_back(snake_->isAtPosition(next_pos) ? 1.0 : 0.0);
    
    next_pos = head;
    next_pos[1] -= 1; // down
    state.push_back(snake_->isAtPosition(next_pos) ? 1.0 : 0.0);
    
    next_pos = head;
    next_pos[0] -= 1; // left
    state.push_back(snake_->isAtPosition(next_pos) ? 1.0 : 0.0);
    
    next_pos = head;
    next_pos[0] += 1; // right
    state.push_back(snake_->isAtPosition(next_pos) ? 1.0 : 0.0);
    
    // Snake length (normalized)
    state.push_back(static_cast<double>(snake_->getLength()) / (GameConfig::GRID_SIZE_X * GameConfig::GRID_SIZE_Y));
    
    return state;
}

double Game::getReward() const {
    return last_reward_;
}

bool Game::performAction(Direction action) {
    if (current_state_ != GameStateType::PLAYING) {
        return false;
    }
    
    setDirection(action);
    updateGameLogic();
    
    return current_state_ == GameStateType::PLAYING;
}

void Game::registerRewardCallback(std::function<void(double)> callback) { // callback function must return void and take one double param
    reward_callback_ = callback;
}

void Game::setGraphics(std::unique_ptr<Graphics> graphics) {
    graphics_ = std::move(graphics);
}

void Game::render() {
    if (!graphics_) {
        return;
    }
    
    graphics_->clear();
    
    switch (current_state_) {
        case GameStateType::PLAYING:
            graphics_->drawSnake(*snake_);
            graphics_->drawApple(*apple_);
            graphics_->drawScore(score_, high_score_);
            break;
        case GameStateType::PAUSED:
            graphics_->drawSnake(*snake_);
            graphics_->drawApple(*apple_);
            graphics_->drawScore(score_, high_score_);
            graphics_->drawPaused();
            break;
        case GameStateType::GAME_OVER:
            graphics_->drawSnake(*snake_);
            graphics_->drawApple(*apple_);
            graphics_->drawScore(score_, high_score_);
            graphics_->drawGameOver();
            break;
        case GameStateType::HELP_DISPLAY:
            graphics_->drawHelp();
            break;
    }
    
    graphics_->present();
}

const Snake& Game::getSnake() const {
    return *snake_;
}

const Apple& Game::getApple() const {
    return *apple_;
}

Graphics* Game::getGraphics() const {
    return graphics_.get();
}

void Game::handleCollision() {
    current_state_ = GameStateType::GAME_OVER;
    updateHighScore();
    last_reward_ = static_cast<double>(RewardType::COLLISION);
    
    if (reward_callback_) {
        reward_callback_(last_reward_);
    }
}

void Game::handleAppleEaten() {
    score_++;
    snake_->grow();
    apple_->generateNewPosition(snake_->getOccupiedPositions());
    last_reward_ = static_cast<double>(RewardType::APPLE_EATEN);
    
    if (reward_callback_) {
        reward_callback_(last_reward_);
    }
}

double Game::calculateReward() const {
    // Default time penalty for each step
    return static_cast<double>(RewardType::TIME_PENALTY);
}

void Game::updateGameLogic() {
    // Move the snake
    bool move_successful = snake_->move(current_direction_);
    
    if (!move_successful || snake_->checkSelfCollision()) {
        handleCollision();
        return;
    }
    
    // Check if apple was eaten
    if (snake_->isAtPosition(apple_->getPosition())) {
        handleAppleEaten();
    } else {
        // Apply time penalty if no apple was eaten
        last_reward_ = calculateReward();
        if (reward_callback_) {
            reward_callback_(last_reward_);
        }
    }
}
} // namespace SnakeGame
