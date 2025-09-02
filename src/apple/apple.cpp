#include "apple.h"

class ApplePos
{
private:
    std::vector<int> position; // Actual position scaled up to be from [-5, 4] in integers.
public:
    void newPosition(std::set<std::vector<int>> availablePos)
    {
        std::srand(std::time(NULL));
        int index = std::rand() % availablePos.size();
        auto it = availablePos.begin();
        std::advance(it, index);
        position = *it;
    }

    ApplePos(std::set<std::vector<int>> availablePos)
    {
        this->newPosition(availablePos);
    }

    const std::vector<int> getPosition()
    {
        return position;
    }

    ~ApplePos() {}
};

Apple::Apple(std::set<std::vector<int>> availablePos)
{
    this->newPosition(availablePos);
}

Apple::~Apple() {}

void Apple::newPosition(std::set<std::vector<int>> availablePos)
{
    std::srand(std::time(NULL));
    int index = std::rand() % availablePos.size();
    auto it = availablePos.begin();
    std::advance(it, index);
    position_ = *it;
}

const std::vector<int> Apple::getPosition()
{
    return position_;
}
