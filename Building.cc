#include "Building.h"
#include <iostream>

Building::Building(const std::string& name, int location, const std::string &t_description,
    int purchaseCost, bool isMortgaged)
    : Square(name, location, t_description), purchaseCost(purchaseCost),
    isMortgaged(isMortgaged) {
        description = "Building";
        owner = ' ';
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

char Building::setOwner(char o) {
    owner = o;
    std::cout << getName() <<","<< o << std::endl;
    notifyObservers();
}

int Building::calculateNetworth() const {
    return purchaseCost;
}

std::string Building::getInfo() const
{
    return Square::getInfo() + "\n\tMortgaged: " + (getIsMortgaged() ? "true" : "false");
}

std::string Building::saveInfo() const
{
    if (getIsMortgaged()) return "-1";
    return "0";
}

Info Building::getSubjectInfo(){
    return Info{InfoType::building,{},{getLocation()},{owner},{}};
}
