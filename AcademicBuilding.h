#ifndef _ACADEMICBUILDING_H_
#define _ACADEMICBUILDING_H_
#include "Building.h"

class AcademicBuilding : public Building {
    int improvementCost;
    std::vector<int> improvementFee;
    int improvementLevel;

  public:
    AcademicBuilding(const std::string name, int location,
            std::shared_ptr<VisitStrategy> visitStrategy,
            const std::string owner, int purchaseCost,
            bool isMortgaged, const std::vector<int>& neighbourIndice,
            int improvementCost, const std::vector<int>& improvementFee);
    virtual int getFee() const override;
    bool checkMonopoly() const;
    int getImprovementCost() const;
    int getImprovementLevel() const;
    void setImprovementLevel(int improvementLevel);
};

#endif

