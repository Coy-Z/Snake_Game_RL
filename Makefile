# Makefile for Snake Game RL - Refactored Version
# Supports both original and refactored builds

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -Iinclude
LIBS = -lfreeglut -lopengl32 -lgdi32

# Source files
CORE_SOURCES = src/game_controller.cpp src/snake.cpp src/apple.cpp
RL_SOURCES = src/rl/rl_interface.cpp
MAIN_REFACTORED = src/main_refactored.cpp

# Object files
CORE_OBJECTS = $(CORE_SOURCES:.cpp=.o)
RL_OBJECTS = $(RL_SOURCES:.cpp=.o)

# Original build (for comparison)
ORIGINAL_SOURCES = src/main.cpp src/apple/apple.cpp src/snake/snake.cpp src/graphics/graphics.cpp src/game/game.cpp

# Targets
.PHONY: all clean refactored original run-human run-rl install

# Default target - build refactored version
all: refactored

# Build refactored version
refactored: snakeGameRefactored.exe

snakeGameRefactored.exe: $(CORE_OBJECTS) $(RL_OBJECTS) $(MAIN_REFACTORED:.cpp=.o)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

# Build original version (for comparison)
original: snakeGame.exe

snakeGame.exe: $(ORIGINAL_SOURCES)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

# Object file rules
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Run targets
run-human: snakeGameRefactored.exe
	./snakeGameRefactored.exe human

run-rl: snakeGameRefactored.exe
	./snakeGameRefactored.exe rl

# Testing
test: snakeGameRefactored.exe
	@echo "Running basic functionality test..."
	./snakeGameRefactored.exe rl

# Documentation
docs:
	@echo "Generating documentation..."
	doxygen docs/Doxyfile

# Clean
clean:
	rm -f *.exe
	rm -f $(CORE_OBJECTS) $(RL_OBJECTS)
	rm -f src/main_refactored.o
	rm -f src/*.o src/*/*.o

# Install (copy to system path - adjust as needed)
install: snakeGameRefactored.exe
	cp snakeGameRefactored.exe /usr/local/bin/snake-game-rl 2>/dev/null || echo "Install manually or run with sudo"

# Help
help:
	@echo "Snake Game RL - Build System"
	@echo "Available targets:"
	@echo "  all (default) - Build refactored version"
	@echo "  refactored    - Build refactored version"
	@echo "  original      - Build original version"
	@echo "  run-human     - Run refactored version in human mode"
	@echo "  run-rl        - Run refactored version in RL demo mode"
	@echo "  test          - Run basic functionality tests"
	@echo "  docs          - Generate documentation"
	@echo "  clean         - Remove build artifacts"
	@echo "  install       - Install to system (requires permissions)"
	@echo "  help          - Show this help message"
