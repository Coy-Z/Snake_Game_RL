# Snake Game RL - Windows Build Guide

## ✅ Quick Start (Windows)

### Prerequisites
- **MinGW-w64 with GCC** (MSYS2 recommended)
- **FreeGLUT** (already included in packages/ directory)

### Build Commands

#### Option 1: One-Line Build (Recommended)
```powershell
g++ -std=c++17 -Iinclude src/game_controller.cpp src/snake.cpp src/apple.cpp src/rl/rl_interface.cpp src/graphics.cpp src/main_refactored.cpp -o snakeGameRefactored.exe -lfreeglut -lopengl32 -lgdi32
```

#### Option 2: Step-by-Step Build
```powershell
# Compile source files
g++ -std=c++17 -Iinclude -c src/game_controller.cpp -o game_controller.o
g++ -std=c++17 -Iinclude -c src/snake.cpp -o snake.o
g++ -std=c++17 -Iinclude -c src/apple.cpp -o apple.o
g++ -std=c++17 -Iinclude -c src/rl/rl_interface.cpp -o rl_interface.o
g++ -std=c++17 -Iinclude -c src/graphics.cpp -o graphics.o
g++ -std=c++17 -Iinclude -c src/main_refactored.cpp -o main_refactored.o

# Link executable
g++ -o snakeGameRefactored.exe *.o -lfreeglut -lopengl32 -lgdi32
```

#### Option 3: Use Batch File
```cmd
build_windows.bat
```

#### Option 4: Use PowerShell Script
```powershell
# May need to set execution policy first:
# Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser

./build_and_run.ps1 rl    # Build and run RL demo
./build_and_run.ps1 human # Build and run human mode
```

### Running the Game

```powershell
# RL Demo Mode (headless, fast training)
./snakeGameRefactored.exe rl

# Human Player Mode (with controls)
./snakeGameRefactored.exe human
```

### Expected Output

**RL Mode:**
```
Starting Snake Game (RL Demo Mode)
Episode 1/10
Episode completed - Total Reward: -119, Final Score: 3, Steps: 53
Episode 2/10
Episode completed - Total Reward: 10, Final Score: 12, Steps: 23
...
RL Demo completed!
```

**Human Mode:**
```
Starting Snake Game (Human Player Mode)
Controls: WASD or Arrow Keys, P to pause, R to reset, ESC to quit
Note: Using headless graphics (OpenGL implementation not complete)
```

## 🎯 Success Criteria

✅ **Build Success** - No compilation errors  
✅ **RL Mode Works** - Random agent plays multiple episodes  
✅ **Proper Rewards** - Apple (+10), Collision (-100), Time (-1)  
✅ **Episode Management** - Automatic reset and termination  
✅ **Modern C++** - Smart pointers, RAII, proper namespaces  

## 🚀 Next Steps

1. **Experiment with RL** - The environment is ready for your algorithms!
2. **Implement Q-Learning** - Use the provided example in `src/rl_example.cpp`
3. **Add Full Graphics** - Implement OpenGL rendering for visual feedback
4. **Create Custom Agents** - DQN, Policy Gradients, etc.

## 🛠️ Troubleshooting

**"make not recognized"** → Use the G++ commands above instead  
**"ld.exe: cannot find -lfreeglut"** → Make sure FreeGLUT packages are in place  
**Missing headers** → Verify include paths with `-Iinclude`  

## 📚 Architecture Overview

- **`include/`** - Header files with clean interfaces
- **`src/`** - Implementation files
- **`src/rl/`** - RL-specific components
- **Modern C++17** - Smart pointers, optional types, proper const-correctness
- **Modular Design** - Easy to extend and test

**🎉 Your RL-ready Snake Game is now built and working!**
