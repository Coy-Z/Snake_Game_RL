#pragma once

#include "common_types.h"
#include <string>
#include <memory>

namespace SnakeGame {

// Forward declarations
class Snake;
class Apple;

/**
 * @brief Abstract graphics interface
 * 
 * This interface allows for different rendering backends (OpenGL, console, headless)
 * and makes the graphics system modular and testable.
 */
class Graphics {
public:
    virtual ~Graphics() = default;
    
    // Core rendering interface
    virtual void initialize() = 0;
    virtual void clear() = 0;
    virtual void present() = 0;
    virtual void shutdown() = 0;
    
    // Entity rendering
    virtual void drawSnake(const Snake& snake) = 0;
    virtual void drawApple(const Apple& apple) = 0;
    virtual void drawScore(unsigned int score, unsigned int high_score) = 0;
    virtual void drawGameOver() = 0;
    virtual void drawPaused() = 0;
    virtual void drawHelp() = 0;
    
    // Window management
    virtual void setWindowTitle(const std::string& title) = 0;
    virtual bool shouldClose() const = 0;
    
    // Input handling (for human players)
    virtual std::optional<Direction> getInputDirection() = 0;
    virtual bool isKeyPressed(char key) const = 0;
};

/**
 * @brief OpenGL-based graphics implementation
 * 
 * This class implements the Graphics interface using OpenGL and FreeGLUT
 * for traditional windowed gameplay.
 */
class OpenGLGraphics : public Graphics {
public:
    OpenGLGraphics();
    ~OpenGLGraphics() override;
    
    // Graphics interface implementation
    void initialize() override;
    void clear() override;
    void present() override;
    void shutdown() override;
    
    // Entity rendering
    void drawSnake(const Snake& snake) override;
    void drawApple(const Apple& apple) override;
    void drawScore(unsigned int score, unsigned int high_score) override;
    void drawGameOver() override;
    void drawPaused() override;
    void drawHelp() override;
    
    // Window management
    void setWindowTitle(const std::string& title) override;
    bool shouldClose() const override;
    
    // Input handling
    std::optional<Direction> getInputDirection() override;
    bool isKeyPressed(char key) const override;
    
    // OpenGL-specific methods
    void setReshapeCallback();
    void setKeyboardCallbacks();
    
private:
    int window_id_;
    bool initialized_;
    std::optional<Direction> pending_direction_;
    
    // Helper methods
    void drawRectangle(const Position& pos, EntityType type);
    void drawText(double x, double y, const std::string& text, void* font);
    Position worldToScreen(const Position& world_pos) const;
    
    // Static callback functions for GLUT
    static void displayCallback();
    static void reshapeCallback(int width, int height);
    static void keyboardCallback(unsigned char key, int x, int y);
    static void specialKeyCallback(int key, int x, int y);
    
    // Static instance pointer for callbacks
    static OpenGLGraphics* instance_;
    
    // Copy prevention
    OpenGLGraphics(const OpenGLGraphics&) = delete;
    OpenGLGraphics& operator=(const OpenGLGraphics&) = delete;
};

/**
 * @brief Headless graphics implementation for RL training
 * 
 * This class provides a no-op graphics implementation for training
 * RL agents without visual output.
 */
class HeadlessGraphics : public Graphics {
public:
    HeadlessGraphics() = default;
    ~HeadlessGraphics() override = default;
    
    // No-op implementations
    void initialize() override {}
    void clear() override {}
    void present() override {}
    void shutdown() override {}
    
    void drawSnake(const Snake& snake) override {}
    void drawApple(const Apple& apple) override {}
    void drawScore(unsigned int score, unsigned int high_score) override {}
    void drawGameOver() override {}
    void drawPaused() override {}
    void drawHelp() override {}
    
    void setWindowTitle(const std::string& title) override {}
    bool shouldClose() const override { return false; }
    
    std::optional<Direction> getInputDirection() override { return std::nullopt; }
    bool isKeyPressed(char key) const override { return false; }
};

/**
 * @brief Console-based graphics implementation
 * 
 * This class provides a simple text-based rendering for debugging
 * or lightweight testing.
 */
class ConsoleGraphics : public Graphics {
public:
    ConsoleGraphics();
    ~ConsoleGraphics() override = default;
    
    // Console-specific implementations
    void initialize() override;
    void clear() override;
    void present() override;
    void shutdown() override;
    
    void drawSnake(const Snake& snake) override;
    void drawApple(const Apple& apple) override;
    void drawScore(unsigned int score, unsigned int high_score) override;
    void drawGameOver() override;
    void drawPaused() override;
    void drawHelp() override;
    
    void setWindowTitle(const std::string& title) override;
    bool shouldClose() const override;
    
    std::optional<Direction> getInputDirection() override;
    bool isKeyPressed(char key) const override;
    
private:
    std::vector<std::vector<char>> screen_buffer_;
    bool should_close_;
    
    void initializeScreenBuffer();
    void printScreenBuffer() const;
    char getEntityChar(EntityType type) const;
};

// Factory function for creating graphics instances
std::unique_ptr<Graphics> createGraphics(const std::string& type = "opengl");

} // namespace SnakeGame
