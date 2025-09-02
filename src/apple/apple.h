#pragma once
#include <ctime>
#include <vector>
#include <set>
#include "../game/game.h"

class Apple
{
public:
    Apple(std::set<std::vector<int>> availablePos);
    ~Apple() {}

    void newPosition(std::set<std::vector<int>> availablePos);
    const std::vector<int> getPosition();

private:
    std::vector<int> position_; // Actual position scaled up to be from [-5, 4] in integers.
};