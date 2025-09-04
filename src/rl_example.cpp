#include "include/rl/rl_interface.h"
#include "include/rl/q_learning_agent.h"
#include <iostream>
#include <string>

using namespace SnakeGame::RL;

void printUsage(const char* program_name) {
    std::cout << "Usage: " << program_name << " [command] [options]" << std::endl;
    std::cout << "Commands:" << std::endl;
    std::cout << "  train [episodes]     - Train Q-Learning agent (default: 1000 episodes)" << std::endl;
    std::cout << "  evaluate [episodes]  - Evaluate trained agent (default: 10 episodes)" << std::endl;
    std::cout << "  demo                 - Quick demo with random agent" << std::endl;
    std::cout << "  compare              - Compare random vs Q-Learning agent" << std::endl;
}

void trainQLearningAgent(int episodes = 1000) {
    std::cout << "=== Training Q-Learning Agent ===" << std::endl;
    
    // Create environment and agent
    SnakeEnvironment env(true); // headless mode for faster training
    QLearningAgent agent(0.1, 0.95, 0.3); // lr=0.1, gamma=0.95, epsilon=0.3
    
    // Configure environment
    env.setMaxSteps(500);
    env.setRewardStructure(10.0, -100.0, -1.0); // apple, collision, time penalty
    
    // Train the agent
    agent.train(env, episodes);
    
    // Save the trained model
    agent.save("q_learning_model.txt");
    std::cout << "Model saved as 'q_learning_model.txt'" << std::endl;
}

void evaluateQLearningAgent(int episodes = 10) {
    std::cout << "=== Evaluating Q-Learning Agent ===" << std::endl;
    
    try {
        // Create environment and agent
        SnakeEnvironment env(false); // with graphics for evaluation
        QLearningAgent agent;
        
        // Load trained model
        agent.load("q_learning_model.txt");
        
        // Evaluate the agent
        agent.evaluate(env, episodes);
        
        // Print some Q-values for inspection
        std::cout << "\nSample Q-values:" << std::endl;
        agent.printQTable();
        
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        std::cout << "Make sure to train the agent first!" << std::endl;
    }
}

void runDemo() {
    std::cout << "=== Random Agent Demo ===" << std::endl;
    
    SnakeEnvironment env(true); // headless mode
    RandomAgent agent;
    
    env.setMaxSteps(200);
    
    const int num_episodes = 5;
    for (int episode = 0; episode < num_episodes; ++episode) {
        std::cout << "Episode " << (episode + 1) << "/" << num_episodes << std::endl;
        
        auto state = env.reset();
        double total_reward = 0.0;
        int steps = 0;
        
        while (!env.isDone()) {
            int action = agent.selectAction(state);
            auto [next_state, reward] = env.step(action);
            
            total_reward += reward;
            state = next_state;
            steps++;
        }
        
        auto info = env.getInfo();
        std::cout << "  Final Score: " << info[0] 
                 << ", Total Reward: " << total_reward 
                 << ", Steps: " << steps << std::endl;
    }
}

void compareAgents() {
    std::cout << "=== Agent Comparison ===" << std::endl;
    
    const int num_episodes = 20;
    
    // Test Random Agent
    std::cout << "\nTesting Random Agent:" << std::endl;
    SnakeEnvironment env1(true);
    RandomAgent random_agent;
    env1.setMaxSteps(300);
    
    double random_total_score = 0.0;
    for (int episode = 0; episode < num_episodes; ++episode) {
        auto state = env1.reset();
        while (!env1.isDone()) {
            int action = random_agent.selectAction(state);
            auto [next_state, reward] = env1.step(action);
            state = next_state;
        }
        auto info = env1.getInfo();
        random_total_score += info[0];
    }
    
    // Test Q-Learning Agent (if available)
    double qlearning_total_score = 0.0;
    try {
        std::cout << "\nTesting Q-Learning Agent:" << std::endl;
        SnakeEnvironment env2(true);
        QLearningAgent qlearning_agent;
        qlearning_agent.load("q_learning_model.txt");
        env2.setMaxSteps(300);
        
        for (int episode = 0; episode < num_episodes; ++episode) {
            auto state = env2.reset();
            while (!env2.isDone()) {
                int action = qlearning_agent.selectAction(state);
                auto [next_state, reward] = env2.step(action);
                state = next_state;
            }
            auto info = env2.getInfo();
            qlearning_total_score += info[0];
        }
        
        std::cout << "\n=== Results ===\n";
        std::cout << "Random Agent Average Score: " << random_total_score / num_episodes << std::endl;
        std::cout << "Q-Learning Agent Average Score: " << qlearning_total_score / num_episodes << std::endl;
        
        double improvement = ((qlearning_total_score - random_total_score) / random_total_score) * 100.0;
        std::cout << "Improvement: " << std::fixed << std::setprecision(1) << improvement << "%" << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "Could not load Q-Learning model: " << e.what() << std::endl;
        std::cout << "Random Agent Average Score: " << random_total_score / num_episodes << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage(argv[0]);
        return 1;
    }
    
    std::string command = argv[1];
    
    try {
        if (command == "train") {
            int episodes = (argc > 2) ? std::stoi(argv[2]) : 1000;
            trainQLearningAgent(episodes);
        } else if (command == "evaluate") {
            int episodes = (argc > 2) ? std::stoi(argv[2]) : 10;
            evaluateQLearningAgent(episodes);
        } else if (command == "demo") {
            runDemo();
        } else if (command == "compare") {
            compareAgents();
        } else {
            std::cout << "Unknown command: " << command << std::endl;
            printUsage(argv[0]);
            return 1;
        }
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
