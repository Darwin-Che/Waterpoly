#include "AcademicBuilding.h"

AcademicBuilding::AcademicBuilding(const std::string name,
        int location, int purchaseCost, bool isMortgaged,
        int improvementCost, const std::vector<int>& improvementFee)
    : Building(name, location, purchaseCost, isMortgaged),
    improvementCost(improvementCost),
    improvementFee(improvementFee), improvementLevel(0) {}

int AcademicBuilding::getImprovementCost() const {
    return improvementCost;
}

int AcademicBuilding::getImprovementFee(int improvementLevel) const {
    return improvementFee[improvementLevel];
}

int AcademicBuilding::getImprovementLevel() const {
    return improvementLevel;
}

void AcademicBuilding::setImprovementLevel(int improvementLevel) {
    this->improvementLevel = improvementLevel;
}

int AcademicBuilding::calculateNetworth() const {
    return getPurchaseCost() + improvementLevel * improvementCost;
}

