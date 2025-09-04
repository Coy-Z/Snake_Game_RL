# Snake Game RL - Refactored Edition

A modern C++ implementation of the classic Snake Game, designed as an educational resource for learning software architecture, game development, and reinforcement learning concepts.

## 🎯 Project Goals

This refactored version transforms a basic Snake Game into:
- **📚 Educational Resource:** Demonstrates modern C++ best practices and software architecture patterns
- **🤖 RL Platform:** Provides a clean interface for experimenting with reinforcement learning algorithms
- **🏗️ Learning Foundation:** Serves as a foundation for building more complex game AI projects

## ✨ Key Features

### Modern C++ Architecture
- **RAII & Smart Pointers:** Automatic memory management with `std::unique_ptr`
- **Modern C++17:** Leverages contemporary language features
- **Const Correctness:** Immutable interfaces where appropriate
- **Exception Safety:** Proper error handling and input validation
- **Move Semantics:** Efficient resource transfer

### Design Patterns
- **Model-View-Controller:** Clear separation of concerns
- **Strategy Pattern:** Pluggable graphics backends
- **Observer Pattern:** Event-driven reward system
- **Factory Pattern:** Flexible component creation
- **Dependency Injection:** Loose coupling between components

### RL Integration
- **Standard Environment API:** Compatible with popular RL frameworks
- **Multiple Graphics Backends:** OpenGL, Console, and Headless modes
- **Flexible State Representation:** 17-dimensional feature vector
- **Configurable Rewards:** Customizable incentive structures
- **Agent Framework:** Easy integration of new RL algorithms

## 🚀 Quick Start

### Building the Project

#### Using Make (Recommended)
```bash
# Build the refactored version
make refactored

# Run human gameplay mode
make run-human

# Run RL demo
make run-rl

# Clean build artifacts
make clean
```

#### Using CMake
```bash
mkdir build && cd build
cmake ..
make
./SnakeGame human
```

### Running Different Modes

#### Human Gameplay
```bash
./snakeGameRefactored.exe human
```
**Controls:** WASD or Arrow Keys, P to pause, R to reset, ESC to quit

#### RL Demo with Random Agent
```bash
./snakeGameRefactored.exe rl
```

#### Q-Learning Training Example
```bash
# Compile RL example
g++ -std=c++17 -Iinclude src/rl_example.cpp src/*.cpp src/rl/*.cpp -o rl_example

# Train a Q-Learning agent
./rl_example train 1000

# Evaluate the trained agent
./rl_example evaluate 10

# Compare random vs trained agent
./rl_example compare
```

## 📁 Project Structure

```
├── include/                    # Header files
│   ├── common_types.h         # Shared types and constants
│   ├── game_controller.h      # Main game controller
│   ├── snake.h                # Snake entity
│   ├── apple.h                # Apple entity
│   ├── graphics.h             # Graphics abstraction
│   └── rl/
│       ├── rl_interface.h     # RL environment & agent interfaces
│       └── q_learning_agent.h # Q-Learning implementation
├── src/                       # Implementation files
│   ├── game_controller.cpp
│   ├── snake.cpp
│   ├── apple.cpp
│   ├── main_refactored.cpp    # Main application
│   ├── rl_example.cpp         # RL training example
│   └── rl/
│       ├── rl_interface.cpp
│       └── q_learning_agent.cpp
├── original_src/              # Original code (for comparison)
├── CMakeLists.txt             # CMake build configuration
├── Makefile                   # Make build configuration
└── cpp_refactor_notes.ipynb   # Detailed refactoring documentation
```

## 🧠 Reinforcement Learning Interface

### Environment API
```cpp
SnakeGame::RL::SnakeEnvironment env(headless=true);

// Standard RL loop
auto state = env.reset();
while (!env.isDone()) {
    int action = agent.selectAction(state);
    auto [next_state, reward] = env.step(action);
    agent.update(state, action, reward, next_state, env.isDone());
    state = next_state;
}
```

### State Representation (17 dimensions)
- Snake head position (2D, normalized)
- Apple position (2D, normalized)
- Current direction (4D, one-hot encoded)
- Distance to walls (4D, normalized)
- Obstacle detection in 4 directions (4D, binary)
- Snake length (1D, normalized)

### Action Space
- `0`: UP
- `1`: DOWN  
- `2`: LEFT
- `3`: RIGHT

### Reward Structure
- **Apple eaten:** +10
- **Collision:** -100
- **Time penalty:** -1 per step

## 🎓 Educational Benefits

### Software Architecture Concepts
- **Separation of Concerns:** Clear boundaries between game logic, rendering, and AI
- **Interface Design:** Clean APIs for extensibility
- **Resource Management:** Modern C++ memory safety practices
- **Error Handling:** Robust exception safety guarantees

### Game Development Patterns
- **Game Loop:** Frame-based state updates
- **Entity Management:** Object-oriented game entities
- **Input Handling:** Decoupled control systems
- **Rendering Pipeline:** Abstracted graphics operations

### Machine Learning Integration
- **Environment Design:** Standard RL environment patterns
- **State Engineering:** Feature representation for learning
- **Reward Shaping:** Incentive design principles
- **Agent Architecture:** Modular AI system design

## 🔧 Extending the Project

### Adding New RL Algorithms

1. **Inherit from Agent interface:**
```cpp
class MyAgent : public SnakeGame::RL::Agent {
public:
    int selectAction(const std::vector<double>& state) override;
    void update(const std::vector<double>& state, int action, 
                double reward, const std::vector<double>& next_state, bool done) override;
};
```

2. **Implement your algorithm in the methods above**

3. **Use with the environment:**
```cpp
SnakeGame::RL::SnakeEnvironment env;
MyAgent agent;
agent.train(env, 1000);
```

### Adding New Graphics Backends

1. **Inherit from Graphics interface:**
```cpp
class MyGraphics : public SnakeGame::Graphics {
    // Implement all pure virtual methods
};
```

2. **Register with the factory:**
```cpp
auto graphics = std::make_unique<MyGraphics>();
game.setGraphics(std::move(graphics));
```

## 📊 Performance Considerations

- **Headless Mode:** Disables rendering for faster training (up to 10,000+ steps/second)
- **State Encoding:** Efficient 17-dimensional vector representation
- **Memory Management:** Smart pointers prevent memory leaks
- **Vectorization Ready:** Environment can be easily extended for batch processing

## 🔬 Comparison with Original

| Aspect | Original | Refactored |
|--------|----------|------------|
| **Architecture** | Global variables, tight coupling | OOP with dependency injection |
| **Memory Safety** | Manual management | RAII + smart pointers |
| **Extensibility** | Hard-coded logic | Plugin-based architecture |
| **RL Support** | None | Full environment + agent framework |
| **Graphics** | Fixed OpenGL | Multiple backends (OpenGL/Console/Headless) |
| **Testing** | Difficult | Modular, easily testable |
| **Documentation** | Minimal | Comprehensive |

## 🛠️ Requirements

- **C++17 compatible compiler** (GCC 7+, Clang 5+, MSVC 2017+)
- **OpenGL/FreeGLUT** (for graphics mode)
- **CMake 3.15+** (optional, for CMake build)

## 📖 Learning Path

1. **Start with the refactored code structure** - understand the class hierarchy
2. **Experiment with the human gameplay** - see how the MVC pattern works
3. **Try the random agent** - understand the RL interface
4. **Implement Q-Learning** - follow the provided example
5. **Create your own agent** - implement DQN, policy gradients, etc.
6. **Extend the environment** - add obstacles, multiple apples, etc.

## 🤝 Contributing

This project is designed for educational purposes. Feel free to:
- Add new RL algorithms
- Implement additional graphics backends  
- Create more sophisticated environments
- Add unit tests
- Improve documentation

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- Original Snake Game implementation (foundation for refactoring)
- OpenGL and FreeGLUT communities
- Reinforcement Learning community for interface standards
- Modern C++ community for best practices guidance

---

**Happy Learning! 🐍🎮🤖**
