#pragma once

// Defining Global Constants
const int WINDOW_WIDTH = 500, WINDOW_HEIGHT = 500;
const int NUMX = 10, NUMY = 10;
const double DX = 2. / NUMX, DY = 2. / NUMY;
const unsigned long MAX_DELAY = 200000;

// Define Game State Parameters
enum class Direction
{
    /**
     * Enumeration for the direction of travel.
     */
    up, down, left, right
};

enum SnakeOrApple
{
    /**
     * Enumeration for the type of object, used for graphics coloring.
     */
    Snake = 0, // Snake
    Apple = 1, // Apple
    Head = 2  // Head of snake
};

class GameState
{
private:
    bool help_;
    bool paused_;
    bool game_over_;
    int game_speed_;
    unsigned int score_;
    unsigned int high_score_;
    Direction in_direction_;
    int test_counter_;
public:
    GameState();
    ~GameState();

    // help access
    void helpSet(const bool var);
    const bool helpGet() const;

    // paused access
    void pausedSet(const bool var);
    const bool pausedGet() const;

    // game_over access
    void gameOverSet(const bool var);
    const bool gameOverGet() const;

    // game_speed access
    void gameSpeedSet(const int var);
    const int gameSpeedGet() const;

    // score access
    void scoreSet(const unsigned int var);
    void incrementScore();
    const unsigned int scoreGet() const;

    // high_score access
    void highScoreSet(const unsigned int var);
    const unsigned int highScoreGet() const;
    void updateHighScore();

    // inDirection access
    void inDirectionSet(const Direction var);
    const Direction inDirectionGet() const;

    // test_counter access
    void testCounterSet(const int var);
    const int testCounterGet() const;
};