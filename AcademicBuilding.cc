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

int AcademicBuilding::getImprovementFee() const {
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

std::string AcademicBuilding::getInfo() const 
{
    return getName() + "(Academic Building)"
                     + " ; Mortgaged - " + (getIsMortgaged() ? "true" : "false")
                     + " ; ImproveLevel - " + std::to_string(getImprovementLevel())
                     + " ; Cost Information: \n"
                     + " \t Purchase Cost: " + std::to_string(getPurchaseCost())
                     + " \t Improvement Cost: " + std::to_string(getImprovementCost());
                     + " \t Tuition with Improvements: " + std::to_string(improvementFee[0]) 
                     + " " + std::to_string(improvementFee[1])
                     + " " + std::to_string(improvementFee[2])
                     + " " + std::to_string(improvementFee[3])
                     + " " + std::to_string(improvementFee[4])
                     + " \n";
}

