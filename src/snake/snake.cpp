#include "snake.h"

// Define Constructor
Snake::Snake()
{
    reset();
}

// Define Destructor
Snake::~Snake() {}

void Snake::reset()
{
    /**
     * Reset the snake's position and state.
     */
    // Clears private attributes
    positions_.clear();
    notPositions_.clear();

    // Initialize the snake's starting position
    std::vector<int> intStart1{ 0,0 };
    std::vector<int> intStart2{ -1,0 };
    positions_.push_back(intStart1);
    positions_.push_back(intStart2);
    for (int i = -NUMX / 2; i < NUMX / 2; ++i)
    {
        for (int j = -NUMY / 2; j < NUMY / 2; ++j)
        {
            if (!(((i == 0) && (j == 0)) || ((i == -1) && (j == 0))))
            {
                std::vector<int> coord{ i,j };
                notPositions_.insert(coord);
            }
        }
    }
}

void Snake::eat(Direction dir)
{
    /**
     * Handle the snake's movement in the specified direction, this is growth.
     * Note we do not have a Snake::move method because the move is effectively
     * Snake::eat chained with Snake::retract.
     */
    std::vector<int> newPos = positions_[0];
    if (dir == Direction::up) // Up
    {
        newPos[1] += 1;
        if (newPos[1] > NUMY / 2 - 1)
        {
            newPos[1] *= -1;
        }
    }
    else if (dir == Direction::down) // Down
    {
        if (newPos[1] <= -NUMY / 2)
        {
            newPos[1] *= -1;
        }
        newPos[1] -= 1;
    }
    else if (dir == Direction::left) // Left
    {
        if (newPos[0] <= -NUMX / 2)
        {
            newPos[0] *= -1;
        }
        newPos[0] -= 1;
    }
    else // Right
    {
        newPos[0] += 1;
        if (newPos[0] > NUMX / 2 - 1)
        {
            newPos[0] *= -1;
        }
    }

    positions_.push_front(newPos);
    if (notPositions_.find(newPos) != notPositions_.end())
        notPositions_.erase(notPositions_.find(newPos));
}

void Snake::retract()
{
    /**
     * Handle the retraction of the snake's tail.
     */
    notPositions_.insert(*(--positions_.end()));
    positions_.pop_back();
}

bool Snake::overlapCheck()
{
    /**
     * Check for overlap with the snake's own body,
     * i.e. if the snake has bitten itself or not.
     */
    for (auto it = ++positions_.begin(); it != positions_.end(); ++it)
    {
        if (positions_[0] == *it) return true;
    }
    return false;
}

const std::deque<std::vector<int>> Snake::getPositions()
{
    return positions_;
}
const std::vector<int> Snake::getPosition(unsigned int index)
{
    return positions_[index];
}
const std::set<std::vector<int>> Snake::getNotPositions()
{
    return notPositions_;
}
