#pragma once

#include "../rl_interface.h"
#include <map>
#include <random>

namespace SnakeGame::RL {

/**
 * @brief Q-Learning agent implementation
 * 
 * This class implements a tabular Q-learning agent for the Snake Game.
 * It demonstrates how to extend the Agent interface for custom RL algorithms.
 */
class QLearningAgent : public Agent {
public:
    QLearningAgent(double learning_rate = 0.1, 
                   double discount_factor = 0.95, 
                   double epsilon = 0.1);
    
    // Agent interface implementation
    int selectAction(const std::vector<double>& state) override;
    void update(const std::vector<double>& state, int action, 
                double reward, const std::vector<double>& next_state, bool done) override;
    
    // Training interface
    void train(Environment& env, size_t episodes) override;
    void evaluate(Environment& env, size_t episodes) override;
    
    // Model management
    void save(const std::string& filepath) override;
    void load(const std::string& filepath) override;
    
    // Configuration
    void setLearningRate(double lr) override;
    void setEpsilon(double epsilon) override;
    
    // Q-Learning specific methods
    void setDiscountFactor(double gamma);
    void setEpsilonDecay(double decay);
    double getQValue(const std::string& state, int action) const;
    void printQTable() const;
    
private:
    // Hyperparameters
    double learning_rate_;
    double discount_factor_;
    double epsilon_;
    double epsilon_decay_;
    double min_epsilon_;
    
    // Q-table (state -> action -> value)
    std::map<std::string, std::map<int, double>> q_table_;
    
    // Random number generation
    mutable std::random_device rd_;
    mutable std::mt19937 gen_;
    mutable std::uniform_real_distribution<double> uniform_dist_;
    
    // Helper methods
    std::string stateToString(const std::vector<double>& state) const;
    int selectGreedyAction(const std::string& state_str) const;
    int selectRandomAction() const;
    double getMaxQValue(const std::string& state_str) const;
    void initializeQValue(const std::string& state_str, int action);
    
    // Statistics
    mutable size_t total_steps_;
    mutable size_t exploration_steps_;
};

} // namespace SnakeGame::RL
