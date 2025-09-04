@echo off
echo Building Snake Game RL - Refactored Version...

REM Clean previous builds
if exist snakeGameRefactored.exe del snakeGameRefactored.exe
if exist *.o del *.o

REM Build the refactored version
echo Compiling source files...
g++ -std=c++17 -Wall -Wextra -O2 -Iinclude -c src/game_controller.cpp -o game_controller.o
if %errorlevel% neq 0 (
    echo Error compiling game_controller.cpp
    pause
    exit /b 1
)

g++ -std=c++17 -Wall -Wextra -O2 -Iinclude -c src/snake.cpp -o snake.o
if %errorlevel% neq 0 (
    echo Error compiling snake.cpp
    pause
    exit /b 1
)

g++ -std=c++17 -Wall -Wextra -O2 -Iinclude -c src/apple.cpp -o apple.o
if %errorlevel% neq 0 (
    echo Error compiling apple.cpp
    pause
    exit /b 1
)

g++ -std=c++17 -Wall -Wextra -O2 -Iinclude -c src/rl/rl_interface.cpp -o rl_interface.o
if %errorlevel% neq 0 (
    echo Error compiling rl_interface.cpp
    pause
    exit /b 1
)

g++ -std=c++17 -Wall -Wextra -O2 -Iinclude -c src/main_refactored.cpp -o main_refactored.o
if %errorlevel% neq 0 (
    echo Error compiling main_refactored.cpp
    pause
    exit /b 1
)

echo Linking executable...
g++ -o snakeGameRefactored.exe game_controller.o snake.o apple.o rl_interface.o main_refactored.o -lfreeglut -lopengl32 -lgdi32
if %errorlevel% neq 0 (
    echo Error linking executable
    pause
    exit /b 1
)

echo Build successful!
echo.
echo To run the game:
echo   snakeGameRefactored.exe human    (for human player)
echo   snakeGameRefactored.exe rl       (for RL demo)
echo.
pause
