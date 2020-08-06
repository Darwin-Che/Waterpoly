#include "Residence.h"

Residence::Residence(const std::string name, int location,
        const std::string owner, int purchaseCost,
        bool isMortgaged, const std::vector<int>& neighbourIndice)
    : Building(name, location, owner, purchaseCost,
            isMortgaged, neighbourIndice) {}

int Residence::getFee() const {
    int numOwned = numNeighbourOwned();
    if (numOwned == 0)
        return 25;
    else if (numOwned == 1)
        return 50;
    else if (numOwned == 2)
        return 100;
    else
        return 200;
}

