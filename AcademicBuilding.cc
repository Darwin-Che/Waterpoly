#include "AcademicBuilding.h"

AcademicBuilding::AcademicBuilding(const std::string& name,
    int location, const std::string &t_description, int purchaseCost, bool isMortgaged,
    int improvementCost, const std::vector<int>& improvementFee)
    : Building(name, location, t_description, purchaseCost, isMortgaged),
    improvementCost(improvementCost),
    improvementFee(improvementFee), improvementLevel(0) {
        description = "AcademicBuilding";
    }

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
    notifyObservers();
}

int AcademicBuilding::calculateNetworth() const {
    return getPurchaseCost() + improvementLevel * improvementCost;
}

std::string AcademicBuilding::getInfo() const
{
    return Building::getInfo() + 
        + "\n\tImproveLevel: " + std::to_string(getImprovementLevel())
        + "\n\tPurchase Cost: " + std::to_string(getPurchaseCost())
        + "\n\tImprovement Cost: " + std::to_string(getImprovementCost())
        + "\n\tTuition with Improvements: " + std::to_string(improvementFee[0])
        + " " + std::to_string(improvementFee[1])
        + " " + std::to_string(improvementFee[2])
        + " " + std::to_string(improvementFee[3])
        + " " + std::to_string(improvementFee[4]);
}

std::string AcademicBuilding::saveInfo() const
{

    if (getIsMortgaged()) {
        return "-1";
    }
    else {
        return std::to_string(getImprovementLevel());
    }

}

Info AcademicBuilding::getSubjectInfo(){
    return Info{InfoType::academic,{},{getLocation(),improvementLevel},{owner},{}};
}
