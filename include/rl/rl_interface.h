#pragma once

#include "../common_types.h"
#include <vector>
#include <memory>
#include <random>

namespace SnakeGame {
    class Game; // Forward declaration
}

namespace SnakeGame::RL {

/**
 * @brief Abstract base class for RL environments
 * 
 * This interface defines the standard RL environment API that can be used
 * with various RL frameworks (like OpenAI Gym, DeepMind Lab, etc.)
 */
class Environment {
public:
    virtual ~Environment() = default;
    
    // Core RL interface
    virtual std::vector<double> reset() = 0;
    virtual std::pair<std::vector<double>, double> step(int action) = 0;
    virtual bool isDone() const = 0;
    virtual void render() = 0;
    
    // Environment information
    virtual size_t getActionSpaceSize() const = 0;
    virtual size_t getStateSpaceSize() const = 0;
    virtual std::vector<int> getActionSpace() const = 0;
    
    // Optional methods for advanced use
    virtual void setSeed(unsigned int seed) {}
    virtual std::vector<double> getInfo() const { return {}; }
};

/**
 * @brief Snake Game environment for RL training
 * 
 * This class implements the Environment interface specifically for the Snake Game,
 * providing a standardized interface for RL agents.
 */
class SnakeEnvironment : public Environment {
public:
    SnakeEnvironment();
    explicit SnakeEnvironment(bool headless = false);
    ~SnakeEnvironment() override;
    
    // RL interface implementation
    std::vector<double> reset() override;
    std::pair<std::vector<double>, double> step(int action) override;
    bool isDone() const override;
    void render() override;
    
    // Environment information
    size_t getActionSpaceSize() const override;
    size_t getStateSpaceSize() const override;
    std::vector<int> getActionSpace() const override;
    
    // Snake-specific methods
    void setSeed(unsigned int seed) override;
    std::vector<double> getInfo() const override;
    
    // Configuration
    void setRewardStructure(double apple_reward, double collision_penalty, double time_penalty);
    void setMaxSteps(size_t max_steps);
    
private:
    std::unique_ptr<SnakeGame::Game> game_;
    bool headless_mode_;
    size_t step_count_;
    size_t max_steps_;
    unsigned int seed_;
    
    // Reward configuration
    double apple_reward_;
    double collision_penalty_;
    double time_penalty_;
    
    // State representation methods
    std::vector<double> encodeGameState() const;
    Direction intToDirection(int action) const;
    
    // Copy prevention
    SnakeEnvironment(const SnakeEnvironment&) = delete;
    SnakeEnvironment& operator=(const SnakeEnvironment&) = delete;
};

/**
 * @brief Abstract base class for RL agents
 * 
 * This interface defines the basic structure that RL agents should implement
 * to interact with the Snake Game environment.
 */
class Agent {
public:
    virtual ~Agent() = default;
    
    // Core agent interface
    virtual int selectAction(const std::vector<double>& state) = 0;
    virtual void update(const std::vector<double>& state, int action, 
                       double reward, const std::vector<double>& next_state, bool done) {}
    
    // Training interface
    virtual void train(Environment& env, size_t episodes) {}
    virtual void evaluate(Environment& env, size_t episodes) {}
    
    // Model management
    virtual void save(const std::string& filepath) {}
    virtual void load(const std::string& filepath) {}
    
    // Configuration
    virtual void setLearningRate(double lr) {}
    virtual void setEpsilon(double epsilon) {}
};

/**
 * @brief Simple random agent for testing
 * 
 * This agent selects actions randomly and can be used as a baseline
 * or for testing the environment.
 */
class RandomAgent : public Agent {
public:
    RandomAgent();
    explicit RandomAgent(unsigned int seed);
    ~RandomAgent() override = default;
    
    int selectAction(const std::vector<double>& state) override;
    
private:
    std::random_device rd_;
    std::mt19937 gen_;
    std::uniform_int_distribution<> dist_;
};

} // namespace SnakeGame::RL
