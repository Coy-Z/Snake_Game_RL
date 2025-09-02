#include "game.h"


GameState::GameState()
{
    help_ = false;
    paused_ = false;
    game_over_ = false;
    game_speed_ = 1;
    score_ = 0;
    high_score_ = 0;
    in_direction_ = Direction::right;
    test_counter_ = 0;
}

GameState::~GameState() {}

// help access
void GameState::helpSet(const bool var) { help_ = var; }
const bool GameState::helpGet() const { return help_; }

// paused access
void GameState::pausedSet(const bool var) { paused_ = var; }
const bool GameState::pausedGet() const { return paused_; }

// game_over access
void GameState::gameOverSet(const bool var) { game_over_ = var; }
const bool GameState::gameOverGet() const { return game_over_; }

// game_speed access
void GameState::gameSpeedSet(const int var) { game_speed_ = var; }
const int GameState::gameSpeedGet() const { return game_speed_; }

// score access
void GameState::scoreSet(const unsigned int var) { score_ = var; }
void GameState::incrementScore() { ++score_; }
const unsigned int GameState::scoreGet() const { return score_; }

// high_score access
void GameState::highScoreSet(const unsigned int var) { high_score_ = var; }
const unsigned int GameState::highScoreGet() const { return high_score_; }
void GameState::updateHighScore() { high_score_ = score_; }

// inDirection access
void GameState::inDirectionSet(const Direction var) { in_direction_ = var; }
const Direction GameState::inDirectionGet() const { return in_direction_; }

// test_counter access
void GameState::testCounterSet(const int var) { test_counter_ = var; }
const int GameState::testCounterGet() const { return test_counter_; }
