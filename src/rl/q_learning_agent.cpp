#include "../include/rl/q_learning_agent.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>

namespace SnakeGame::RL {

QLearningAgent::QLearningAgent(double learning_rate, double discount_factor, double epsilon)
    : learning_rate_(learning_rate)
    , discount_factor_(discount_factor)
    , epsilon_(epsilon)
    , epsilon_decay_(0.995)
    , min_epsilon_(0.01)
    , gen_(rd_())
    , uniform_dist_(0.0, 1.0)
    , total_steps_(0)
    , exploration_steps_(0) {
}

int QLearningAgent::selectAction(const std::vector<double>& state) {
    total_steps_++;
    std::string state_str = stateToString(state);
    
    // Epsilon-greedy action selection
    if (uniform_dist_(gen_) < epsilon_) {
        exploration_steps_++;
        return selectRandomAction();
    } else {
        return selectGreedyAction(state_str);
    }
}

void QLearningAgent::update(const std::vector<double>& state, int action,
                           double reward, const std::vector<double>& next_state, bool done) {
    std::string state_str = stateToString(state);
    std::string next_state_str = stateToString(next_state);
    
    // Initialize Q-values if not exists
    initializeQValue(state_str, action);
    
    // Calculate Q-learning update
    double current_q = q_table_[state_str][action];
    double max_next_q = done ? 0.0 : getMaxQValue(next_state_str);
    double target_q = reward + discount_factor_ * max_next_q;
    
    // Update Q-value
    q_table_[state_str][action] = current_q + learning_rate_ * (target_q - current_q);
}

void QLearningAgent::train(Environment& env, size_t episodes) {
    std::cout << "Training Q-Learning agent for " << episodes << " episodes..." << std::endl;
    
    std::vector<double> episode_rewards;
    std::vector<double> episode_lengths;
    
    for (size_t episode = 0; episode < episodes; ++episode) {
        auto state = env.reset();
        double total_reward = 0.0;
        size_t steps = 0;
        
        while (!env.isDone()) {
            int action = selectAction(state);
            auto [next_state, reward] = env.step(action);
            bool done = env.isDone();
            
            update(state, action, reward, next_state, done);
            
            state = next_state;
            total_reward += reward;
            steps++;
        }
        
        episode_rewards.push_back(total_reward);
        episode_lengths.push_back(static_cast<double>(steps));
        
        // Decay epsilon
        epsilon_ = std::max(min_epsilon_, epsilon_ * epsilon_decay_);
        
        // Print progress
        if ((episode + 1) % 100 == 0) {
            double avg_reward = 0.0;
            double avg_length = 0.0;
            size_t recent_episodes = std::min(size_t(100), episode + 1);
            
            for (size_t i = episode + 1 - recent_episodes; i <= episode; ++i) {
                avg_reward += episode_rewards[i];
                avg_length += episode_lengths[i];
            }
            avg_reward /= recent_episodes;
            avg_length /= recent_episodes;
            
            std::cout << "Episode " << episode + 1 
                     << " | Avg Reward: " << std::fixed << std::setprecision(2) << avg_reward
                     << " | Avg Length: " << std::fixed << std::setprecision(1) << avg_length
                     << " | Epsilon: " << std::fixed << std::setprecision(3) << epsilon_
                     << " | Q-table size: " << q_table_.size() << std::endl;
        }
    }
    
    double exploration_rate = static_cast<double>(exploration_steps_) / total_steps_ * 100.0;
    std::cout << "Training completed! Exploration rate: " << std::fixed << std::setprecision(1) 
              << exploration_rate << "%" << std::endl;
}

void QLearningAgent::evaluate(Environment& env, size_t episodes) {
    std::cout << "Evaluating Q-Learning agent for " << episodes << " episodes..." << std::endl;
    
    double old_epsilon = epsilon_;
    epsilon_ = 0.0; // No exploration during evaluation
    
    std::vector<double> episode_rewards;
    std::vector<double> episode_lengths;
    
    for (size_t episode = 0; episode < episodes; ++episode) {
        auto state = env.reset();
        double total_reward = 0.0;
        size_t steps = 0;
        
        while (!env.isDone()) {
            int action = selectAction(state);
            auto [next_state, reward] = env.step(action);
            
            state = next_state;
            total_reward += reward;
            steps++;
        }
        
        episode_rewards.push_back(total_reward);
        episode_lengths.push_back(static_cast<double>(steps));
        
        std::cout << "Eval Episode " << episode + 1 
                 << " | Reward: " << total_reward 
                 << " | Length: " << steps << std::endl;
    }
    
    // Calculate statistics
    double total_reward = 0.0, total_length = 0.0;
    for (size_t i = 0; i < episodes; ++i) {
        total_reward += episode_rewards[i];
        total_length += episode_lengths[i];
    }
    
    std::cout << "Evaluation completed!" << std::endl;
    std::cout << "Average Reward: " << total_reward / episodes << std::endl;
    std::cout << "Average Length: " << total_length / episodes << std::endl;
    
    epsilon_ = old_epsilon; // Restore epsilon
}

void QLearningAgent::save(const std::string& filepath) {
    std::ofstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file for saving: " + filepath);
    }
    
    // Save hyperparameters
    file << learning_rate_ << " " << discount_factor_ << " " << epsilon_ << " " 
         << epsilon_decay_ << " " << min_epsilon_ << std::endl;
    
    // Save Q-table
    file << q_table_.size() << std::endl;
    for (const auto& state_entry : q_table_) {
        file << state_entry.first << " " << state_entry.second.size() << std::endl;
        for (const auto& action_entry : state_entry.second) {
            file << action_entry.first << " " << action_entry.second << std::endl;
        }
    }
    
    std::cout << "Q-Learning agent saved to: " << filepath << std::endl;
}

void QLearningAgent::load(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file for loading: " + filepath);
    }
    
    // Load hyperparameters
    file >> learning_rate_ >> discount_factor_ >> epsilon_ >> epsilon_decay_ >> min_epsilon_;
    
    // Load Q-table
    size_t num_states;
    file >> num_states;
    q_table_.clear();
    
    for (size_t i = 0; i < num_states; ++i) {
        std::string state;
        size_t num_actions;
        file >> state >> num_actions;
        
        for (size_t j = 0; j < num_actions; ++j) {
            int action;
            double q_value;
            file >> action >> q_value;
            q_table_[state][action] = q_value;
        }
    }
    
    std::cout << "Q-Learning agent loaded from: " << filepath << std::endl;
    std::cout << "Q-table size: " << q_table_.size() << " states" << std::endl;
}

void QLearningAgent::setLearningRate(double lr) {
    learning_rate_ = lr;
}

void QLearningAgent::setEpsilon(double epsilon) {
    epsilon_ = epsilon;
}

void QLearningAgent::setDiscountFactor(double gamma) {
    discount_factor_ = gamma;
}

void QLearningAgent::setEpsilonDecay(double decay) {
    epsilon_decay_ = decay;
}

double QLearningAgent::getQValue(const std::string& state, int action) const {
    auto state_it = q_table_.find(state);
    if (state_it == q_table_.end()) {
        return 0.0;
    }
    
    auto action_it = state_it->second.find(action);
    if (action_it == state_it->second.end()) {
        return 0.0;
    }
    
    return action_it->second;
}

void QLearningAgent::printQTable() const {
    std::cout << "Q-Table (showing top 10 states):" << std::endl;
    
    size_t count = 0;
    for (const auto& state_entry : q_table_) {
        if (count >= 10) break;
        
        std::cout << "State: " << state_entry.first << std::endl;
        for (const auto& action_entry : state_entry.second) {
            std::cout << "  Action " << action_entry.first 
                     << ": " << std::fixed << std::setprecision(4) << action_entry.second << std::endl;
        }
        count++;
    }
}

std::string QLearningAgent::stateToString(const std::vector<double>& state) const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    for (size_t i = 0; i < state.size(); ++i) {
        if (i > 0) oss << ",";
        oss << state[i];
    }
    return oss.str();
}

int QLearningAgent::selectGreedyAction(const std::string& state_str) const {
    auto state_it = q_table_.find(state_str);
    if (state_it == q_table_.end()) {
        return selectRandomAction();
    }
    
    int best_action = 0;
    double best_value = std::numeric_limits<double>::lowest();
    
    for (int action = 0; action < 4; ++action) {
        auto action_it = state_it->second.find(action);
        double q_value = (action_it != state_it->second.end()) ? action_it->second : 0.0;
        
        if (q_value > best_value) {
            best_value = q_value;
            best_action = action;
        }
    }
    
    return best_action;
}

int QLearningAgent::selectRandomAction() const {
    std::uniform_int_distribution<int> action_dist(0, 3);
    return action_dist(gen_);
}

double QLearningAgent::getMaxQValue(const std::string& state_str) const {
    auto state_it = q_table_.find(state_str);
    if (state_it == q_table_.end()) {
        return 0.0;
    }
    
    double max_value = std::numeric_limits<double>::lowest();
    for (int action = 0; action < 4; ++action) {
        auto action_it = state_it->second.find(action);
        double q_value = (action_it != state_it->second.end()) ? action_it->second : 0.0;
        max_value = std::max(max_value, q_value);
    }
    
    return max_value;
}

void QLearningAgent::initializeQValue(const std::string& state_str, int action) {
    if (q_table_[state_str].find(action) == q_table_[state_str].end()) {
        q_table_[state_str][action] = 0.0;
    }
}

} // namespace SnakeGame::RL
