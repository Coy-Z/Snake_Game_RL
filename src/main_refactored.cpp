#include "game_controller.h"
#include "graphics.h"
#include "rl/rl_interface.h"
#include <iostream>
#include <memory>
#include <chrono>
#include <thread>

#ifdef __APPLE__
#include<GLUT/glut.h>
#else
#include "../packages/freeglut.3.0.0.v140.1.0.2/sources/GL/glut.h"
#endif

using namespace SnakeGame;

// Global game instance for GLUT callbacks
std::unique_ptr<Game> g_game = nullptr;

// GLUT callback functions
void display() {
    if (g_game) {
        g_game->render();
    }
}

void update() {
    if (g_game && g_game->getState() == GameStateType::PLAYING) {
        g_game->step();
    }
    
    // Control game speed
    std::this_thread::sleep_for(std::chrono::milliseconds(150));
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    if (!g_game) return;
    
    switch (key) {
        case 'w':
        case 'W':
            g_game->setDirection(Direction::UP);
            break;
        case 's':
        case 'S':
            g_game->setDirection(Direction::DOWN);
            break;
        case 'a':
        case 'A':
            g_game->setDirection(Direction::LEFT);
            break;
        case 'd':
        case 'D':
            g_game->setDirection(Direction::RIGHT);
            break;
        case 'r':
        case 'R':
            g_game->reset();
            break;
        case 'p':
        case 'P':
            if (g_game->getState() == GameStateType::PLAYING) {
                g_game->setState(GameStateType::PAUSED);
            } else if (g_game->getState() == GameStateType::PAUSED) {
                g_game->setState(GameStateType::PLAYING);
            }
            break;
        case 27: // ESC key
            exit(0);
            break;
    }
}

void specialKeyboard(int key, int x, int y) {
    if (!g_game) return;
    
    switch (key) {
        case GLUT_KEY_UP:
            g_game->setDirection(Direction::UP);
            break;
        case GLUT_KEY_DOWN:
            g_game->setDirection(Direction::DOWN);
            break;
        case GLUT_KEY_LEFT:
            g_game->setDirection(Direction::LEFT);
            break;
        case GLUT_KEY_RIGHT:
            g_game->setDirection(Direction::RIGHT);
            break;
    }
}

void runHumanGame(int argc, char* argv[]) {
    std::cout << "Starting Snake Game (Human Player Mode)" << std::endl;
    std::cout << "Controls: WASD or Arrow Keys, P to pause, R to reset, ESC to quit" << std::endl;
    
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(GameConfig::WINDOW_WIDTH, GameConfig::WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    int window = glutCreateWindow("Snake Game - Refactored Edition");
    
    // Set up OpenGL
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Create game instance with OpenGL graphics
    g_game = std::make_unique<Game>();
    g_game->setGraphics(createGraphics("opengl"));
    g_game->initialize();
    
    // Set GLUT callbacks
    glutDisplayFunc(display);
    glutIdleFunc(update);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeyboard);
    
    // Start main loop
    glutMainLoop();
}

void runRLDemo() {
    std::cout << "Starting Snake Game (RL Demo Mode)" << std::endl;
    
    // Create RL environment
    RL::SnakeEnvironment env(true); // headless mode
    RL::RandomAgent agent;
    
    const int num_episodes = 10;
    const int max_steps_per_episode = 500;
    
    for (int episode = 0; episode < num_episodes; ++episode) {
        std::cout << "Episode " << (episode + 1) << "/" << num_episodes << std::endl;
        
        auto state = env.reset();
        double total_reward = 0.0;
        int steps = 0;
        
        while (!env.isDone() && steps < max_steps_per_episode) {
            int action = agent.selectAction(state);
            auto [next_state, reward] = env.step(action);
            
            total_reward += reward;
            state = next_state;
            steps++;
            
            // Optional: print some debug info
            if (steps % 100 == 0) {
                auto info = env.getInfo();
                std::cout << "  Step " << steps << ", Score: " << info[0] 
                         << ", Length: " << info[3] << std::endl;
            }
        }
        
        auto final_info = env.getInfo();
        std::cout << "Episode completed - Total Reward: " << total_reward 
                 << ", Final Score: " << final_info[0] 
                 << ", Steps: " << steps << std::endl << std::endl;
    }
    
    std::cout << "RL Demo completed!" << std::endl;
}

void printUsage(const char* program_name) {
    std::cout << "Usage: " << program_name << " [mode]" << std::endl;
    std::cout << "Modes:" << std::endl;
    std::cout << "  human (default) - Play the game with keyboard controls" << std::endl;
    std::cout << "  rl             - Run RL agent demo" << std::endl;
    std::cout << "  help           - Show this help message" << std::endl;
}

int main(int argc, char* argv[]) {
    std::string mode = "human";
    
    if (argc > 1) {
        mode = argv[1];
    }
    
    if (mode == "help") {
        printUsage(argv[0]);
        return 0;
    } else if (mode == "human") {
        runHumanGame(argc, argv);
    } else if (mode == "rl") {
        runRLDemo();
    } else {
        std::cout << "Unknown mode: " << mode << std::endl;
        printUsage(argv[0]);
        return 1;
    }
    
    return 0;
}
