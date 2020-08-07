#include "Building.h"

Building::Building(const std::string& name, int location, const std::string &description,
        int purchaseCost, bool isMortgaged)
    : Square(name, location, description), purchaseCost(purchaseCost),
    isMortgaged(isMortgaged) {}

int Building:getPurchaseCost() const {
    return purchaseCost;
}

bool Building::getIsMortgaged() const {
    return isMortgaged;
}

void Building::setIsMortgaged(bool isMortgaged) {
    this->isMortgaged = isMortgaged;
}

int Building::calculateNetworth() const {
    return purchaseCost;
}
