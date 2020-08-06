#include "Gym.h"
#include <cstdlib>

Gym::Gym(const std::string name, int location,
        const std::string owner, int purchaseCost,
        bool isMortgaged, const std::vector<int>& neighbourIndice)
    : Building(name, location, owner purchaseCost,
            isMortgaged, neighbourIndice) {}

int Gym::getFee() const {
    int roll1 = rand() % 6;
    if (roll1 == 0)
        roll1 = 6;
    int roll2 = rand() % 6;
    if (roll2 == 0)
        roll2 = 6;
    if (numNeighbourOwned() == 0)
        return (roll1 + roll2) * 4;
    else
        return (roll1 + roll2) * 10;
}

