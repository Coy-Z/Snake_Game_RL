#include "rl/rl_interface.h"
#include "game_controller.h"
#include "graphics.h"
#include <stdexcept>
#include <iostream>

namespace SnakeGame::RL {

// SnakeEnvironment implementation
SnakeEnvironment::SnakeEnvironment() 
    : SnakeEnvironment(false) {
}

SnakeEnvironment::SnakeEnvironment(bool headless) 
    : game_(std::make_unique<SnakeGame::Game>())
    , headless_mode_(headless)
    , step_count_(0)
    , max_steps_(1000)
    , seed_(0)
    , apple_reward_(static_cast<double>(RewardType::APPLE_EATEN))
    , collision_penalty_(static_cast<double>(RewardType::COLLISION))
    , time_penalty_(static_cast<double>(RewardType::TIME_PENALTY)) {
    
    // Set up graphics based on mode
    if (headless_mode_) {
        game_->setGraphics(std::make_unique<SnakeGame::HeadlessGraphics>());
    } else {
        game_->setGraphics(SnakeGame::createGraphics("opengl"));
    }
    
    game_->initialize();
}

SnakeEnvironment::~SnakeEnvironment() = default;

std::vector<double> SnakeEnvironment::reset() {
    game_->reset();
    step_count_ = 0;
    return encodeGameState();
}

std::pair<std::vector<double>, double> SnakeEnvironment::step(int action) {
    if (action < 0 || action >= static_cast<int>(getActionSpaceSize())) {
        throw std::invalid_argument("Invalid action: " + std::to_string(action));
    }
    
    Direction dir = intToDirection(action);
    bool game_continues = game_->performAction(dir);
    
    step_count_++;
    
    double reward = game_->getReward();
    std::vector<double> next_state = encodeGameState();
    
    // Check if episode is done
    bool done = !game_continues || step_count_ >= max_steps_;
    
    return {next_state, reward};
}

bool SnakeEnvironment::isDone() const {
    return game_->isGameOver() || step_count_ >= max_steps_;
}

void SnakeEnvironment::render() {
    if (!headless_mode_) {
        game_->render();
    }
}

size_t SnakeEnvironment::getActionSpaceSize() const {
    return 4; // UP, DOWN, LEFT, RIGHT
}

size_t SnakeEnvironment::getStateSpaceSize() const {
    // Current state vector size:
    // 2 (snake head position) + 2 (apple position) + 4 (direction one-hot) + 
    // 4 (distance to walls) + 4 (obstacles in 4 directions) + 1 (snake length) = 17
    return 17;
}

std::vector<int> SnakeEnvironment::getActionSpace() const {
    return {0, 1, 2, 3}; // UP, DOWN, LEFT, RIGHT
}

void SnakeEnvironment::setSeed(unsigned int seed) {
    seed_ = seed;
    // TODO: Implement proper seeding for all random components
}

std::vector<double> SnakeEnvironment::getInfo() const {
    return {
        static_cast<double>(game_->getScore()),
        static_cast<double>(game_->getHighScore()),
        static_cast<double>(step_count_),
        10.0  // placeholder for snake length
    };
}

void SnakeEnvironment::setRewardStructure(double apple_reward, double collision_penalty, double time_penalty) {
    apple_reward_ = apple_reward;
    collision_penalty_ = collision_penalty;
    time_penalty_ = time_penalty;
}

void SnakeEnvironment::setMaxSteps(size_t max_steps) {
    max_steps_ = max_steps;
}

std::vector<double> SnakeEnvironment::encodeGameState() const {
    return game_->getStateVector();
}

Direction SnakeEnvironment::intToDirection(int action) const {
    switch (action) {
        case 0: return Direction::UP;
        case 1: return Direction::DOWN;
        case 2: return Direction::LEFT;
        case 3: return Direction::RIGHT;
        default: return Direction::NONE;
    }
}

// RandomAgent implementation
RandomAgent::RandomAgent() : gen_(rd_()), dist_(0, 3) {
}

RandomAgent::RandomAgent(unsigned int seed) : gen_(seed), dist_(0, 3) {
}

int RandomAgent::selectAction(const std::vector<double>& state) {
    return dist_(gen_);
}

} // namespace SnakeGame::RL
