#include "Building.h"

Building::Building(const std::string& name, int location,
        std::shared_ptr<VisitStrategy> visitStrategy,
        const std::string& owner, int purchaseCost,
        bool isMortgaged, const std::vector<int>& neighbourIndice)
    : Square(name, location, visitStrategy), owner(owner),
    purchaseCost(purchaseCost), isMortgaged(isMortgaged), neighbour() {
        for (index : neighbourIndice) {
            neighbour[index] = "BANK";
        }
}

std::string Building::getOwner() const {
    return owner;
}

void Building::setOwner(std::string owner) {
    this->owner = owner;
}

int Building::getPurchaseCost() const {
    return purchaseCost;
}

bool Building::getIsMortgaged() const {
    return isMortgaged;
}

void Building::setIsMortgaged(bool isMortgaged) {
    this->isMortgaged = isMortgaged;
}

void Building::notify(Subject& whoNotified) {
    Building* nb = dynamic_cast<Building*>(&whoNotified);
    if (nb != nullptr)
        neighbour[nb->getLocation()] = nb->getOwner();
}

int Building::numNeighbourOwned() const {
    int count = 0;
    for (auto& pair : neighbour) {
        if (pair.second == owner)
            count ++;
    }
    return count;
}

bool Building::allNeighbourOwned() const {
    for (auto& pair : neighbour) {
        if (pair.second != owner)
            return false;
    }
    return true;
}

