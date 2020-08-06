#ifndef _ACADEMICBUILDING_H_
#define _ACADEMICBUILDING_H_
#include "Building.h"

class AcademicBuilding : public Building {
    int improvementCost;
    std::vector<int> improvementFee;
    int improvementLevel;

  public:
    AcademicBuilding(const std::string name, int location,
            int purchaseCost, bool isMortgaged, int improvementCost,
            const std::vector<int>& improvementFee);
    int getImprovementCost() const;
    int getImprovementFee(int improvementLevel) const;
    int getImprovementLevel() const;
    void setImprovementLevel(int improvementLevel);
};

#endif

