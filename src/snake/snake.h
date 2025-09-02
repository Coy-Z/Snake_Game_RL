#pragma once
#include <deque>
#include <vector>
#include <set>
#include "../game/game.h"

class Snake {
public:
    Snake();
    ~Snake();

    void reset();
    void eat(Direction dir);
    void retract();
    bool overlapCheck();
    const std::deque<std::vector<int>> getPositions();
    const std::vector<int> getPosition(unsigned int index);
    const std::set<std::vector<int>> getNotPositions();
    
private:
    std::deque<std::vector<int>> positions_;
    std::set<std::vector<int>> notPositions_;
};
