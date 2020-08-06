#include "AcademicBuilding.h"

AcademicBuilding::AcademicBuilding(const std::string name,
        int location, const std::string owner, int purchaseCost,
        bool isMortgaged, const std::vector<int>& neighbourIndice,
        int improvementCost, const std::vector<int>& improvementFee)
    : Building(name, location, owner, purchaseCost, isMortgaged,
            neighbourIndice), improvementCost(improvementCost),
            improvementFee(improvementFee), improvementLevel(0) {}

int AcademicBuilding::getImprovementCost() const {
    return improvementCost;
}

int AcademicBuilding::getImprovementLevel() const {
    return improvementLevel;
}

void AcademicBuilding::setImprovementLevel(int improvementLevel) {
    this->improvementLevel = improvementLevel;
}

bool AcademicBuilding::checkMonopoly() const {
    return allNeighbourOwned();
}

int AcademicBuilding::getFee() const {
    if (improvementLevel == 0) {
        if (checkMonopoly()) {
            return improvementFee[0] * 2;
        else
            return improvementFee[0];
    } else {
        return improvementFee[improvementLevel];
    }
}

void AcademicBuilding::acceptVisitor(Player& player) {
    player.setMoney(player.getMoney() - getFee());

