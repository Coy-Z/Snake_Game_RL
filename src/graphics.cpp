#include "graphics.h"
#include "snake.h"
#include "apple.h"
#include <iostream>
#include <sstream>

#ifdef __APPLE__
#include<GLUT/glut.h>
#else
#include "../packages/freeglut.3.0.0.v140.1.0.2/sources/GL/glut.h"
#endif

namespace SnakeGame {

// Static instance for OpenGL callbacks
OpenGLGraphics* OpenGLGraphics::instance_ = nullptr;

// OpenGLGraphics implementation
OpenGLGraphics::OpenGLGraphics() 
    : window_id_(0)
    , initialized_(false)
    , pending_direction_(std::nullopt) {
    instance_ = this;
}

OpenGLGraphics::~OpenGLGraphics() {
    shutdown();
    instance_ = nullptr;
}

void OpenGLGraphics::initialize() {
    if (initialized_) return;
    
    // OpenGL is already initialized by main, just set our flag
    initialized_ = true;
    std::cout << "OpenGL Graphics initialized successfully!" << std::endl;
}

void OpenGLGraphics::clear() {
    glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLGraphics::present() {
    glutSwapBuffers();
}

void OpenGLGraphics::shutdown() {
    if (initialized_ && window_id_ != 0) {
        glutDestroyWindow(window_id_);
        window_id_ = 0;
    }
    initialized_ = false;
}

void OpenGLGraphics::drawSnake(const Snake& snake) {
    const auto& positions = snake.getAllPositions();
    
    for (size_t i = 0; i < positions.size(); ++i) {
        EntityType type = (i == 0) ? EntityType::SNAKE_HEAD : EntityType::SNAKE_BODY;
        drawRectangle(positions[i], type);
    }
}

void OpenGLGraphics::drawApple(const Apple& apple) {
    drawRectangle(apple.getPosition(), EntityType::APPLE);
}

void OpenGLGraphics::drawScore(unsigned int score, unsigned int high_score) {
    std::stringstream ss;
    ss << "Score: " << score << "  High: " << high_score;
    drawText(-0.95, 0.9, ss.str(), GLUT_BITMAP_HELVETICA_18);
}

void OpenGLGraphics::drawGameOver() {
    drawText(-0.2, 0.0, "GAME OVER", GLUT_BITMAP_HELVETICA_18);
    drawText(-0.3, -0.1, "Press R to restart", GLUT_BITMAP_HELVETICA_12);
}

void OpenGLGraphics::drawPaused() {
    drawText(-0.1, 0.0, "PAUSED", GLUT_BITMAP_HELVETICA_18);
    drawText(-0.2, -0.1, "Press P to continue", GLUT_BITMAP_HELVETICA_12);
}

void OpenGLGraphics::drawHelp() {
    drawText(-0.4, 0.3, "SNAKE GAME CONTROLS", GLUT_BITMAP_HELVETICA_18);
    drawText(-0.3, 0.1, "Arrow Keys or WASD: Move", GLUT_BITMAP_HELVETICA_12);
    drawText(-0.3, 0.0, "P: Pause/Unpause", GLUT_BITMAP_HELVETICA_12);
    drawText(-0.3, -0.1, "R: Restart", GLUT_BITMAP_HELVETICA_12);
    drawText(-0.3, -0.2, "ESC: Quit", GLUT_BITMAP_HELVETICA_12);
    drawText(-0.3, -0.4, "Press any key to continue", GLUT_BITMAP_HELVETICA_12);
}

void OpenGLGraphics::setWindowTitle(const std::string& title) {
    if (initialized_) {
        glutSetWindowTitle(title.c_str());
    }
}

bool OpenGLGraphics::shouldClose() const {
    return false; // GLUT handles window closing
}

std::optional<Direction> OpenGLGraphics::getInputDirection() {
    auto direction = pending_direction_;
    pending_direction_ = std::nullopt;
    return direction;
}

bool OpenGLGraphics::isKeyPressed(char key) const {
    // This would require additional state tracking in a real implementation
    return false;
}

void OpenGLGraphics::drawRectangle(const Position& pos, EntityType type) {
    if (pos.size() < 2) return;
    
    // Convert game coordinates to screen coordinates
    // Game coordinates: [-5, 4] for a 10x10 grid
    // Screen coordinates: [-1, 1]
    // Map [-5, 4] to [-0.9, 0.9] evenly
    double x = ((static_cast<double>(pos[0]) - (-GameConfig::GRID_SIZE_X / 2)) / (GameConfig::GRID_SIZE_X - 1)) * 1.8 - 0.9;
    double y = ((static_cast<double>(pos[1]) - (-GameConfig::GRID_SIZE_Y / 2)) / (GameConfig::GRID_SIZE_Y - 1)) * 1.8 - 0.9;
    
    // Set color based on entity type
    switch (type) {
        case EntityType::SNAKE_HEAD:
            glColor3f(0.0f, 1.0f, 0.0f); // Bright green for snake head
            break;
        case EntityType::SNAKE_BODY:
            glColor3f(0.0f, 0.7f, 0.0f); // Darker green for snake body
            break;
        case EntityType::APPLE:
            glColor3f(1.0f, 0.0f, 0.0f); // Red for apple
            break;
    }
    
    // Draw rectangle
    double size = 0.08; // Size of each cell
    
    glBegin(GL_QUADS);
    glVertex2d(x - size, y - size);
    glVertex2d(x + size, y - size);
    glVertex2d(x + size, y + size);
    glVertex2d(x - size, y + size);
    glEnd();
}

void OpenGLGraphics::drawText(double x, double y, const std::string& text, void* font) {
    glColor3f(1.0f, 1.0f, 1.0f); // White text
    glRasterPos2d(x, y);
    
    for (char c : text) {
        glutBitmapCharacter(font, c);
    }
}

Position OpenGLGraphics::worldToScreen(const Position& world_pos) const {
    if (world_pos.size() < 2) return world_pos;
    
    int screen_x = static_cast<int>((world_pos[0] + 1.0) / 2.0 * GameConfig::WINDOW_WIDTH);
    int screen_y = static_cast<int>((1.0 - world_pos[1]) / 2.0 * GameConfig::WINDOW_HEIGHT);
    
    return {screen_x, screen_y};
}

// Static callback functions
void OpenGLGraphics::displayCallback() {
    if (instance_) {
        // This will be called by the main game loop
        glutPostRedisplay();
    }
}

void OpenGLGraphics::reshapeCallback(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

void OpenGLGraphics::keyboardCallback(unsigned char key, int x, int y) {
    if (!instance_) return;
    
    switch (key) {
        case 'w':
        case 'W':
            instance_->pending_direction_ = Direction::UP;
            break;
        case 's':
        case 'S':
            instance_->pending_direction_ = Direction::DOWN;
            break;
        case 'a':
        case 'A':
            instance_->pending_direction_ = Direction::LEFT;
            break;
        case 'd':
        case 'D':
            instance_->pending_direction_ = Direction::RIGHT;
            break;
        case 27: // ESC
            exit(0);
            break;
    }
}

void OpenGLGraphics::specialKeyCallback(int key, int x, int y) {
    if (!instance_) return;
    
    switch (key) {
        case GLUT_KEY_UP:
            instance_->pending_direction_ = Direction::UP;
            break;
        case GLUT_KEY_DOWN:
            instance_->pending_direction_ = Direction::DOWN;
            break;
        case GLUT_KEY_LEFT:
            instance_->pending_direction_ = Direction::LEFT;
            break;
        case GLUT_KEY_RIGHT:
            instance_->pending_direction_ = Direction::RIGHT;
            break;
    }
}

// Factory function implementation
std::unique_ptr<Graphics> createGraphics(const std::string& type) {
    if (type == "headless") {
        return std::make_unique<HeadlessGraphics>();
    } else if (type == "opengl") {
        return std::make_unique<OpenGLGraphics>();
    }
    
    // Default to OpenGL for visual mode
    return std::make_unique<OpenGLGraphics>();
}

} // namespace SnakeGame
